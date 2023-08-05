#include <iostream>
#include <QObject>
#include <QEventLoop>
#include <QJsonDocument>
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

QString chooseUserAgent(QString ua = "") {
    const QList<QString> mobile = {
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
    };

    const QList<QString> pc = {
        // macOS 10.15.6  Firefox / Chrome / Safari
        R"(Mozilla/5.0 (Macintosh; Intel Mac OS X 10.15; rv:80.0) Gecko/20100101 Firefox/80.0)",
        R"(Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_6) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/86.0.4240.30 Safari/537.36)",
        R"(Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_6) AppleWebKit/605.1.15 (KHTML, like Gecko) Version/13.1.2 Safari/605.1.15)",
        // Windows 10 Firefox / Chrome / Edge
        R"(Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:80.0) Gecko/20100101 Firefox/80.0)",
        R"(Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/86.0.4240.30 Safari/537.36)",
        R"(Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/42.0.2311.135 Safari/537.36 Edge/13.10586)"
        // Linux 就算了
    };

    const QMap<QString, QList<QString>> userAgentList = {
        { "mobile", mobile },
        { "pc", pc }
    };

    auto list = userAgentList["mobile"].toList();
    list.append(userAgentList["pc"].toList());

    auto realUserAgentList = userAgentList.contains(ua) ? userAgentList[ua].toList() : list;
    return QList({"mobile", "pc", ""}).indexOf(ua) > -1 ? realUserAgentList[QRandomGenerator::global()->generate() % realUserAgentList.size()]
                                                        : ua;
}
/**
 * @brief 发送一个HTTP GET请求，并返回响应内容
 * @param request 要发送的请求
 * @return 如果请求成功，返回响应内容；如果请求失败或超时，返回空字节数组
 */
QByteArray get(QNetworkRequest request) {
    QByteArray result; // 定义一个空字节数组作为结果
    result.clear();
    // 设置超时处理定时器
    QTimer timer;
    timer.setInterval(10000);  // 设置超时时间 10 秒
    timer.setSingleShot(true);  // 单次触发

    // 设置网络访问管理器
    QNetworkAccessManager* manager = new QNetworkAccessManager();

    // 开启一个局部的事件循环，等待响应结束，退出
    QNetworkReply* reply = manager->get(request); // 发送GET请求
    QEventLoop eventLoop;
    QObject::connect(&timer, &QTimer::timeout, &eventLoop, &QEventLoop::quit); // 定时器超时时退出事件循环
    QObject::connect(manager, &QNetworkAccessManager::finished, &eventLoop, &QEventLoop::quit); // 请求结束时退出事件循环
    timer.start(); // 启动定时器
    eventLoop.exec(); // 启动事件循环

    if(timer.isActive()) { // 处理响应，定时器激活状态
        timer.stop(); // 停止定时器
        if(reply->error() != QNetworkReply::NoError) { // http请求出错，进行错误处理
            qDebug() << "http请求出错 : " << reply->errorString();
                                                     reply->deleteLater();
            return result;
        }
        else {
            // http - 响应状态码
            int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            qDebug() << "服务器返回的Code : " << statusCode;
                if(statusCode == 200) { // http请求响应正常
                result = reply->readAll(); // 读取响应内容
                return result;
            }
            else {
                reply->deleteLater();
                return result;
            }
        }
    }
    else { // 超时处理
        QObject::disconnect(manager, &QNetworkAccessManager::finished, &eventLoop, &QEventLoop::quit); // 断开连接
        reply->abort(); // 中止请求
        qDebug() << "http请求超时 ";
        return result;
    }
    reply->deleteLater();
    qDebug() << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toString();
    return result;
}

/**
 * @brief 发送一个HTTP POST请求，并返回响应内容
 * @param request 要发送的网络请求
 * @param data 要上传的数据
 * @return 如果请求成功，返回响应内容；如果请求失败或超时，返回空字节数组
 */
