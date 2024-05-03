#include <QCoreApplication>
#include <QSslSocket>
#include <QJsonDocument>
#include <QMetaMethod>
#include <QMetaObject>
#include <QDir>

#include <cstring>
#include <iostream>

#include "api_c.h"
#include "../QCloudMusicApi/apihelper.h"

void init() {
    /*
     * Python调用时，QCoreApplication::libraryPaths只返回python可执行程序的路径，因此需要手动添加API动态库的路径；
     * Qt网络库会在路径下查找插件，用于实现ssl加解密
     */
    auto currentPath = QDir::currentPath();
    if (!QCoreApplication::libraryPaths().contains(currentPath)) {
        QCoreApplication::addLibraryPath(currentPath);
    }

    QCoreApplication *a = Q_NULLPTR;
    if (!QCoreApplication::instance()) {
        int argc = 1;
        char* argv[1];
        a = new QCoreApplication(argc, argv);
        // a->deleteLater();
    }
}

QCLOUDMUSICAPI_EXPORT const char* invoke(char* memberName, char* value) {
    init();

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
