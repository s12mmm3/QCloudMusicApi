﻿#include "../QCloudMusicApi/module.h"

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

                QVariantMap args = {
                    { "path", path },
                    { "funName", funName },
                    { "localAddress", request.localAddress().toString() + ":" + QString::number(request.localPort())  },
                    { "remoteAddress", request.remoteAddress().toString() + ":" + QString::number(request.remotePort())  },
                    { "query", query }
                };
                qDebug().noquote() << QJsonDocument::fromVariant(args).toJson();

                return QtConcurrent::run([=]() {
                    QByteArray ret;
                    NeteaseCloudMusicApi api;
                    bool ok = QMetaObject::invokeMethod(&api, funName
                                                        , Qt::DirectConnection
                                                        , Q_RETURN_ARG(QByteArray, ret)
                                                        , Q_ARG(QVariantMap, query));
                    if(ok) {
                        return QHttpServerResponse(ret);
                    }
                    else {
                        return QHttpServerResponse(QString(u8"函数调用错误").toUtf8());
                    }
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