QByteArray post(QNetworkRequest request, QString data)
{
    QByteArray emptyResult; // 定义一个空字节数组，用于返回错误或超时的结果
    emptyResult.clear();
    // 设置超时处理定时器
    QTimer timer;
    timer.setInterval(10000);  // 设置超时时间 10 秒
    timer.setSingleShot(true);  // 单次触发

    QByteArray postData; // 定义一个字节数组，用于存储要上传的JSON数据
    postData.append(data.toUtf8());

    // 创建一个网络访问管理器，用于发送和接收网络请求
    QNetworkAccessManager* manager = new QNetworkAccessManager();

    // 开启一个局部的事件循环，等待响应结束，退出
    qDebug()<<"Send: "<<postData;
    QNetworkReply* reply = manager->post(request, postData); // 发送POST请求，并获取响应对象
    QEventLoop eventLoop;
    QObject::connect(&timer, &QTimer::timeout, &eventLoop, &QEventLoop::quit); // 连接定时器的超时信号和事件循环的退出槽函数
    QObject::connect(manager, &QNetworkAccessManager::finished, &eventLoop, &QEventLoop::quit); // 连接网络管理器的完成信号和事件循环的退出槽函数
    timer.start(); // 启动定时器
    eventLoop.exec(); // 启动事件循环

    if(timer.isActive()) { // 处理响应，定时器激活状态表示没有超时
        timer.stop(); // 停止定时器
        if(reply->error() != QNetworkReply::NoError) { // HTTP请求出错，进行错误处理
            qDebug() << "HTTP请求出错 : " << reply->errorString();
                                                     reply->deleteLater(); // 删除响应对象
            return emptyResult; // 返回空结果
        }
        else {
            // HTTP - 响应状态码
            int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            qDebug()<<"服务器返回的Code : "<<statusCode;
                if(statusCode == 200) { // HTTP请求响应正常
                QByteArray replyContent = reply->readAll(); // 读取响应内容
                return replyContent; // 返回响应内容
            }
            else {
                reply->deleteLater(); // 删除响应对象
                return emptyResult; // 返回空结果
            }
        }
    }
    else { // 超时处理
        QObject::disconnect(manager, &QNetworkAccessManager::finished, &eventLoop, &QEventLoop::quit); // 断开网络管理器的完成信号和事件循环的退出槽函数的连接
        reply->abort(); // 中止响应对象
        qDebug() << "HTTP请求超时 ";
        return emptyResult; // 返回空结果
    }
    reply->deleteLater(); // 删除响应对象
    qDebug() << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toString();
    return emptyResult; // 返回空结果
}

QByteArray request() {
    QNetworkRequest request; // 创建一个网络请求对象
    QUrl qUrl("http://coolaf.com/tool/params?r=rtest&t2=rtest2"); // 创建一个URL对象
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json;charset=UTF-8"); // 设置请求头中的内容类型字段
    request.setUrl(qUrl); // 设置请求的URL
    qDebug().noquote() << post(request, qUrl.toString());
    return QByteArray();
}

