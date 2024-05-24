#include <QCoreApplication>
#include <QSslSocket>
#include <QJsonDocument>
#include <QMetaMethod>
#include <QMetaObject>
#include <QDir>
#include <QUrlQuery>
#include <QMutex>

#include "api_c.h"
#include "../QCloudMusicApi/apihelper.h"

QCoreApplication *app = Q_NULLPTR;
thread_local std::string result;
ApiHelper helper;
QMutex mutex;
bool inited = false;

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
    QMutexLocker locker(&mutex);
    if (inited) {
        return;
    }
    /*
     * Python调用时，QCoreApplication::libraryPaths只返回python可执行程序的路径，因此需要手动添加API动态库的路径；
     * Qt网络库会在路径下查找插件，用于实现ssl加解密
     */
    auto currentPath = QDir::currentPath().toStdString();
    if (!QCoreApplication::libraryPaths().contains(QString::fromUtf8(currentPath))) {
        QCoreApplication::addLibraryPath(QString::fromUtf8(currentPath));
    }

    // 创建一个QCoreApplication单例，用于支持事件循环QEventLoop
    if (!QCoreApplication::instance()) {
        int argc = 1;
        char* argv[1] = { (char*)currentPath.c_str() };
        app = new QCoreApplication(argc, argv);
        // a->deleteLater();
    }
    inited = true;
}

const char* invoke_p(QVariantMap ret) {
    result = QString::fromUtf8(QJsonDocument::fromVariant(ret["body"].toMap()).toJson(QJsonDocument::Compact)).toStdString();
    return result.c_str();
}

QCLOUDMUSICAPI_EXPORT const char* invoke(char* memberName, char* value) {
    init();

    QVariantMap ret = helper.invoke(memberName, QJsonDocument::fromJson(value).toVariant().toMap());
    return invoke_p(ret);
}

QCLOUDMUSICAPI_EXPORT const char *invokeUrl(char *url) {
    init();

    QVariantMap ret = helper.invokeUrl(url);
    return invoke_p(ret);
}

QCLOUDMUSICAPI_EXPORT const char* memberName(int index) {
    NeteaseCloudMusicApi api;
    result = api.metaObject()->method(QObject().metaObject()->methodCount() + index).name().data();
    return result.c_str();
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
