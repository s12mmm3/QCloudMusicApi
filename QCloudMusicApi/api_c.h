#ifndef API_C_H
#define API_C_H

#include "global.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 通过反射调用API的成员函数。
 *
 * @param memberName 要调用的成员函数的名称。
 * @param value 函数参数的JSON格式字符串。
 * @return 函数调用结果的JSON格式字符串。
 */
QCLOUDMUSICAPI_EXPORT const char* invoke(char* memberName, char* value);

/**
 * @brief 释放资源，结束时调用
 */
QCLOUDMUSICAPI_EXPORT void freeApi();

/**
 * @brief 通过索引检索成员的名称。
 *
 * @param i 成员的索引。
 * @return 指定索引处成员的名称。
 */
QCLOUDMUSICAPI_EXPORT char* memberName(int i);

/**
 * @brief 获取API中成员函数的数量。
 *
 * @return 成员函数的数量。
 */
QCLOUDMUSICAPI_EXPORT int memberCount();

#ifdef __cplusplus
}
#endif

#endif // API_C_H
