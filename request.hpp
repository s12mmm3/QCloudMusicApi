#include <iostream>
#include <QObject>
#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QTimer>
#include <QVariantMap>
#include <QByteArray>
#include <QObject>
#include <QRandomGenerator>
#include <QNetworkCookie>
#include <QNetworkProxyFactory>
#include <QNetworkReply>
#include <QUrlQuery>

#include "crypto.hpp"
#include "config.h"

static QString chooseUserAgent(QString ua = "") {
    const QVariantMap userAgentList = {
        {
            "mobile",
            QVariant::fromValue(QList<QString>({
                // iOS 13.5.1 14.0 beta with safari
                R"(Mozilla/5.0 (iPhone; CPU iPhone OS 13_5_1 like Mac OS X) AppleWebKit/605.1.15 (KHTML, like Gecko) Version/13.1.1 Mobile/15E148 Safari/604.1)",
                R"(Mozilla/5.0 (iPhone; CPU iPhone OS 14_0 like Mac OS X) AppleWebKit/605.1.15 (KHTML, like Gecko) Version/14.0 Mobile/15E148 Safari/604.)",
                // iOS with qq micromsg
                R"(Mozilla/5.0 (iPhone; CPU iPhone OS 13_5_1 like Mac OS X) AppleWebKit/602.1.50 (KHTML like Gecko) Mobile/14A456 QQ/6.5.7.408 V1_IPH_SQ_6.5.7_1_APP_A Pixel/750 Core/UIWebView NetType/4G Mem/103)",
                R"(Mozilla/5.0 (iPhone; CPU iPhone OS 13_5_1 like Mac OS X) AppleWebKit/605.1.15 (KHTML, like Gecko) Mobile/15E148 MicroMessenger/7.0.15(0x17000f27) NetType/WIFI Language/zh)",
                // Android -> Huawei Xiaomi
                R"(Mozilla/5.0 (Linux; Android 9; PCT-AL10) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/70.0.3538.64 HuaweiBrowser/10.0.3.311 Mobile Safari/537.36)",
                R"(Mozilla/5.0 (Linux; U; Android 9; zh-cn; Redmi Note 8 Build/PKQ1.190616.001) AppleWebKit/537.36 (KHTML, like Gecko) Version/4.0 Chrome/71.0.3578.141 Mobile Safari/537.36 XiaoMi/MiuiBrowser/12.5.22)",
                // Android + qq micromsg
                R"(Mozilla/5.0 (Linux; Android 10; YAL-AL00 Build/HUAWEIYAL-AL00; wv) AppleWebKit/537.36 (KHTML, like Gecko) Version/4.0 Chrome/78.0.3904.62 XWEB/2581 MMWEBSDK/200801 Mobile Safari/537.36 MMWEBID/3027 MicroMessenger/7.0.18.1740(0x27001235) Process/toolsmp WeChat/arm64 NetType/WIFI Language/zh_CN ABI/arm64)",
                R"(Mozilla/5.0 (Linux; U; Android 8.1.0; zh-cn; BKK-AL10 Build/HONORBKK-AL10) AppleWebKit/537.36 (KHTML, like Gecko) Version/4.0 Chrome/66.0.3359.126 MQQBrowser/10.6 Mobile Safari/537.36)"
            }))
        },
        {
            "pc",
            QVariant::fromValue(QList<QString>({
                // macOS 10.15.6  Firefox / Chrome / Safari
                R"(Mozilla/5.0 (Macintosh; Intel Mac OS X 10.15; rv:80.0) Gecko/20100101 Firefox/80.0)",
                R"(Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_6) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/86.0.4240.30 Safari/537.36)",
                R"(Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_6) AppleWebKit/605.1.15 (KHTML, like Gecko) Version/13.1.2 Safari/605.1.15)",
                // Windows 10 Firefox / Chrome / Edge
                R"(Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:80.0) Gecko/20100101 Firefox/80.0)",
                R"(Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/86.0.4240.30 Safari/537.36)",
                R"(Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/42.0.2311.135 Safari/537.36 Edge/13.10586)"
                // Linux 就算了
            }))
        }
    };
    auto list = userAgentList["mobile"].toStringList() + userAgentList["pc"].toStringList();
    auto realUserAgentList = userAgentList[ua].isValid() ? userAgentList[ua].toStringList() : list;
    return QList({"mobile", "pc", ""}).indexOf(ua) > -1 ? realUserAgentList[QRandomGenerator::global()->generate() % realUserAgentList.size()]
                                                        : ua;
}

