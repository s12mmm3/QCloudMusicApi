#ifndef APIHELPER_H
#define APIHELPER_H

#include "module.h"

#include <QObject>

class ApiPluginImpl;

/**
 * @class ApiHelper
 * @brief API助手类，通过反射机制调用API。
 * 继承自NeteaseCloudMusicApi，提供了调用API的高级接口。
 */
class QCLOUDMUSICAPI_EXPORT ApiHelper : public NeteaseCloudMusicApi
{
    Q_OBJECT
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
     * @brief 设置cookie
     * @param cookie 用于API调用的cookie字符串
     */
    Q_INVOKABLE void set_cookie(QString cookie);

    /**
     * @brief 获取cookie
     * @return cookie
     */
    Q_INVOKABLE QString cookie();

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
     * @brief 卸载插件
     * @param fileName 插件路径
     * @return 卸载结果
     */
    Q_INVOKABLE bool unloadPlugin(const QString& fileName);

public:
    DEFINE_VALUE(QString, proxy, "")
    DEFINE_VALUE(QString, realIP, "")

private:
    void beforeInvoke(QVariantMap& arg);
    void afterInvoke(QVariantMap& ret);

private:
    QVariantMap m_cookie;
    QStringList m_memberList;
    QList<ApiPluginImpl*> m_pluginImpls;
};

#endif // APIHELPER_H
