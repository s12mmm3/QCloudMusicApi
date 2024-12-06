#include <QJsonDocument>
#include <QMetaMethod>
#include <QMetaObject>
#include <QUrlQuery>
#include <QtConcurrent>
#include <QCoreApplication>
#include <QHttpServer>
#include <QJsonDocument>
#include <QMetaMethod>
#include <QMetaObject>
#include <QUrlQuery>
#include <QtConcurrent>
#include <QTcpServer>

#include "../QCloudMusicApi/apihelper.h"
#include "../QCloudMusicApi/util/index.h"

#include "server.h"
#include "logger.h"

using namespace QCloudMusicApi;
Server::Server() {}

void Server::serveNcmApi(QVariantMap options)
{
    QCommandLineParser parser;
    QCommandLineOption portOption(QStringList() << "PORT",
        QCoreApplication::translate("main", "Set PORT."),
        QCoreApplication::translate("main", "PORT"), "3000");
    QCommandLineOption hostOption(QStringList() << "HOST",
        QCoreApplication::translate("main", "Set HOST."),
        QCoreApplication::translate("main", "HOST"));
    parser.addOptions({ portOption, hostOption });
    parser.process(*QCoreApplication::instance());

    // 端口号
    const quint16 port = options.value("port", parser.value(portOption).toUInt()).toUInt();
    const QHostAddress host = options.value("host").isValid() ? QHostAddress(options["host"].toString())
        : !parser.value(hostOption).isEmpty() ? QHostAddress(parser.value(hostOption))
        : QHostAddress::Any;

    consturctServer({});
#if QT_VERSION >= QT_VERSION_CHECK(6, 8, 0)
    auto tcpserver = new QTcpServer();
    if (tcpserver->listen(host, port) && server.bind(tcpserver)) {
#else
    if (port == server.listen(host, port)) {
#endif
        DEBUG << "server running @ http://" + (host != QHostAddress::Any ? host.toString() : "localhost") + ":" + QString::number(port);
    }
    else {
        DEBUG << "address already in use :::" + QString::number(port);
#if QT_VERSION >= QT_VERSION_CHECK(6, 8, 0)
        delete tcpserver;
        return;
#else

#endif
    }
}

void Server::consturctServer(QVariantMap options)
{
    QCommandLineParser parser;
    QCommandLineOption option(QStringList() << "CORS_ALLOW_ORIGIN",
        QCoreApplication::translate("main", "Set CORS_ALLOW_ORIGIN."),
        QCoreApplication::translate("main", "CORS_ALLOW_ORIGIN"));
    parser.addOptions({ option });
    parser.process(*QCoreApplication::instance());

    const QString CORS_ALLOW_ORIGIN = parser.value(option);

    // 设置请求的路径和方法未知时的错误提示

#if QT_VERSION >= QT_VERSION_CHECK(6, 8, 0)
    server.setMissingHandler(this, [](const QHttpServerRequest& request, QHttpServerResponder& responder) {
#else
    server.setMissingHandler([](const QHttpServerRequest& request, QHttpServerResponder&& responder) {
#endif
        QHttpServerResponse response(("Cannot GET "
            + request.url().path()).toUtf8()
            , QHttpServerResponse::StatusCode::NotFound);
        responder.sendResponse(response);
        });

    QVariantMap special{
        { "daily_signin", "/daily_signin" },
        { "fm_trash", "/fm_trash" },
        { "personal_fm", "/personal_fm" },
    };
    /**
   * Load every modules in this directory
   */
    auto moduleDefinitions = getModulesDefinitions(special);

    for (auto& moduleDef : moduleDefinitions) {
        QString identifier = moduleDef.toMap()["identifier"].toString();
        auto route = moduleDef.toMap()["route"].toString();

        auto ViewHandler = [=](const QHttpServerRequest& request) {
            QVariantMap arg;
            // 参数注入客户端IP
            auto ip = request.remoteAddress().toString();
            if (ip.size() >= 8) {
                if (ip.sliced(0, 7) == "::ffff:") {
                    ip = ip.sliced(0, 7);
                }
            }
            arg.insert("realIP", ip);

            auto url = request.url();
            INFO.noquote() << "[OK]" << url.path() + (url.hasQuery() ? "?" + url.query() : "");

            QUrlQuery urlQuery = request.query();
            auto body = request.body();
            if (request.value("Content-Type") == "application/json") {
                auto bodyMap = QJsonDocument::fromJson(body).toVariant().toMap();
                for (auto i = bodyMap.constBegin(); i != bodyMap.constEnd(); i++) {
                    urlQuery.addQueryItem(i.key(), i.value().toString());
                }
            }
            else {
                urlQuery.setQueryItems(urlQuery.queryItems() + QUrlQuery(body).queryItems());
            }

            for (auto& i : urlQuery.queryItems()) {
                arg[i.first] = i.second;
            }

            QVariantMap headers;
#if QT_VERSION >= QT_VERSION_CHECK(6, 8, 0)
            for (auto& i : request.headers().toListOfPairs()) {
#else
            for (auto& i : request.headers()) {
#endif
                headers[QUrl::fromPercentEncoding(i.first)] = QUrl::fromPercentEncoding(i.second);
            }
            auto cookie = Index::cookieToJson(headers["Cookie"].toString());
            if (!cookie.isEmpty()) {
                arg["cookie"] = cookie;
            }

            return QtConcurrent::run([=]() {
                ApiHelper helper;
                QVariantMap ret = helper.invoke(identifier, arg);
                QByteArray result = QJsonDocument::fromVariant(ret["body"].toMap()).toJson();

                auto response = QHttpServerResponse(result, (QHttpServerResponse::StatusCode)ret["status"].toInt());

                /**
                   * CORS & Preflight request
                 */

#if QT_VERSION >= QT_VERSION_CHECK(6, 8, 0)
                const auto cookies = ret["cookie"].toString();
                response.setHeaders(QHttpHeaders::fromListOfPairs({
                     { "Access-Control-Allow-Credentials", "true" },
                     { "Access-Control-Allow-Origin", (!CORS_ALLOW_ORIGIN.isEmpty() ? "*" : CORS_ALLOW_ORIGIN).toUtf8() },
                     { "Access-Control-Allow-Headers", "X-Requested-With,Content-Type" },
                     { "Access-Control-Allow-Methods", "PUT,POST,GET,DELETE,OPTIONS" },
                     { "Content-Type", "application/json; charset=utf-8" },
                     { "Set-Cookie", cookies.toUtf8() },
                }));
#else
                response.addHeaders({
                    { "Access-Control-Allow-Credentials", "true" },
                    { "Access-Control-Allow-Origin", (!CORS_ALLOW_ORIGIN.isEmpty() ? "*" : CORS_ALLOW_ORIGIN).toUtf8() },
                    { "Access-Control-Allow-Headers", "X-Requested-With,Content-Type" },
                    { "Access-Control-Allow-Methods", "PUT,POST,GET,DELETE,OPTIONS" },
                    { "Content-Type", "application/json; charset=utf-8" },
                });
                const auto cookies = ret["cookie"].toString();
                response.setHeader("Set-Cookie", cookies.toUtf8());
#endif
                return response;
                });

            };
        server.route(route, ViewHandler);
    }
}

QVariantList Server::getModulesDefinitions(QVariantMap specificRoute)
{
    auto parseRoute = [&](QString fileName) {
        return specificRoute.contains(fileName)
            ? specificRoute[fileName].toString()
            : "/" + fileName.replace("_", "/").trimmed();
        };
    QVariantList modules;
    for (auto& identifier : ApiHelper().memberList()) {
        auto route = parseRoute(identifier);
        modules.push_back(QVariantMap
            {
                { "identifier", identifier },
                { "route", route }
            });
    }
    return modules;
}
