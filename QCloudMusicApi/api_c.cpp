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

std::string currentPath_c;
QCoreApplication *app = Q_NULLPTR;
char* data = Q_NULLPTR;
ApiHelper helper;

void freeApp() {
    if (app) {
        delete app;
        app = Q_NULLPTR;
    }
}

void freeData() {
    if (data) {
        delete[] data;
        data = Q_NULLPTR;
    }
}

QCLOUDMUSICAPI_EXPORT void freeApi() {
    freeApp();
    freeData();
}

void init() {
    /*
     * Python调用时，QCoreApplication::libraryPaths只返回python可执行程序的路径，因此需要手动添加API动态库的路径；
     * Qt网络库会在路径下查找插件，用于实现ssl加解密
     */
    QString currentPath = QDir::currentPath();
    currentPath_c = currentPath.toStdString();
    if (!QCoreApplication::libraryPaths().contains(currentPath)) {
        QCoreApplication::addLibraryPath(currentPath);
    }

    // 创建一个QCoreApplication单例，用于支持事件循环QEventLoop
    if (!QCoreApplication::instance()) {
        int argc = 1;
        char* argv[1] = { (char*)currentPath_c.c_str() };
        app = new QCoreApplication(argc, argv);
        // a->deleteLater();
    }
}

QCLOUDMUSICAPI_EXPORT const char* invoke(char* memberName, char* value) {
    init();

    QVariantMap ret = helper.invoke(memberName, QJsonDocument::fromJson(value).toVariant().toMap());
    std::string result = QString::fromUtf8(QJsonDocument::fromVariant(ret["body"].toMap()).toJson(QJsonDocument::Compact)).toStdString();

    freeData();
    data = new char[result.size() + 1];
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

QCLOUDMUSICAPI_EXPORT void set_proxy(char* proxy) {
    helper.set_proxy(proxy);
}
