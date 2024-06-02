#include "apihelper.h"
#include "util/index.h"
#include "util/logger.h"
#include "qcloudmusicapiplugin.h"

#include <QMetaMethod>
#include <QPluginLoader>
#include <QUrl>
#include <QUrlQuery>

using namespace QCloudMusicApi;
ApiHelper::ApiHelper(QObject *parent)
    : NeteaseCloudMusicApi{parent}
{
    m_memberList.clear();
    NeteaseCloudMusicApi api;
    for(int i = QObject().metaObject()->methodCount(); i < api.metaObject()->methodCount(); i++) {
        m_memberList.push_back(api.metaObject()->method(i).name());
    }
}

void ApiHelper::beforeInvoke(QVariantMap& arg)
{
    // Api只能处理map类型的cookie
    if(arg.contains("cookie")) {
        // 若传入新的cookie，替换原有的cookie
        if(arg["cookie"].userType() == QMetaType::QVariantMap) {
            m_cookie = arg["cookie"].toMap();
        }
        else if(arg["cookie"].userType() == QMetaType::QString) {
            m_cookie = Index::cookieToJson(arg["cookie"].toString());
        }
    }
    // 使用存储的cookie
    arg["cookie"] = m_cookie;

    // 设置全局代理
    if (!proxy().isEmpty()) {
        arg["proxy"] = proxy();
    }
}

void ApiHelper::afterInvoke(QVariantMap& ret)
{
    auto newCookie = Index::cookieToJson(ret.value("cookie").toString());
    if (!newCookie.isEmpty()) {
        m_cookie = Index::mergeMap(m_cookie, newCookie);
    }
    auto token = ret.value("body").toMap()["token"].toString();
    if (!token.isEmpty()) {
        m_cookie["MUSIC_A"] = token;
    }
}

QVariantMap ApiHelper::invoke(QString member, QVariantMap arg)
{
    beforeInvoke(arg);

    QVariantMap ret;

    // 若方法重名，优先调用内部方法，尽量不要重名
    if (m_memberList.contains(member)) {
        QMetaObject::invokeMethod(this, member.toUtf8(),
                                  Qt::DirectConnection,
                                  Q_RETURN_ARG(QVariantMap, ret),
                                  Q_ARG(QVariantMap, arg));
    }
    else {
        for (auto& pluginImpl: m_pluginImpls) {
            if (pluginImpl->plugin->memberList().contains(member)) {
                ret = pluginImpl->plugin->invoke(member, arg);
                break;
            }
        }
    }

    afterInvoke(ret);

    return ret;
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

QVariantMap ApiHelper::invokeUrl(QString url)
{
    QUrl qurl(url);
    auto member = qurl.path();
    if (member.startsWith('/')) {
        member.remove(0, 1);
    }
    member = member.replace("/", "_").trimmed();
    QVariantMap arg;
    for (auto& queryItem: QUrlQuery(qurl.query()).queryItems()) {
        arg[queryItem.first] = queryItem.second;
    }
    DEBUG << "host" << qurl.host()
          << "port" << qurl.port()
          << "member" << member
          << "arg" << arg;
    return invoke(member, arg);
}

QStringList ApiHelper::memberList()
{
    // 原生方法 + 插件中的方法
    auto memberList = m_memberList;

    for (auto& pluginImpl: m_pluginImpls) {
        memberList.append(pluginImpl->plugin->memberList());
    }
    return memberList;
}

bool ApiHelper::loadPlugin(const QString &fileName)
{
    ApiPluginImpl* pluginImpl = new ApiPluginImpl();
    QPluginLoader *loader = new QPluginLoader(this);
    loader->setFileName(fileName);
    if (loader->load())
    {
        QCloudMusicApiPlugin *plugin = qobject_cast<QCloudMusicApiPlugin *>(loader->instance());
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

bool ApiHelper::unloadPlugin(const QString &fileName)
{
    for (auto i = 0; i < m_pluginImpls.size(); i++) {
        auto pluginImpl = m_pluginImpls[i];
        if (pluginImpl->loader->fileName() == fileName) {
            m_pluginImpls.remove(i);
            pluginImpl->loader->deleteLater();
            return pluginImpl->loader->unload();
        }
    }
    return false;
}

void ApiHelper::set_cookie(QString cookie)
{
    m_cookie = Index::cookieToJson(cookie);
}

QString ApiHelper::cookie()
{
    return Index::cookieObjToString(m_cookie);
}
