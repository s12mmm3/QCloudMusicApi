#ifndef API_C_H
#define API_C_H

#include "global.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 通过反射调用API的成员函数
 * @param memberName 调用的函数的名称
 * @param value 函数参数的JSON格式字符
 * @return 调用结果的JSON格式字符串
 */
QCLOUDMUSICAPI_EXPORT const char* invoke(char* memberName, char* value);

/**
 * @brief 通过反射调用API的成员函数
 * @param url 调用的API的URL，例如："/song/url/v1?id=2058263032, 2057797340&level=exhigh", "http://localhost:3000/activate/init/profile"
 * @return 调用结果的JSON格式字符串
 */
QCLOUDMUSICAPI_EXPORT const char* invokeUrl(char* url);

/**
 * @brief 释放资源，结束时调用
 */
QCLOUDMUSICAPI_EXPORT void freeApi();

/**
 * @brief 通过索引检索成员的名称
 * @param index 成员的索引。
 * @return 指定索引处成员的名称
 */
QCLOUDMUSICAPI_EXPORT const char* memberName(int index);

/**
 * @brief 获取API中成员函数的数量
 * @return 成员函数的数量
 */
QCLOUDMUSICAPI_EXPORT int memberCount();

/**
 * @brief 设置全局代理
 * @param proxy 代理地址
 */
QCLOUDMUSICAPI_EXPORT void set_proxy(char* proxy);

/**
 * @brief 设置全局cookie
 * @param cookie
 */
QCLOUDMUSICAPI_EXPORT void set_cookie(char* cookie);

#ifdef __cplusplus
}
#endif

#endif // API_C_H
