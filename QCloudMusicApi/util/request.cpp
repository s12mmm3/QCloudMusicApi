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
#include <QRandomGenerator>
#include <QNetworkCookie>
#include <QNetworkProxyFactory>
#include <QNetworkReply>
#include <QUrlQuery>
#include <QRegularExpression>

#include "crypto.h"
#include "config.h"
#include "index.h"
#include "request.h"
#include "logger.h"

using namespace QCloudMusicApi;
QString Request::chooseUserAgent(QString ua) {
    const QMap<QString, QString> userAgentList {
        {
            QStringLiteral("mobile"),
            QStringLiteral("Mozilla/5.0 (iPhone; CPU iPhone OS 17_2_1 like Mac OS X) AppleWebKit/605.1.15 (KHTML, like Gecko) Version/17.2 Mobile/15E148 Safari/604.1")
        },
        {
            QStringLiteral("pc"),
            QStringLiteral("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36 Edg/120.0.0.0")
        }
    };
    if(ua == "mobile") {
        return userAgentList["mobile"];
    }
    return userAgentList["pc"];
}

QVariantMap Request::createRequest(QNetworkAccessManager::Operation method,
                          QString url,
                          QVariantMap data,
                          QVariantMap options) {
    DEBUG.noquote() <<
        QJsonDocument::fromVariant(
            QVariantMap {
                { "method", method },
                { "url", url },
                { "data", data },
                { "options", options}
            }
            ).toJson();
    QVariantMap headers {
        { "User-Agent", options.value("ua", chooseUserAgent(options["uaType"].toString())) }
    };
    headers = Index::mergeMap(headers, options["headers"].toMap());

    if(method == QNetworkAccessManager::PostOperation) {
        headers["Content-Type"] = "application/x-www-form-urlencoded";
    }
    if(url.contains("music.163.com")) {
        headers["Referer"] = "https://music.163.com";
    }
    QString ip = options.value("realIP", options.value("ip", "").toString()).toString();

    if(ip.length() > 0) {
        headers["X-Real-IP"] = ip;
        headers["X-Forwarded-For"] = ip;
    }
    //    headers["X-Real-IP"] = "118.88.88.88";
    auto randomBytes = []() {
        QByteArray bytes;
        for(int i = 0; i < 16; ++i) {
            bytes.append(QRandomGenerator::global()->generate());
        }
        return bytes;
    };
    if(options["cookie"].userType() == QMetaType::QVariantMap) {
        auto cookie = options["cookie"].toMap();
        cookie.insert("__remember_me", true);
        //        cookie.insert("NMTID", randomBytes().toHex());
        cookie.insert("_ntes_nuid", randomBytes().toHex());
        if (url.indexOf("login") == -1) {
            cookie["NMTID"] = randomBytes().toHex();
        }
        if(!cookie.contains("MUSIC_U")) {
            // 游客
            if(!cookie.contains("MUSIC_A")) {
                //options.cookie.MUSIC_A = config.anonymous_token
                cookie["MUSIC_A"] = Config::anonymous_token;
                cookie["os"] = cookie.value("os", "ios");
                cookie["appver"] = cookie.value("appver", "8.10.90");
            }
        }
        options["cookie"] = cookie;
        headers["Cookie"] = Index::mapToString(cookie);
    }
    else if(options.contains("cookie")) {
        headers["Cookie"] = options["cookie"];
    }
    else {
        headers["Cookie"] = "__remember_me=true; NMTID=xxx";
    }

    if(options["crypto"].toString() == "weapi") {
        headers["User-Agent"] =
            "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/116.0.0.0 Safari/537.36 Edg/116.0.1938.69";
        QRegularExpression csrfTokenRegex("_csrf=([^;]+)");
        QRegularExpressionMatch match = csrfTokenRegex.match(headers.value("Cookie", "").toString());
        data["csrf_token"] = match.hasMatch() ? match.captured(1) : "";

        data = Crypto::weapi(QJsonDocument::fromVariant(data));
        url = url.replace(QRegularExpression("\\w*api"), "weapi");
    }
    else if(options["crypto"].toString() == "linuxapi") {
        data = Crypto::linuxapi(QJsonDocument::fromVariant(QVariantMap {
            { "method", method },
            { "url", url.replace(QRegularExpression("\\w*api"), "api") },
            { "params", data }
        }));
        headers["User-Agent"] ="Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/60.0.3112.90 Safari/537.36";
        url = "https://music.163.com/api/linux/forward";
    }
    else if(options["crypto"].toString() == "eapi") {
        const QVariantMap cookie = options["cookie"].userType() == QMetaType::QVariantMap
                                       ? options["cookie"].toMap()
                                       : Index::stringToMap(options.value("cookie", "").toString());
        const QString csrfToken = cookie.value("__csrf", "").toString();
        QVariantMap header {
                           { "osver", cookie.value("osver", "17.1.2") }, //系统版本
                           { "deviceId", cookie["deviceId"] },
                           { "appver", cookie.value("appver", "9.0.65") }, // app版本
                           { "versioncode", cookie.value("versioncode", "140") }, //版本号
                           { "mobilename", cookie.value("mobilename", "") }, //设备model
                           { "buildver", cookie.value("buildver", QString::number(QDateTime::currentDateTime().toMSecsSinceEpoch()).mid(0, 10)) },
                           { "resolution", cookie.value("resolution", "1920x1080") }, //设备分辨率
                           { "__csrf", csrfToken },
                           { "os", cookie.value("os", "ios") },
                           { "channel", cookie["channel"] },
                           { "requestId", QString::number(QDateTime::currentDateTime().toMSecsSinceEpoch())
                                                + "_"
                                                + QString::number((int)(QRandomGenerator::global()->bounded(1.0) * 1000)).rightJustified(4, '0')
                           },
                           };
        if(cookie.contains("MUSIC_U")) header["MUSIC_U"] = cookie["MUSIC_U"];
        if(cookie.contains("MUSIC_A")) header["MUSIC_A"] = cookie["MUSIC_A"];

        headers["Cookie"] = Index::mapToString(header);
        data["header"] = QVariant::fromValue(header);
        data = Crypto::eapi(options["url"].toString(), QJsonDocument::fromVariant(data));
        url = url.replace(QRegularExpression("\\w*api"), "eapi");
    }

    QNetworkProxy proxy = QNetworkProxy::NoProxy;
    if (options.contains("proxy") && !options["proxy"].isNull()) {
        QUrl purl(options["proxy"].toString());
        if (!purl.host().isEmpty()) {
            proxy.setType(QNetworkProxy::HttpProxy);
            proxy.setHostName(purl.host());
            proxy.setPort(purl.port(80));

        } else {
            proxy = QNetworkProxy::NoProxy;
            DEBUG << "代理配置无效，不使用代理";
        }
    }

    DEBUG.noquote() << QJsonDocument::fromVariant(headers).toJson();

    QUrlQuery query;
    query.setQuery(QUrl(url).query());
    for(auto i = data.constBegin(); i != data.constEnd(); ++i) {
        query.addQueryItem(i.key(), i.value().toString());
    }
    QNetworkReply* reply = axios(method, url, data, headers, query.toString().toUtf8(), proxy);
    reply->deleteLater();
    reply->manager()->deleteLater();

    QVariantMap answer {
        { "status", 500 },
        { "body", {} },
        { "cookie", {} }
    };

    if(reply->error() != QNetworkReply::NoError) { // http请求出错，进行错误处理
        answer["body"] = QVariantMap {
            { "code", 502 },
            { "msg", reply->errorString() }
        };
        answer["status"] = 502;
    }
    else {
        // 打印响应头
        DEBUG.noquote() << reply->rawHeaderPairs();

        // 读取响应内容
        auto body = reply->readAll();
        DEBUG.noquote() << "body" << body;

        auto cookie = QString(reply->rawHeader("set-cookie"));
        cookie.replace(QRegularExpression("\\s*Domain=[^;]*"), "");
        answer["cookie"] = cookie;

        if(options["crypto"].toString() == "eapi") {
            answer["body"] = QJsonDocument::fromJson(
                                 Crypto::aesDecrypt(body, "ecb", Crypto::eapiKey.toUtf8().data(), "")
                                 ).toVariant().toMap();
            if(answer["body"].toMap().isEmpty()) {
                answer["body"] = QJsonDocument::fromJson(body).toVariant().toMap();
            }
        }
        else {
            if(!QJsonDocument::fromJson(body).isNull()) answer["body"] = QJsonDocument::fromJson(body).toVariant().toMap();
            else answer["body"] = QString::fromUtf8(body);
        }
        answer["status"] = answer["body"].toMap().value("code", reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt());
        if(QList<int> { 201, 302, 400, 502, 800, 801, 802, 803 }.indexOf(answer["body"].toMap()["code"].toInt()) > -1) {
            answer["status"] = 200;
        }

        answer["status"] = 100 < answer["status"].toInt() && answer["status"].toInt() < 600
                               ? answer["status"]
                               : 400;
    }

    return answer;
}

