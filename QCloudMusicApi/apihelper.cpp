#include "apihelper.h"

#include <functional>

ApiHelper::ApiHelper(QObject *parent)
    : QObject{parent}
{}

QVariantMap ApiHelper::invoke(QString member, QVariantMap arg)
{
    QVariantMap ret;
    QMetaObject::invokeMethod(&api, member.toUtf8()
                              , Qt::DirectConnection
                              , Q_RETURN_ARG(QVariantMap, ret)
                              , Q_ARG(QVariantMap, arg));
    return ret;
}

QVariantMap ApiHelper::invoke(QVariantMap (NeteaseCloudMusicApi::*member)(QVariantMap), QVariantMap arg)
{
    auto bind = std::bind(member, &api, arg);
    QVariantMap ret = bind();
    return ret;
}
