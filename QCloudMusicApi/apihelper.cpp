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
class ApiHelperPrivate;

QString ApiHelper::proxy() const
{
    Q_D(const ApiHelper);
    return d->proxy();
}

void ApiHelper::set_proxy(QString proxy)
{
    Q_D(ApiHelper);
    return d->set_proxy(proxy);
}

QString ApiHelper::realIP() const
{
    Q_D(const ApiHelper);
    return d->realIP();
}

void ApiHelper::set_realIP(QString realIP)
{
    Q_D(ApiHelper);
    return d->set_realIP(realIP);
}

QString ApiHelper::cookie() const
{
    Q_D(const ApiHelper);
    return d->cookie();
}

void ApiHelper::set_cookie(QString cookie)
{
    Q_D(ApiHelper);
    return d->set_cookie(cookie);
}

ApiHelper::ApiHelper(QObject* parent)
    : d_ptr(new ApiHelperPrivate(this))
{
}

QVariantMap ApiHelper::invoke(QString member, QVariantMap arg)
{
    Q_D(ApiHelper);
    return d->invoke(member, arg);
}

QVariantMap ApiHelper::invokeUrl(QString url)
{
    Q_D(ApiHelper);
    return d->invokeUrl(url);
}

QStringList ApiHelper::memberList()
{
    Q_D(ApiHelper);
    return d->memberList();
}

void ApiHelper::setFilterRules(const QString& rules)
{
    QLoggingCategory::setFilterRules(rules);
}

bool ApiHelper::loadPlugin(const QString &fileName)
{
    Q_D(ApiHelper);
    return d->loadPlugin(fileName);
}

bool ApiHelper::loadPlugin(QCloudMusicApiPlugin *plugin)
{
    Q_D(ApiHelper);
    return d->loadPlugin(plugin);
}

bool ApiHelper::unloadPlugin(const QString &fileName)
{
    Q_D(ApiHelper);
    return d->unloadPlugin(fileName);
}

bool ApiHelper::unloadPlugin(QCloudMusicApiPlugin *plugin)
{
    Q_D(ApiHelper);
    return d->unloadPlugin(plugin);
}
