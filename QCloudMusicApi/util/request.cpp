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
#include <QStandardPaths>
#include <QDir>

#include "crypto.h"
#include "index.h"
#include "request.h"
#include "logger.h"
#include "config.h"

QString anonymous_token;

const QString iosAppVersion = "9.0.65";

using namespace QCloudMusicApi;
QString Request::chooseUserAgent(QString uaType) {
    const QMap<QString, QString> userAgentList{
        {
            QStringLiteral("mobile"),
            QStringLiteral("Mozilla/5.0 (iPhone; CPU iPhone OS 17_2_1 like Mac OS X) AppleWebKit/605.1.15 (KHTML, like Gecko) Version/17.2 Mobile/15E148 Safari/604.1")
        },
        {
            QStringLiteral("pc"),
            QStringLiteral("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36 Edg/120.0.0.0")
        }
    };
    if (uaType == "mobile") {
        return userAgentList["mobile"];
    }
    return userAgentList["pc"];
}

QVariantMap Request::createRequest(
    QString uri,
    QVariantMap data,
    QVariantMap options) {
    QNetworkAccessManager::Operation method = QNetworkAccessManager::PostOperation;
    DEBUG.noquote() <<
        QJsonDocument::fromVariant(
            QVariantMap{
                { "method", method },
                { "uri", uri },
                { "data", data },
                { "options", options}
            }
    ).toJson();
    const QVariantMap cookie = options.value("cookie", QVariantMap()).toMap();
    QVariantMap headers{
        { "Content-Type", "application/x-www-form-urlencoded" },
    };
    options["headers"] = options.value("headers", QVariantMap()).toMap();
    headers = Index::mergeMap(headers, options["headers"].toMap());

    QString ip = options.value("realIP", options.value("ip", "")).toString();

    if (!ip.isEmpty()) {
        headers["X-Real-IP"] = ip;
        headers["X-Forwarded-For"] = ip;
    }
    // headers["X-Real-IP"] = "118.88.88.88";
    auto randomBytes = []() {
        QByteArray bytes;
        for (int i = 0; i < 16; ++i) {
            bytes.append(QRandomGenerator::global()->generate());
        }
        return bytes;
        };
    if (options["cookie"].userType() == QMetaType::QVariantMap) {
        options["cookie"] =
            Index::mergeMap(options["cookie"].toMap(),
                {
                    { "__remember_me", true },
                    { "NMTID", randomBytes().toHex() },
                    { "_ntes_nuid", randomBytes().toHex() },
                });
        if (uri.indexOf("login") == -1) {
            auto cookie = options["cookie"].toMap();
            cookie["NMTID"] = randomBytes().toHex();
            options["cookie"] = cookie;
        }
        if (!options["cookie"].toMap().contains("MUSIC_U")) {
            // 游客
            if (!options["cookie"].toMap().contains("MUSIC_A")) {
                auto cookie = options["cookie"].toMap();
                cookie["MUSIC_A"] = []() -> QString {
                    if (anonymous_token.isEmpty()) {
                        QString tmpPath = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
                        QFile file(QDir(tmpPath).absoluteFilePath("anonymous_token"));
                        file.open(QIODevice::ReadOnly | QIODevice::Text);
                        anonymous_token = file.readAll();
                    }
                    return anonymous_token;
                    }();
                    options["cookie"] = cookie;
            }
        }
        headers["Cookie"] = Index::cookieObjToString(options["cookie"].toMap());
    }
    else if (options.contains("cookie")) {
        // cookie string
        headers["Cookie"] = options["cookie"];
    }
    else {
        auto cookie = Index::cookieToJson("__remember_me=true; NMTID=xxx");
        headers["Cookie"] = Index::cookieObjToString(cookie);
    }

    QString url = "";
    QVariantMap encryptData;
    auto crypto = options["crypto"];
    auto csrfToken = cookie.value("__csrf", "");

    if (crypto == "") {
        // 加密方式为空，以配置文件的加密方式为准
        if (Config::APP_CONF["encrypt"].toBool()) {
            crypto = "eapi";
        }
        else {
            crypto = "api";
        }
    }

    // 根据加密方式加密请求数据；目前任意uri都支持四种加密方式
    if (crypto == "weapi") {
        headers["Referer"] = Config::APP_CONF["domain"];
        headers["User-Agent"] = options.value("ua", chooseUserAgent("pc"));
        data["csrf_token"] = csrfToken;

        encryptData = Crypto::weapi(QJsonDocument::fromVariant(data));
        url = Config::APP_CONF["domain"].toString() + "/weapi/" + uri.mid(5);
    }
    else if (crypto == "linuxapi") {
        encryptData = Crypto::linuxapi(QJsonDocument::fromVariant(QVariantMap{
            { "method", method },
            { "url", Config::APP_CONF["domain"].toString() + uri },
            { "params", data }
            }));
        headers["User-Agent"] = "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/60.0.3112.90 Safari/537.36";
        url = "https://music.163.com/api/linux/forward";
    }
    else if (crypto == "eapi" || crypto == "api") {
        // 两种加密方式，都应生成客户端的cookie
        const QVariantMap cookie = options.value("cookie", QVariantMap()).toMap();
        QVariantMap header{
            { "osver", cookie.value("osver", "17.4.1") }, //系统版本
            { "deviceId", cookie["deviceId"] },
            { "os", cookie.value("os", "ios") },
            { "appver", cookie.value("appver", cookie.value("os") != "pc" ? iosAppVersion : "") }, // app版本
            { "versioncode", cookie.value("versioncode", "140") }, //版本号
            { "mobilename", cookie.value("mobilename", "") }, //设备model
            { "buildver", cookie.value("buildver", QString::number(QDateTime::currentDateTime().toMSecsSinceEpoch()).mid(0, 10)) },
            { "resolution", cookie.value("resolution", "1920x1080") }, //设备分辨率
            { "__csrf", csrfToken },
            { "channel", cookie.value("channel", "") },
            { "requestId", QString::number(QDateTime::currentDateTime().toMSecsSinceEpoch())
                              + "_"
                              + QString::number((int)(QRandomGenerator::global()->bounded(1.0) * 1000)).rightJustified(4, '0')
            },
        };
        if (cookie.contains("MUSIC_U")) header["MUSIC_U"] = cookie["MUSIC_U"];
        if (cookie.contains("MUSIC_A")) header["MUSIC_A"] = cookie["MUSIC_A"];

        headers["Cookie"] = [&header]() -> QString {
            QStringList result;
            for (auto i = header.constBegin(); i != header.constEnd(); i++) {
                result.push_back(
                    QUrl::toPercentEncoding(i.key()) + "=" + QUrl::toPercentEncoding(i.value().toString())
                    );
            }
            return result.join("; ");
        }();
        headers["User-Agent"] = !options.value("ua").toString().isEmpty() ? options.value("ua") : chooseUserAgent(options["uaType"].toString());
        if (crypto == "eapi") {
            // 使用eapi加密
            data["header"] = header;
            data["e_r"] = options.value("e_r").isValid() ? options.value("e_r") : data.value("e_r", Config::APP_CONF.value("encryptResponse")); // 用于加密eapi接口的返回值
            data["e_r"] = Index::toBoolean(data["e_r"]);
            encryptData = Crypto::eapi(uri, QJsonDocument::fromVariant(data));
            url = Config::APP_CONF["apiDomain"].toString() + "/eapi/" + uri.mid(5);
        }
        else if (crypto == "api") {
            // 不使用任何加密
            url = Config::APP_CONF["apiDomain"].toString() + uri;
            encryptData = data;
        }
    }
    else {
        // 未知的加密方式
        DEBUG << "[ERR]" << "Unknown Crypto:" << options["crypto"].toString();
    }

    QNetworkProxy proxy = QNetworkProxy::NoProxy;
    if (options.contains("proxy") && !options["proxy"].isNull()) {
        QUrl purl(options["proxy"].toString());
        if (!purl.host().isEmpty()) {
            proxy.setType(QNetworkProxy::HttpProxy);
            proxy.setHostName(purl.host());
            proxy.setPort(purl.port(80));

        }
        else {
            proxy = QNetworkProxy::DefaultProxy;
            DEBUG << "代理配置无效，不使用代理";
        }
    }

    QUrlQuery query;
    query.setQuery(QUrl(url).query());
    for (auto i = encryptData.constBegin(); i != encryptData.constEnd(); ++i) {
        query.addQueryItem(i.key(), QUrl::toPercentEncoding(i.value().toString()));
    }
    DEBUG << "method" << method;
    DEBUG << "url" << url;
    DEBUG << "data" << encryptData;
    QNetworkReply* reply = axios(method, url, encryptData, headers, query.toString().toUtf8(), proxy);
    reply->manager()->deleteLater();

    QVariantMap answer{
        { "status", 500 },
        { "body", {} },
        { "cookie", {} }
    };

    if (reply->error() != QNetworkReply::NoError) { // http请求出错，进行错误处理
        answer["body"] = QVariantMap{
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

        QString cookie;
        for (auto& i : QString(reply->rawHeader("set-cookie")).split("\n")) {
            cookie.append(i.replace(QRegularExpression("\\s*Domain=[^;]*"), ""));
        }
        answer["cookie"] = cookie;

        if (data["e_r"].toBool()) {
            // eapi接口返回值被加密，需要解密
            answer["body"] = Crypto::eapiResDecrypt(body.toHex().toUpper());
        }
        else {
            answer["body"] = QJsonDocument::fromJson(body).toVariant().toMap();
        }
        if (answer["body"].toMap().isEmpty()) {
            answer["body"] = body;
        }
        answer["status"] = answer["body"].toMap().value("code", reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt());
        if (QList<int> { 201, 302, 400, 502, 800, 801, 802, 803 }.indexOf(answer["body"].toMap()["code"].toInt()) > -1) {
            // 特殊状态码
            answer["status"] = 200;
        }

        answer["status"] = 100 < answer["status"].toInt() && answer["status"].toInt() < 600
            ? answer["status"]
            : 400;
    }
    return answer;
}

QNetworkReply* Request::axios(QNetworkAccessManager::Operation method,
    QString url,
    const QVariantMap& urlQuery,
    const QVariantMap& headers,
    const QByteArray& data,
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
    if (method != QNetworkAccessManager::PostOperation) {
        for (auto i = urlQuery.constBegin(); i != urlQuery.constEnd(); ++i) {
            query.addQueryItem(i.key(), i.value().toString());
        }
    }
    qurl.setQuery(query);
    request.setUrl(qurl);

    // 发送HTTP请求
    QNetworkReply* reply;
    if (method == QNetworkAccessManager::PostOperation) {
        reply = manager->post(request, data);
    }
    else {
        reply = manager->get(request);
    }

    // 开启一个局部的事件循环，等待响应结束，退出
    QEventLoop eventLoop;
    QObject::connect(reply->manager(), &QNetworkAccessManager::finished, &eventLoop, &QEventLoop::quit); // 请求结束时退出事件循环
    eventLoop.exec(); // 启动事件循环

    return reply;
}
