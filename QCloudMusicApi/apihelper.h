#ifndef APIHELPER_H
#define APIHELPER_H

#include "module.h"

#include <QObject>

class QCLOUDMUSICAPI_EXPORT ApiHelper : public NeteaseCloudMusicApi
{
    Q_OBJECT
public:
    explicit ApiHelper(QObject *parent = nullptr);

    QVariantMap invoke(QString member, QVariantMap arg);

    QVariantMap invoke(QVariantMap (NeteaseCloudMusicApi::* member)(QVariantMap), QVariantMap arg);

    void set_cookie(QString cookie);

private:
    void beforeInvoke(QVariantMap& arg);
    void afterInvoke(QVariantMap& ret);

private:
    DEFINE_VALUE(QVariantMap, cookie, {})

public:
    static QStringList memberList();
};

#endif // APIHELPER_H
