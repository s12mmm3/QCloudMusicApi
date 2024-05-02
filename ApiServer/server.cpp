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

    if(port == server.listen(host, port)) {
        qCDebug(Logger) << "server running @ http://" + (host != QHostAddress::Any ? host.toString() : "localhost") + ":" + QString::number(port);
    }
    else {
        qCDebug(Logger) << "address already in use :::" + QString::number(port);
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
    server.setMissingHandler([] (const QHttpServerRequest &request, QHttpServerResponder &&responder) {
        QHttpServerResponse response(("Cannot GET "
                                      + request.url().path()).toUtf8()
                                     , QHttpServerResponse::StatusCode::NotFound);
        responder.sendResponse(response);
    });

    QVariantMap special {
        { "daily_signin", "/daily_signin" },
        { "fm_trash", "/fm_trash" },
        { "personal_fm", "/personal_fm" },
        };
    /**
   * Load every modules in this directory
   */
    auto moduleDefinitions = getModulesDefinitions(special);

    for (auto& moduleDef: moduleDefinitions) {
        QString identifier = moduleDef.toMap()["identifier"].toString();
        auto route = moduleDef.toMap()["route"].toString();

        auto ViewHandler = [=](const QHttpServerRequest &request) {
            QVariantMap arg;
            // 参数注入客户端IP
            auto ip = request.remoteAddress().toString();
            if (ip.size() >= 8) {
                if (ip.sliced(0, 7) == "::ffff:") {
                    ip = ip.sliced(0, 7);
                }
            }
            arg.insert("realIP", ip);

            qCDebug(Logger) << "[OK]" << request.url().path();

            QUrlQuery urlQuery;
            if (request.method() == QHttpServerRequest::Method::Post) {
                auto body = request.body();
                urlQuery.setQuery(body);
            }
            else {
                urlQuery = request.query();
            }
            for (auto& i : urlQuery.queryItems()) {
                arg[i.first] = i.second;
            }

            QVariantMap headers;
            for(auto& i: request.headers()) {
                headers[i.first] = i.second;
            }
            auto cookie = Index::stringToMap(headers["Cookie"].toString());
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
                response.addHeaders({
                    { "Access-Control-Allow-Credentials", "true" },
                    { "Access-Control-Allow-Origin", (!CORS_ALLOW_ORIGIN.isEmpty() ? "*" : CORS_ALLOW_ORIGIN).toUtf8() },
                    { "Access-Control-Allow-Headers", "X-Requested-With,Content-Type" },
                    { "Access-Control-Allow-Methods", "PUT,POST,GET,DELETE,OPTIONS" },
                    { "Content-Type", "application/json; charset=utf-8" },
                });
                const auto cookies = ret["cookie"].toString();
                response.setHeader("Set-Cookie", cookies.toUtf8());
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
    for (auto& identifier: ApiHelper().memberList()) {
        auto route = parseRoute(identifier);
        modules.push_back(QVariantMap
                          {
                              { "identifier", identifier },
                              { "route", route }
                          });
    }
    return modules;
}
