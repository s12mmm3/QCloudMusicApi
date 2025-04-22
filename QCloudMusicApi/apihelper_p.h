#ifndef APIHELPER_P_H
#define APIHELPER_P_H

#include <QObject>
#include <QScopedPointer>
#include <QStringList>
#include <QList>
#include <QMutex>

class ApiHelper;
class QCloudMusicApiPlugin;
class ApiPluginImpl;
class ApiHelperPrivate : public QObject
{
    Q_OBJECT
public:
    explicit ApiHelperPrivate(ApiHelper* q);

    // 公共接口实现
    QString proxy() const;
    void set_proxy(QString proxy);
    QString realIP() const;
    void set_realIP(QString realIP);
    QString cookie() const;
    void set_cookie(QString cookie);

    QVariantMap invoke(QString member, QVariantMap arg);
    QVariantMap invokeUrl(QString url);
    QStringList memberList();
    bool loadPlugin(const QString& fileName);
    bool loadPlugin(QCloudMusicApiPlugin* plugin);
    bool unloadPlugin(const QString& fileName);
    bool unloadPlugin(QCloudMusicApiPlugin* plugin);

private:
    void beforeInvoke(QVariantMap& arg);
    void afterInvoke(QVariantMap& ret);

private:
    Q_DECLARE_PUBLIC(ApiHelper) // 关键宏
    ApiHelper* const q_ptr; // 公有类指针
    // 成员变量
    QString m_proxy;
    QString m_realIP;
    QString m_cookie;

    QScopedPointer<QObject> m_api;

    QStringList m_memberList;
    QList<ApiPluginImpl*> m_pluginImpls;

    QMutex m_mutex;
};

#endif // APIHELPER_P_H
