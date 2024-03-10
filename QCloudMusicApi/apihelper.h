#ifndef APIHELPER_H
#define APIHELPER_H

#include "module.h"

#include <QObject>

#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) \
|| defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#define Q_DECL_EXPORT __declspec(dllexport)
#define Q_DECL_IMPORT __declspec(dllimport)
#else
#define Q_DECL_EXPORT __attribute__((visibility("default")))
#define Q_DECL_IMPORT __attribute__((visibility("default")))
#endif

#if defined(BUILD_SHARED_LIBS)
#if defined(QCLOUDMUSICAPI_LIBRARY)
#  define QCLOUDMUSICAPI_EXPORT Q_DECL_EXPORT
#else
#  define QCLOUDMUSICAPI_EXPORT Q_DECL_IMPORT
#endif
#else
#  define QCLOUDMUSICAPI_EXPORT
#endif

class QCLOUDMUSICAPI_EXPORT ApiHelper : public QObject
{
    Q_OBJECT
public:
    explicit ApiHelper(QObject *parent = nullptr);

    QVariantMap invoke(QString member, QVariantMap arg);

    QVariantMap invoke(QVariantMap (NeteaseCloudMusicApi::* member)(QVariantMap), QVariantMap arg);

private:
    NeteaseCloudMusicApi api;
};

#endif // APIHELPER_H