static auto createRequest(QNetworkAccessManager::Operation method, QUrl url, QVariantMap data, QVariantMap options) {
    qDebug().noquote() << "method:" << method;
    qDebug().noquote() << "url:" << url;
    qDebug().noquote() << "data:" << data;
    qDebug().noquote() << "options:" << options;
    QNetworkRequest request;

    request.setHeader(QNetworkRequest::UserAgentHeader, chooseUserAgent(options["ua"].toString()));

    if(method == QNetworkAccessManager::PostOperation) {
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    }
    if(url.toString().contains("music.163.com")) {
        request.setRawHeader("Referer", "https://music.163.com");
    }
    QString ip;
    if(options["realIP"].isValid()) ip = options["realIP"].toString();
    else if(options["ip"].isValid()) ip = options["ip"].toString();
    else ip = "";
    if(ip.length() > 0) {
        request.setRawHeader("X-Real-IP", ip.toUtf8());
        request.setRawHeader("X-Forwarded-For", ip.toUtf8());
    }
//    request.setRawHeader("X-Real-IP", "118.88.88.88");
    if(options["cookie"].userType() == QMetaType::QVariantMap) {
        auto cookie = options["cookie"].toMap();
        auto randomBytes = []() {
            QByteArray bytes;
            for(int i = 0; i < 16; ++i) {
                bytes.append(QRandomGenerator::global()->generate());
            }
            return bytes;
        };
        cookie.insert({
            { "__remember_me", true },
//            { "NMTID", randomBytes().toHex() },
            { "_ntes_nuid", randomBytes().toHex() }
        });
        if(!cookie["MUSIC_U"].isValid()) {
            // 游客
            if(!cookie["MUSIC_A"].isValid()) {
                //options.cookie.MUSIC_A = config.anonymous_token
                cookie["MUSIC_A"] = Config::anonymous_token;
            }
        }
        QList<QNetworkCookie> cookieList;
        for(QMap<QString, QVariant>::iterator i = cookie.begin(); i != cookie.end(); ++i) {
            if(!i.value().isValid()) continue;
            cookieList.append(QNetworkCookie(i.key().toUtf8(), i.value().toByteArray()));
        }
        request.setHeader(QNetworkRequest::CookieHeader, QVariant::fromValue(cookieList));
    }
    else if(options["cookie"].isValid()) {
        request.setHeader(QNetworkRequest::CookieHeader, options["cookie"]);
    }
    else {
        request.setHeader(QNetworkRequest::CookieHeader, QVariant::fromValue(QList<QNetworkCookie>({
                                                             QNetworkCookie("__remember_me", QVariant(true).toByteArray()),
                                                             QNetworkCookie("NMTID", "xxx")
                                                         })));
    }

    if(options["crypto"].toString() == "weapi") {
        QString csrfToken = "";
        auto cookieList = request.header(QNetworkRequest::CookieHeader).value<QList<QNetworkCookie>>();
        for(auto i: cookieList) {
            if(i.name() == "__csrf") {
                csrfToken = i.value();
                break;
            }
        }
        data["csrf_token"] = csrfToken;

        data = Crypto::weapi(QJsonDocument::fromVariant(data));
        url.setPath(url.path().replace(QRegularExpression("\\w*api"), "weapi"));
    }
    else if(options["crypto"].toString() == "linuxapi") {
        data = Crypto::linuxapi(QJsonDocument::fromVariant(QVariantMap({
            { "method", method },
            { "url", url.path().replace(QRegularExpression("\\w*api"), "api") },
            { "params", data }
        })));
    }
    else if(options["crypto"].toString() == "eapi") {
        const QVariantMap cookie = options["cookie"].isValid() ? options["cookie"].toMap() : QVariantMap();
        const QString csrfToken = cookie["__csrf"].isValid() ? cookie["__csrf"].toString() : "";
        QVariantMap header =
            {
             //系统版本
             { "osver", cookie["osver"] },
             { "deviceId", cookie["deviceId"] },
             // app版本
             { "appver", cookie["appver"] },
             //版本号
             { "versioncode", cookie["versioncode"].isValid() ? cookie["versioncode"]
                                                             : "" },
             //设备model
             { "mobilename", cookie["mobilename"] },
             { "buildver", cookie["buildver"].isValid() ? cookie["buildver"]
                                                       : QString::number(QDateTime::currentDateTime().toMSecsSinceEpoch()).mid(0, 10) },
             //设备分辨率
             { "resolution", cookie["resolution"].isValid() ? cookie["resolution"]
                                                           : "1920x1080" },
             { "__csrf", csrfToken },
             { "os", cookie["os"].isValid() ? cookie["os"]
                                           : "android" },
             { "channel", cookie["channel"] },
             { "requestId", QString::number(QDateTime::currentDateTime().toMSecsSinceEpoch())
                                  + "_"
                                  + QString::number((int)(QRandomGenerator::global()->bounded(1.0) * 1000)).rightJustified(4, '0')
             },
             };
        if(cookie["MUSIC_U"].isValid()) header["MUSIC_U"] = cookie["MUSIC_U"];
        if(cookie["MUSIC_A"].isValid()) header["MUSIC_A"] = cookie["MUSIC_A"];

        auto getCookies = [](QVariantMap header) {
            QList<QNetworkCookie> l;
            for(QMap<QString, QVariant>::iterator i = header.begin(); i != header.end(); ++i) {
                l.append(QNetworkCookie(i.key().toUtf8(), i.value().toByteArray()));
            }
            return l;
        };
        request.setHeader(QNetworkRequest::CookieHeader, QVariant::fromValue(getCookies(header)));
        data["header"] = QVariant::fromValue(header);
        data = Crypto::eapi(options["url"].toString(), QJsonDocument::fromVariant(data));
        url.setPath(url.path().replace(QRegularExpression("\\w*api"), "eapi"));
    }
//    QVariantMap answer = {
//        { "status", 500 },
//        { "body", {} },
//        { "cookie", {} }
//    };
//    QVariantMap settings = {
//        { "method", method },
//        { "url", url },
//        { "headers", headers }
//    };
    // 创建一个QNetworkAccessManager对象，用来管理HTTP请求和响应
    QNetworkAccessManager manager;
//    if (options.contains("proxy")) {
//        if (options["proxy"].toString().contains("pac")) {
//            QNetworkProxyFactory::setUseSystemConfiguration(true);
//            QNetworkProxyQuery query(url);
//            QList<QNetworkProxy> proxies = QNetworkProxyFactory::systemProxyForQuery(query);
//            if (!proxies.isEmpty()) {
//                manager.setProxy(proxies.first());
//            }
//        } else {
//            QUrl purl(options["proxy"].toString());
//            if (!purl.host().isEmpty()) {
//                QNetworkProxy proxy;
//                proxy.setType(QNetworkProxy::HttpProxy);
//                proxy.setHostName(purl.host());
//                proxy.setPort(purl.port(80));
//                manager.setProxy(proxy);
//            } else {
//                qDebug() << "代理配置无效，不使用代理";
//            }
//        }
//    }

    qDebug() << "headers" ;
    for(auto i : request.rawHeaderList()) {
        qDebug() << "header:" << i << "value:" << request.rawHeader(i);
    }

    // 发送HTTP请求，并返回一个QNetworkReply对象
    auto getResult = [](QNetworkReply* reply) {
        QByteArray result; // 定义一个空字节数组作为结果
        result.clear();
        // 设置超时处理定时器
        QTimer timer;
        timer.setInterval(10000);  // 设置超时时间 10 秒
        timer.setSingleShot(true);  // 单次触发

        // 开启一个局部的事件循环，等待响应结束，退出
        QEventLoop eventLoop;
        QObject::connect(&timer, &QTimer::timeout, &eventLoop, &QEventLoop::quit); // 定时器超时时退出事件循环
        QObject::connect(reply->manager(), &QNetworkAccessManager::finished, &eventLoop, &QEventLoop::quit); // 请求结束时退出事件循环
        timer.start(); // 启动定时器
        eventLoop.exec(); // 启动事件循环

        if(timer.isActive()) { // 处理响应，定时器激活状态
            timer.stop(); // 停止定时器
            if(reply->error() != QNetworkReply::NoError) { // http请求出错，进行错误处理
                qDebug() << "http请求出错 : " << reply->errorString();
                                                         reply->deleteLater();
            }
            else {
                // http - 响应状态码
                int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
                qDebug() << "服务器返回的Code : " << statusCode;
                    if(statusCode == 200) { // http请求响应正常
                    // 读取响应内容
                    result = reply->readAll();
                    auto doc = QJsonDocument::fromJson(result);
                    if(!doc.isNull()) {
                        return doc.toJson (QJsonDocument::Indented);
                    }
                    else {
                        return result;
                    }
                }
                else {
                    reply->deleteLater();
                }
            }
        }
        else { // 超时处理
            QObject::disconnect(reply->manager(), &QNetworkAccessManager::finished, &eventLoop, &QEventLoop::quit); // 断开连接
            reply->abort(); // 中止请求
            qDebug() << "http请求超时 ";
        }
        reply->deleteLater();
        return result;
    };
    qDebug() << url;
    request.setUrl(url);
    if (method == QNetworkAccessManager::PostOperation) {
        QUrlQuery urlQuery;
        for(QMap<QString, QVariant>::iterator i = data.begin(); i != data.end(); ++i) {
            urlQuery.addQueryItem(i.key(), i.value().toString());
        }
        qDebug() << urlQuery.toString().toUtf8();
        QNetworkReply* reply = manager.post(request, urlQuery.toString().toUtf8());
        return getResult(reply);
    } else {
        QNetworkReply* reply = manager.get(request);
        return getResult(reply);
    }
}
