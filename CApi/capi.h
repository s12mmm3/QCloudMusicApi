#ifndef CAPI_H
#define CAPI_H

#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) \
|| defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#define Q_DECL_EXPORT __declspec(dllexport)
#define Q_DECL_IMPORT __declspec(dllimport)
#else
#define Q_DECL_EXPORT __attribute__((visibility("default")))
#define Q_DECL_IMPORT __attribute__((visibility("default")))
#endif

#if defined(CAPI_LIBRARY)
#  define CAPI_EXPORT Q_DECL_EXPORT
#else
#  define CAPI_EXPORT Q_DECL_IMPORT
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
