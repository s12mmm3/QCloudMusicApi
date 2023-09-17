#include "../QCloudMusicApi/module.h"

#include <QCoreApplication>
#include <QHttpServer>
#include <QJsonDocument>
#include <QMetaMethod>
#include <QMetaObject>
#include <QUrlQuery>
#include <QtConcurrent>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    NeteaseCloudMusicApi api;

    QHttpServer server;
    //设置请求的路径和方法未知时的错误提示
    server.setMissingHandler([] (const QHttpServerRequest &request, QHttpServerResponder &&responder) {
        QHttpServerResponse response(("Cannot GET "
                                      + request.url().path()).toUtf8()
                                     , QHttpServerResponse::StatusCode::NotFound);
        responder.sendResponse(response);
    });

    auto parseRoute = [](QString funName) {
        QString path = funName.replace("_", "/").trimmed();
        return path;
    };
    for(int i = QObject().metaObject()->methodCount(); i < api.metaObject()->methodCount(); i++) {
        auto funName = api.metaObject()->method(i).name();
        auto path = "/" + parseRoute(funName);
        server.route(
            path,
            QHttpServerRequest::Method::Get,
            [=](const QHttpServerRequest &request) {
                QVariantMap query;

                for(auto i: request.query().queryItems()) {
                    query[i.first] = i.second;
                }

                QVariantMap headers;
                for(auto i: request.headers()) {
                    headers[i.first] = i.second;
                }
                QVariantMap args = {
                    { "path", path },
                    { "funName", funName },
                    { "localAddress", request.localAddress().toString() + ":" + QString::number(request.localPort())  },
                    { "remoteAddress", request.remoteAddress().toString() + ":" + QString::number(request.remotePort())  },
                    { "query", query },
                    { "headers", headers }
                };
                qDebug().noquote() << QJsonDocument::fromVariant(args).toJson();

                return QtConcurrent::run([=]() {
                    NeteaseCloudMusicApi api;
                    QByteArray ret;
                    bool ok = QMetaObject::invokeMethod(&api, funName
                                                        , Qt::DirectConnection
                                                        , Q_RETURN_ARG(QByteArray, ret)
                                                        , Q_ARG(QVariantMap, query));
                    if(!ok) {
                        ret = QString(u8"函数调用错误").toUtf8();
                    }
                    return QHttpServerResponse(ret);
                });

            });
    }

    quint16 port = 3000;
    if(port == server.listen(QHostAddress::Any, port)) {
        qDebug() << "ApiServer running at port" << port;
    }
    else {
        qDebug() << "该端口号已被占用:" << port;
    }
    return a.exec();
}
