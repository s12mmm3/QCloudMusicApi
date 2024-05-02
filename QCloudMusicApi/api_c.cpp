#include <QCoreApplication>
#include <QSslSocket>
#include <QJsonDocument>
#include <QMetaMethod>
#include <QMetaObject>

#include <cstring>
#include <iostream>

#include "api_c.h"
#include "../QCloudMusicApi/apihelper.h"

QCLOUDMUSICAPI_EXPORT const char* invoke(char* memberName, char* value) {
    QCoreApplication *a = Q_NULLPTR;
    if (!QCoreApplication::instance()) {
        int argc = 1;
        char* argv[1];
        a = new QCoreApplication(argc, argv);
        // a->deleteLater();
    }
    ApiHelper helper;
    QVariantMap ret = helper.invoke(memberName, QJsonDocument::fromJson(value).toVariant().toMap());
    std::string result = QString::fromUtf8(QJsonDocument::fromVariant(ret["body"].toMap()).toJson(QJsonDocument::Compact)).toStdString();

    char* data = new char[result.size() + 1];
    std::strcpy(data, result.c_str());
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
