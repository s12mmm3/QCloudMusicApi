#include "apihelper.h"
#include "util/index.h"

#include <QMetaMethod>

using namespace QCloudMusicApi;

QVariantMap CacheManager::getOrQueryCache(const QString& key, std::function<QVariantMap()> callback)
{
    // 在获取缓存前先清理过期缓存
    cleanupExpiredCache();

    if (isCacheValid(key)) {
        return cache[key].first;
    } else {
        QVariantMap response = callback();
        updateCache(key, response);
        return response;
    }
}

QString CacheManager::createCacheKey(const QString& member, const QVariantMap& arg)
{
    // 这个实现依赖于成员名称和参数
    return member + "|" + QString::fromUtf8(QJsonDocument::fromVariant(arg).toJson(QJsonDocument::Compact));
}

// 检查缓存是否有效
bool CacheManager::isCacheValid(const QString& key)
{
    if (cache.contains(key)) {
        auto expiry = cache[key].second;
        return QDateTime::currentDateTime() <= expiry;
    }
    return false;
}

// 更新缓存
void CacheManager::updateCache(const QString& key, QVariantMap response)
{
    QDateTime expiry = QDateTime::currentDateTime().addSecs(cacheDurationSeconds);
    mutex.lock();
    cache[key] = qMakePair(response, expiry);
    mutex.unlock();
}

// 清理已过期的缓存
void CacheManager::cleanupExpiredCache()
{
    QDateTime now = QDateTime::currentDateTime();
    mutex.lock();
    auto it = cache.begin();
    while (it != cache.end()) {
        if (it.value().second < now) {
            it = cache.erase(it);  // 删除过期条目
        } else {
            ++it;
        }
    }
    mutex.unlock();
}

ApiHelper::ApiHelper(QObject *parent)
    : NeteaseCloudMusicApi{parent}
{}

void ApiHelper::beforeInvoke(QVariantMap& arg)
{
    //Api只能处理map类型的cookie
    if(arg.contains("cookie")) {
        //如果传入新的cookie，替换原有的cookie
        if(arg["cookie"].userType() == QMetaType::QVariantMap) {
            set_cookie(arg["cookie"].toMap());
        }
        else if(arg["cookie"].userType() == QMetaType::QString) {
            set_cookie(arg["cookie"].toString());
        }
    }
    else {
        //使用存储的cookie
        arg["cookie"] = cookie();
    }

    // 设置全局代理
    if (!proxy().isEmpty()) {
        arg["proxy"] = proxy();
    }
}

void ApiHelper::afterInvoke(QVariantMap& ret)
{
    auto newCookie = Index::stringToMap(ret["cookie"].toString());
    if (!newCookie.isEmpty()) {
        set_cookie(Index::mergeMap(cookie(), newCookie));
    }
    auto token = ret["body"].toMap()["token"].toString();
    if (!token.isEmpty()) {
        m_cookie["MUSIC_A"] = token;
    }
}

QVariantMap ApiHelper::invoke(QString member, QVariantMap arg)
{
    auto callback = [&]() -> QVariantMap
    {
        beforeInvoke(arg);

        QVariantMap ret;
        QMetaObject::invokeMethod(this, member.toUtf8(), Qt::DirectConnection,
                                    Q_RETURN_ARG(QVariantMap, ret),
                                    Q_ARG(QVariantMap, arg));

        afterInvoke(ret);

        return ret;
    };

    if (arg.contains("do-cache"))
    {
        QString cacheKey = cacheManager.createCacheKey(member, arg);
        return cacheManager.getOrQueryCache(
            cacheKey,
            callback);
    }
    return callback();
}

QVariantMap ApiHelper::invoke(QVariantMap (NeteaseCloudMusicApi::*member)(QVariantMap), QVariantMap arg)
{
    beforeInvoke(arg);

    // auto bind = std::bind(member, &api, arg);
    // QVariantMap ret = bind();
    QVariantMap ret = (this->*member)(arg);

    afterInvoke(ret);

    return ret;
}

QStringList ApiHelper::memberList()
{
    QStringList memberList;
    NeteaseCloudMusicApi api;
    for(int i = QObject().metaObject()->methodCount(); i < api.metaObject()->methodCount(); i++) {
        memberList.push_back(api.metaObject()->method(i).name());
    }
    return memberList;
}

void ApiHelper::set_cookie(QString cookie)
{
    set_cookie(Index::stringToMap(cookie));
}
