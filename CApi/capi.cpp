#include <QCoreApplication>
#include <QSslSocket>
#include <QJsonDocument>
#include <QMetaMethod>
#include <QMetaObject>

#include "capi.h"
#include "../QCloudMusicApi/module.h"

CAPI_EXPORT char* invoke(char* funName, char* value) {
    QString result;
    int argc = 1;
    char *argv[1];
    QCoreApplication a(argc, argv);
    qDebug() << "QSslSocket" << QSslSocket::sslLibraryBuildVersionString()
             << "OpenSSL支持情况" << QSslSocket::supportsSsl();

                        NeteaseCloudMusicApi api;
    QVariantMap ret;
    bool ok = QMetaObject::invokeMethod(&api, funName
                                        , Qt::DirectConnection
                                        , Q_RETURN_ARG(QVariantMap, ret)
                                        , Q_ARG(QVariantMap, QJsonDocument::fromJson(value).toVariant().toMap()));
    if(ok) {
        result = QJsonDocument::fromVariant(ret["body"].toMap()).toJson();
    }
    else {
        //函数调用错误
    }
    return result.toUtf8().data();
}

CAPI_EXPORT char* getFunName(int i) {
    NeteaseCloudMusicApi api;
    return api.metaObject()->method(QObject().metaObject()->methodCount() + i).name().data();
}

CAPI_EXPORT int getFunCount() {
    return NeteaseCloudMusicApi().metaObject()->methodCount() - QObject().metaObject()->methodCount();
}
