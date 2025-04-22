#ifndef APIHELPER_H
#define APIHELPER_H

#include "qcloudmusicapiglobal.h"

#include <QMutex>
#include <QObject>
#include <QVariantMap>
#include <QScopedPointer>
#include <QStringList>

// 前向声明
class ApiHelperPrivate;
class QCloudMusicApiPlugin;

/**
 * @class ApiHelper
 * @brief API助手类，通过反射机制调用API，提供了调用API的高级接口。
 */
class QCLOUDMUSICAPI_EXPORT ApiHelper : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString proxy READ proxy WRITE set_proxy NOTIFY proxyChanged);
    Q_PROPERTY(QString realIP READ realIP WRITE set_realIP NOTIFY realIPChanged);
    Q_PROPERTY(QString cookie READ cookie WRITE set_cookie NOTIFY cookieChanged);

public:
    QString proxy() const;
    void set_proxy(QString proxy);
    QString realIP() const;
    void set_realIP(QString realIP);
    QString cookie() const;
    void set_cookie(QString cookie);

public:
    explicit ApiHelper(QObject* parent = nullptr);

    /**
     * @brief 调用API的成员函数
     * @param member 成员函数名
     * @param arg 调用参数
     * @return 返回调用结果
     */
    Q_INVOKABLE QVariantMap invoke(QString member, QVariantMap arg);

    /**
     * @brief 调用API的成员函数
     * @param url 要调用的API的URL，例如："/song/url/v1?id=2058263032, 2057797340&level=exhigh", "http://localhost:3000/activate/init/profile"
     * @return 返回API调用结果
     */
    Q_INVOKABLE QVariantMap invokeUrl(QString url);

    /**
     * @brief 获取API列表
     * @return API列表
     */
    Q_INVOKABLE QStringList memberList();

    /**
     * @brief 设置全局log规则，例如："QCloudMusicApi.debug=false"
     * @param rules log规则
     */
    Q_INVOKABLE void setFilterRules(const QString& rules);

    /**
     * @brief 加载插件
     * @param fileName 插件路径
     * @return 加载结果
     */
    Q_INVOKABLE bool loadPlugin(const QString& fileName);

    /**
     * @brief 加载插件
     * @param plugin 插件指针
     * @return 加载结果
     */
    Q_INVOKABLE bool loadPlugin(QCloudMusicApiPlugin* plugin);

    /**
     * @brief 卸载插件
     * @param fileName 插件路径
     * @return 卸载结果
     */
    Q_INVOKABLE bool unloadPlugin(const QString& fileName);

    /**
     * @brief 卸载插件
     * @param plugin 插件指针
     * @return 卸载结果
     */
    Q_INVOKABLE bool unloadPlugin(QCloudMusicApiPlugin* plugin);

signals:
    void proxyChanged();
    void realIPChanged();
    void cookieChanged();

private:
    Q_DECLARE_PRIVATE(ApiHelper)
    QScopedPointer<QObject> d_ptr;
};

#endif // APIHELPER_H
