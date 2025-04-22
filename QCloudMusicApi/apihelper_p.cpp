#include "module.h"
#include "apihelper.h"
#include "apihelper_p.h"
#include "util/index.h"
#include "util/logger.h"
#include "qcloudmusicapiplugin.h"

#include <QMetaMethod>
#include <QPluginLoader>
#include <QThread>
#include <QUrl>
#include <QUrlQuery>

using namespace QCloudMusicApi;

ApiHelperPrivate::ApiHelperPrivate(ApiHelper* q):
    q_ptr(q),
    m_api(new NeteaseCloudMusicApi)
{
    m_memberList.clear();
    for (int i = QObject().metaObject()->methodCount(); i < m_api->metaObject()->methodCount(); i++) {
        m_memberList.push_back(m_api->metaObject()->method(i).name());
    }
}

QString ApiHelperPrivate::proxy() const { return m_proxy; }

void ApiHelperPrivate::set_proxy(QString proxy)
{
    if (proxy == m_proxy) {
        return;
    }
    m_proxy = proxy;
    Q_Q(ApiHelper);
    emit q->proxyChanged();
}

QString ApiHelperPrivate::realIP() const { return m_realIP; }

void ApiHelperPrivate::set_realIP(QString realIP)
{
    if (realIP == m_realIP) {
        return;
    }
    m_realIP = realIP;
    Q_Q(ApiHelper);
    emit q->realIPChanged();
}

QString ApiHelperPrivate::cookie() const { return m_cookie; }

void ApiHelperPrivate::set_cookie(QString cookie)
{
    if (cookie == m_cookie) {
        return;
    }
    m_cookie = cookie;
    Q_Q(ApiHelper);
    emit q->cookieChanged();
}

void ApiHelperPrivate::beforeInvoke(QVariantMap& arg)
{
    QMutexLocker locker(&m_mutex);
    QVariantMap arg_cookie_map = Index::cookieToJson(cookie());
    // Api只能处理map类型的cookie
    if (arg.contains("cookie")) {
        // 若传入新的cookie，替换原有的cookie
        if (arg["cookie"].userType() == QMetaType::QVariantMap) {
            arg_cookie_map = arg["cookie"].toMap();
        }
        else if (arg["cookie"].userType() == QMetaType::QString) {
            arg_cookie_map = Index::cookieToJson(arg["cookie"].toString());
        }
        set_cookie(Index::cookieObjToString(arg_cookie_map));
    }
    // 使用存储的cookie map
    arg["cookie"] = arg_cookie_map;

    // 设置全局代理
    if (!proxy().isEmpty() && !arg.contains("proxy")) {
        arg.insert("proxy", proxy());
    }

    // 设置全局realIP
    if (!realIP().isEmpty() && !arg.contains("realIP")) {
        arg.insert("realIP", realIP());
    }
}

void ApiHelperPrivate::afterInvoke(QVariantMap& ret)
{
    QMutexLocker locker(&m_mutex);
    QVariantMap arg_cookie_map = Index::cookieToJson(cookie());
    auto newCookie = Index::cookieToJson(ret.value("cookie").toString());
    if (!newCookie.isEmpty()) {
        arg_cookie_map = Index::mergeMap(arg_cookie_map, newCookie);
    }
    auto token = ret.value("body").toMap()["token"].toString();
    if (!token.isEmpty()) {
        arg_cookie_map["MUSIC_A"] = token;
    }
    set_cookie(Index::cookieObjToString(arg_cookie_map));
}

QVariantMap ApiHelperPrivate::invoke(QString member, QVariantMap arg)
{
    beforeInvoke(arg);
    QVariantMap ret;

    // 若方法重名，优先调用插件方法，尽量不要重名
    bool useNative = true;
    for (auto& pluginImpl : m_pluginImpls) {
        if (pluginImpl->plugin->memberList().contains(member)) {
            DEBUG << QString("found member %1 in: %2").arg(member).arg(pluginImpl->loader->fileName());
            ret = pluginImpl->plugin->invoke(member, arg);
            useNative = false;
            break;
        }
    }
    if (useNative) {
        QMetaObject::invokeMethod(m_api.data(), member.toUtf8(),
                                  Q_RETURN_ARG(QVariantMap, ret),
                                  Q_ARG(QVariantMap, arg));
    }

    afterInvoke(arg);
    return ret;
}

QVariantMap ApiHelperPrivate::invokeUrl(QString url)
{
    QUrl qurl(url);
    auto member = qurl.path();
    if (member.startsWith('/')) {
        member.remove(0, 1);
    }
    member = member.replace("/", "_").trimmed();
    QVariantMap arg;
    for (auto& queryItem : QUrlQuery(qurl.query()).queryItems()) {
        arg[queryItem.first] = queryItem.second;
    }
    DEBUG << "host" << qurl.host()
          << "port" << qurl.port()
          << "member" << member
          << "arg" << arg;
    return invoke(member, arg);
}

QStringList ApiHelperPrivate::memberList()
{
    // 原生方法 + 插件中的方法
    auto memberList = m_memberList;

    for (auto& pluginImpl : m_pluginImpls) {
        memberList.append(pluginImpl->plugin->memberList());
    }
    return memberList;
}

bool ApiHelperPrivate::loadPlugin(const QString& fileName)
{
    // 加载过的插件不再重复加载
    for (auto i = 0; i < m_pluginImpls.size(); i++) {
        auto pluginImpl = m_pluginImpls[i];
        if (pluginImpl->loader->fileName() == fileName) {
            return true;
        }
    }

    ApiPluginImpl* pluginImpl = new ApiPluginImpl();
    QPluginLoader* loader = new QPluginLoader(this);
    loader->setFileName(fileName);
    if (loader->load())
    {
        QCloudMusicApiPlugin* plugin = qobject_cast<QCloudMusicApiPlugin*>(loader->instance());
        if (plugin) {
            pluginImpl->loader = loader;
            pluginImpl->plugin = plugin;
            m_pluginImpls.push_back(pluginImpl);
            return true;
        }
    }
    DEBUG << loader->errorString();
    loader->deleteLater();
    return false;
}

bool ApiHelperPrivate::loadPlugin(QCloudMusicApiPlugin *plugin)
{
    ApiPluginImpl* pluginImpl = new ApiPluginImpl();
    if (plugin) {
        pluginImpl->loader = new QPluginLoader(this);
        pluginImpl->plugin = plugin;
        m_pluginImpls.push_back(pluginImpl);
        return true;
    }
    return false;
}

bool ApiHelperPrivate::unloadPlugin(const QString& fileName)
{
    auto result = false;
    for (auto i = 0; i < m_pluginImpls.size(); i++) {
        auto pluginImpl = m_pluginImpls[i];
        if (pluginImpl->loader->fileName() == fileName) {
            m_pluginImpls.removeAt(i);
            pluginImpl->loader->deleteLater();
            result = pluginImpl->loader->unload();
            if (!result) {
                DEBUG << pluginImpl->loader->errorString();
            }
            break;
        }
    }
    return result;
}

bool ApiHelperPrivate::unloadPlugin(QCloudMusicApiPlugin *plugin)
{
    auto result = false;
    for (auto i = 0; i < m_pluginImpls.size(); i++) {
        auto pluginImpl = m_pluginImpls[i];
        if (pluginImpl->plugin == plugin) {
            m_pluginImpls.removeAt(i);
            pluginImpl->loader->deleteLater();
            result = pluginImpl->loader->unload();
            if (!result) {
                DEBUG << pluginImpl->loader->errorString();
            }
            break;
        }
    }
    return result;
}
