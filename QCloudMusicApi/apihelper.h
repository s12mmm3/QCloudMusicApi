#ifndef APIHELPER_H
#define APIHELPER_H

#include "module.h"

#include <QObject>
#include <QMutex>
#include <QDateTime>
#include <functional>
#include <QVariantMap>
#include <QMap>
#include <QJsonDocument>

class CacheManager {
public:
    QVariantMap getOrQueryCache(const QString& key, std::function<QVariantMap()> callback);
    QString createCacheKey(const QString& member, const QVariantMap& arg);

private:
    QMap<QString, QPair<QString, QDateTime>> cache;
    int cacheDurationSeconds = 300;
    QMutex mutex;

    // 检查缓存是否有效
    bool isCacheValid(const QString& key);
    // 更新缓存
    void updateCache(const QString& key, QVariantMap response);
    // 清理已过期的缓存
    void cleanupExpiredCache();
};

class QCLOUDMUSICAPI_EXPORT ApiHelper : public NeteaseCloudMusicApi
{
    Q_OBJECT
public:
    explicit ApiHelper(QObject *parent = nullptr);

    QVariantMap invoke(QString member, QVariantMap arg);

    QVariantMap invoke(QVariantMap (NeteaseCloudMusicApi::* member)(QVariantMap), QVariantMap arg);

    void set_cookie(QString cookie);

    DEFINE_VALUE(QVariantMap, cookie, {})

    DEFINE_VALUE(QString, proxy, "")

private:
    void beforeInvoke(QVariantMap& arg);
    void afterInvoke(QVariantMap& ret);

    CacheManager cacheManager;

public:
    static QStringList memberList();
};

#endif // APIHELPER_H
