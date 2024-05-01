#include <QCoreApplication>
#include <QHttpServer>
#include <QJsonDocument>
#include <QMetaMethod>
#include <QMetaObject>
#include <QUrlQuery>
#include <QtConcurrent>

#include "../QCloudMusicApi/apihelper.h"
#include "../QCloudMusicApi/util/index.h"

Q_LOGGING_CATEGORY(Logger, "ApiServer")

using namespace QCloudMusicApiNS;
int main(int argc, char *argv[])
{
    qSetMessagePattern("%{time yyyy-MM-dd hh:mm:ss.zzz} : %{pid} : %{category} : %{type} : %{line} : %{function} : %{message}");
    QCoreApplication a(argc, argv);
    QCommandLineParser parser;
    QCommandLineOption portOption(QStringList() << "PORT",
                                  QCoreApplication::translate("main", "Set PORT."),
                                  QCoreApplication::translate("main", "PORT"), "3000");
    parser.addOption(portOption);
    parser.process(a);

    // 端口号
    quint16 port = parser.value(portOption).toUInt();

    QHttpServer server;
    // 设置请求的路径和方法未知时的错误提示
    server.setMissingHandler([] (const QHttpServerRequest &request, QHttpServerResponder &&responder) {
        QHttpServerResponse response(("Cannot GET "
                                      + request.url().path()).toUtf8()
                                     , QHttpServerResponse::StatusCode::NotFound);
        responder.sendResponse(response);
    });
    for (auto& member: ApiHelper().memberList()) {
        auto path = member;
        path = "/" + path.replace("_", "/").trimmed();

        auto ViewHandler = [=](const QHttpServerRequest &request) {
            QVariantMap arg;
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

            // QVariantMap args {
            //     { "path", path },
            //     { "member", member },
            //     { "localAddress", request.localAddress().toString() + ":" + QString::number(request.localPort())  },
            //     { "remoteAddress", request.remoteAddress().toString() + ":" + QString::number(request.remotePort())  },
            //     { "arg", arg },
            //     { "headers", headers }
            // };
            // qDebug().noquote() << QJsonDocument::fromVariant(args).toJson();

            return QtConcurrent::run([=]() {
                ApiHelper helper;
                QVariantMap ret = helper.invoke(member, arg);
                QByteArray result = QJsonDocument::fromVariant(ret["body"].toMap()).toJson();

                auto response = QHttpServerResponse(result, (QHttpServerResponse::StatusCode)ret["status"].toInt());
                const auto cookies = ret["cookie"].toString();
                response.setHeader("Set-Cookie", cookies.toUtf8());
                return response;
            });

        };
        server.route(path, ViewHandler);
    }

    if(port == server.listen(QHostAddress::Any, port)) {
        qCDebug(Logger) << "ApiServer running at port" << port;
    }
    else {
        qCDebug(Logger) << "该端口号已被占用:" << port;
    }
    return a.exec();
}
