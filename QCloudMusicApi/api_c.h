#ifndef API_C_H
#define API_C_H

#include "global.h"

#ifdef __cplusplus
extern "C" {
#endif
QCLOUDMUSICAPI_EXPORT char* invoke(char* memberName, char* value);
QCLOUDMUSICAPI_EXPORT char* memberName(int i);
QCLOUDMUSICAPI_EXPORT int memberCount();
#ifdef __cplusplus
}
#endif

#endif // API_C_H