auto createRequest(QNetworkAccessManager::Operation method, QUrl url, QVariantMap data, QVariantMap options) {
    qDebug().noquote() << "data:" << data;
    qDebug().noquote() << "options:" << options;
    QNetworkRequest request(url);

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
    if(options["cookie"].isValid()) {
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
            { "NMTID", randomBytes().toHex() },
            { "_ntes_nuid", randomBytes().toHex() }
        });
        if(!cookie["MUSIC_U"].isValid()) {
            // 游客
            if(!cookie["MUSIC_A"].isValid()) {
                //options.cookie.MUSIC_A = config.anonymous_token
                cookie["MUSIC_A"] = "bf8bfeabb1aa84f9c8c3906c04a04fb864322804c83f5d607e91a04eae463c9436bd1a17ec353cf780b396507a3f7464e8a60f4bbc019437993166e004087dd32d1490298caf655c2353e58daa0bc13cc7d5c198250968580b12c1b8817e3f5c807e650dd04abd3fb8130b7ae43fcc5b";
            }
        }
        QList<QNetworkCookie> cookieList;
        for(QMap<QString, QVariant>::iterator i = cookie.begin(); i != cookie.end(); ++i) {
            cookieList.append(QNetworkCookie(i.key().toUtf8(), i.value().toByteArray()));
        }
        request.setHeader(QNetworkRequest::CookieHeader, QVariant::fromValue(cookieList));
    }
    else {
        request.setHeader(QNetworkRequest::CookieHeader, QVariant::fromValue(QList<QNetworkCookie>({
                                                             QNetworkCookie("__remember_me", QVariant(true).toByteArray()),
                                                             QNetworkCookie("NMTID", "xxx")
                                                         })));
    }
    if(options["crypto"].toString() == "weapi") {

    }
    else if(options["crypto"].toString() == "linuxapi") {

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
//        qDebug().noquote() << data << QJsonDocument::fromVariant(data);
        data = Crypto::eapi(options["url"].toString(), QJsonDocument::fromVariant(data));
        url.setPath(url.path().replace(QRegularExpression("\\w*api"), "eapi"));
    }
    QVariantMap answer = {
        { "status", 500 },
        { "body", {} },
        { "cookie", {} }
    };
//    QVariantMap settings = {
//        { "method", method },
//        { "url", url },
//        { "headers", headers }
//    };
    // 创建一个QNetworkAccessManager对象，用来管理HTTP请求和响应
    QNetworkAccessManager* manager = new QNetworkAccessManager();
    if (options.contains("proxy")) {
        if (options["proxy"].toString().contains("pac")) {
            QNetworkProxyFactory::setUseSystemConfiguration(true);
            QNetworkProxyQuery query(url);
            QList<QNetworkProxy> proxies = QNetworkProxyFactory::systemProxyForQuery(query);
            if (!proxies.isEmpty()) {
                manager->setProxy(proxies.first());
            }
        } else {
            QUrl purl(options["proxy"].toString());
            if (!purl.host().isEmpty()) {
                QNetworkProxy proxy;
                proxy.setType(QNetworkProxy::HttpProxy);
                proxy.setHostName(purl.host());
                proxy.setPort(purl.port(80));
                manager->setProxy(proxy);
            } else {
                qDebug() << "代理配置无效，不使用代理";
            }
        }
    }

    // 发送HTTP请求，并返回一个QNetworkReply对象
    qDebug().noquote() << request.rawHeaderList() << request.header(QNetworkRequest::CookieHeader);
    auto getResult = [](QNetworkAccessManager* manager, QNetworkReply* reply) {
        if(reply->error() != QNetworkReply::NoError) { // http请求出错，进行错误处理
            qDebug() << "http请求出错 : " << reply->errorString();
                                                     reply->deleteLater();
        }
        else {
            // http - 响应状态码
            int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            qDebug() << "服务器返回的Code : " << statusCode;
                if(statusCode == 200) { // http请求响应正常
                qDebug().noquote() << reply->operation() << QString::fromUtf8(QJsonDocument::fromJson(reply->readAll()).toJson (QJsonDocument::Indented)); // 读取响应内容
            }
            else {
                reply->deleteLater();
            }
        }
        manager->deleteLater();
        reply->deleteLater();
    };
    if (method == QNetworkAccessManager::PostOperation) {
        QUrlQuery urlQuery;
        for(QMap<QString, QVariant>::iterator i = data.begin(); i != data.end(); ++i) {
            urlQuery.addQueryItem(i.key(), i.value().toString());
        }
        QNetworkReply* reply = manager->post(request, urlQuery.toString().toUtf8());
        QObject::connect(manager, &QNetworkAccessManager::finished, [&]() {
            getResult(manager, reply);
        });
        return reply;
    } else {
        QNetworkReply* reply = manager->get(request);
        QObject::connect(manager, &QNetworkAccessManager::finished, [&]() {
            getResult(manager, reply);
        });
        return reply;
    }
}
