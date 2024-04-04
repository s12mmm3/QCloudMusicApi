#include <QCoreApplication>
#include <QSslSocket>
#include <QJsonDocument>
#include <QMetaMethod>
#include <QMetaObject>

#include "capi.h"
#include "../QCloudMusicApi/apihelper.h"

CAPI_EXPORT char* invoke(char* funName, char* value) {
    int argc = 1;
    char *argv[1];
    QCoreApplication a(argc, argv);

    ApiHelper helper;
    QVariantMap ret = helper.invoke(funName, QJsonDocument::fromJson(value).toVariant().toMap());
    QString result = QJsonDocument::fromVariant(ret["body"].toMap()).toJson();
    auto data = result.toUtf8().data();
    return data;
}

CAPI_EXPORT char* getFunName(int i) {
    NeteaseCloudMusicApi api;
    auto data = api.metaObject()->method(QObject().metaObject()->methodCount() + i).name().data();
    return data;
}

CAPI_EXPORT int getFunCount() {
    NeteaseCloudMusicApi api;
    return api.metaObject()->methodCount() - QObject().metaObject()->methodCount();
}
