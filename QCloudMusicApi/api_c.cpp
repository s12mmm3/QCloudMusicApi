#include <QCoreApplication>
#include <QSslSocket>
#include <QJsonDocument>
#include <QMetaMethod>
#include <QMetaObject>
#include <QDir>
#include <QUrlQuery>

#include <cstring>
#include <iostream>
#include <QMutex>

#include "api_c.h"
#include "../QCloudMusicApi/apihelper.h"

std::string currentPath_c;
QCoreApplication *app = Q_NULLPTR;
std::string result;
ApiHelper helper;
QMap<int, std::string> results; // Global map to store results
int callCounter = 0;
QMutex mutex;
char initialized = 0;

void freeApp() {
    if (app) {
        delete app;
        app = Q_NULLPTR;
    }
}

QCLOUDMUSICAPI_EXPORT void freeApi() {
    freeApp();
}

void init() {
    static QMutex mutex;
    mutex.lock();
    if (initialized) {
        mutex.unlock();
        return;
    }
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
    initialized = 1;
    mutex.unlock();
}

// Store the result in the map and return the key
int storeResult(const std::string &result) {
    mutex.lock();
    int key = ++callCounter;

    // Ensure unique key
    while (results.contains(key)) {
        key = ++callCounter;
    }

    results[key] = result;
    mutex.unlock();
    return key;
}

int invoke_p(QVariantMap ret) {
    std::string result = QString::fromUtf8(QJsonDocument::fromVariant(ret["body"].toMap()).toJson(QJsonDocument::Compact)).toStdString();
    return storeResult(result);
}

QCLOUDMUSICAPI_EXPORT int invoke(char* memberName, char* value) {
    init();

    QVariantMap ret = helper.invoke(memberName, QJsonDocument::fromJson(value).toVariant().toMap());
    return invoke_p(ret);
}

QCLOUDMUSICAPI_EXPORT int invokeUrl(char *url) {
    init();

    QVariantMap ret = helper.invokeUrl(url);
    return invoke_p(ret);
}

QCLOUDMUSICAPI_EXPORT const char *get_result(int key) {
    mutex.lock();
    const char* result = results.contains(key) ? results[key].c_str() : nullptr;
    mutex.unlock();
    return result;
}

QCLOUDMUSICAPI_EXPORT void free_result(int key) {
    mutex.lock();
    results.remove(key);
    mutex.unlock();
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

QCLOUDMUSICAPI_EXPORT void set_cookie(char *cookie) {
    helper.set_cookie(cookie);
}
