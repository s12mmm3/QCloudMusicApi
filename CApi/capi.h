#ifndef CAPI_H
#define CAPI_H

#if defined(CAPI_LIBRARY)
#  define CAPI_EXPORT __declspec(dllexport)
#else
#  define CAPI_EXPORT __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {
#endif
CAPI_EXPORT char* invoke(char* funName, char* value);
CAPI_EXPORT char* getFunName(int i);
CAPI_EXPORT int getFunCount();
#ifdef __cplusplus
}
#endif

#endif // CAPI_H
