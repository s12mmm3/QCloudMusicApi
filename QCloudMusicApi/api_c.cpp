#include <QCoreApplication>
#include <QSslSocket>
#include <QJsonDocument>
#include <QMetaMethod>
#include <QMetaObject>

#include "api_c.h"
#include "../QCloudMusicApi/apihelper.h"

QCLOUDMUSICAPI_EXPORT char* invoke(char* memberName, char* value) {
    QCoreApplication *a = Q_NULLPTR;
    if (!QCoreApplication::instance()) {
        int argc = 1;
        char* argv[1];
        a = new QCoreApplication(argc, argv);
        a->deleteLater();
    }

    ApiHelper helper;
    QVariantMap ret = helper.invoke(memberName, QJsonDocument::fromJson(value).toVariant().toMap());
    QString result = QJsonDocument::fromVariant(ret["body"].toMap()).toJson();
    auto data = result.toUtf8().data();

    return data;
}

QCLOUDMUSICAPI_EXPORT char* memberName(int i) {
    NeteaseCloudMusicApi api;
    auto data = api.metaObject()->method(QObject().metaObject()->methodCount() + i).name().data();
    return data;
}

QCLOUDMUSICAPI_EXPORT int memberCount() {
    NeteaseCloudMusicApi api;
    return api.metaObject()->methodCount() - QObject().metaObject()->methodCount();
}
