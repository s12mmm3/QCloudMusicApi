#include "apihelper.h"
#include "util/index.h"
#include "util/logger.h"

#include <QMetaMethod>
#include <QUrl>
#include <QUrlQuery>

using namespace QCloudMusicApi;
ApiHelper::ApiHelper(QObject *parent)
    : NeteaseCloudMusicApi{parent}
{}

void ApiHelper::beforeInvoke(QVariantMap& arg)
{
    //Api只能处理map类型的cookie
    if(arg.contains("cookie")) {
        //如果传入新的cookie，替换原有的cookie
        if(arg["cookie"].userType() == QMetaType::QVariantMap) {
            set_cookie(arg["cookie"].toMap());
        }
        else if(arg["cookie"].userType() == QMetaType::QString) {
            set_cookie(arg["cookie"].toString());
        }
    }
    else {
        //使用存储的cookie
        arg["cookie"] = cookie();
    }

    // 设置全局代理
    if (!proxy().isEmpty()) {
        arg["proxy"] = proxy();
    }
}

void ApiHelper::afterInvoke(QVariantMap& ret)
{
    auto newCookie = Index::cookieToJson(ret["cookie"].toString());
    if (!newCookie.isEmpty()) {
        set_cookie(Index::mergeMap(cookie(), newCookie));
    }
    auto token = ret["body"].toMap()["token"].toString();
    if (!token.isEmpty()) {
        m_cookie["MUSIC_A"] = token;
    }
}

QVariantMap ApiHelper::invoke(QString member, QVariantMap arg)
{
    beforeInvoke(arg);

    QVariantMap ret;
    QMetaObject::invokeMethod(this, member.toUtf8()
                              , Qt::DirectConnection
                              , Q_RETURN_ARG(QVariantMap, ret)
                              , Q_ARG(QVariantMap, arg));

    afterInvoke(ret);

    return ret;
}

QVariantMap ApiHelper::invoke(QVariantMap (NeteaseCloudMusicApi::*member)(QVariantMap), QVariantMap arg)
{
    beforeInvoke(arg);

    // auto bind = std::bind(member, &api, arg);
    // QVariantMap ret = bind();
    QVariantMap ret = (this->*member)(arg);

    afterInvoke(ret);

    return ret;
}

QVariantMap ApiHelper::invokeUrl(QString url)
{
    QUrl qurl(url);
    auto member = qurl.path();
    if (member.startsWith('/')) {
        member.remove(0, 1);
    }
    member = member.replace("/", "_").trimmed();
    QVariantMap arg;
    for (auto& queryItem: QUrlQuery(qurl.query()).queryItems()) {
        arg[queryItem.first] = queryItem.second;
    }
    DEBUG << "host" << qurl.host()
          << "port" << qurl.port()
          << "member" << member
          << "arg" << arg;
    return invoke(member, arg);
}

QStringList ApiHelper::memberList()
{
    QStringList memberList;
    NeteaseCloudMusicApi api;
    for(int i = QObject().metaObject()->methodCount(); i < api.metaObject()->methodCount(); i++) {
        memberList.push_back(api.metaObject()->method(i).name());
    }
    return memberList;
}

void ApiHelper::set_cookie(QString cookie)
{
    set_cookie(Index::cookieToJson(cookie));
}