QNetworkReply *Request::axios(QNetworkAccessManager::Operation method,
                              QString url,
                              const QVariantMap &urlQuery,
                              const QVariantMap &headers,
                              const QByteArray &data,
                              QNetworkProxy proxy)
{
    QNetworkRequest request;
    for (auto i = headers.constBegin(); i != headers.constEnd(); i++) {
        request.setRawHeader(i.key().toUtf8(), i.value().toByteArray());
    }
    // 创建一个QNetworkAccessManager对象，用来管理HTTP请求和响应
    QNetworkAccessManager* manager = new QNetworkAccessManager();
    manager->setProxy(proxy);

    QUrlQuery query;
    QUrl qurl(url);
    query.setQuery(qurl.query());
    for(auto i = urlQuery.constBegin(); i != urlQuery.constEnd(); ++i) {
        query.addQueryItem(i.key(), i.value().toString());
    }
    qurl.setQuery(query);
    request.setUrl(qurl);

    // 发送HTTP请求
    QNetworkReply* reply;
    if (method == QNetworkAccessManager::PostOperation) {
        reply = manager->post(request, data);
    } else {
        reply = manager->get(request);
    }

    // 开启一个局部的事件循环，等待响应结束，退出
    QEventLoop eventLoop;
    QObject::connect(reply->manager(), &QNetworkAccessManager::finished, &eventLoop, &QEventLoop::quit); // 请求结束时退出事件循环
    eventLoop.exec(); // 启动事件循环

    return reply;
}
