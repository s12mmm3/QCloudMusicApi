#ifndef REQUEST_H
#define REQUEST_H

#include <QFile>
#include <QNetworkAccessManager>
#include <QNetworkProxy>
#include <QVariantMap>

#include "../global.h"

namespace QCloudMusicApi {

// 请求管理类，参考原项目文件util/request.js
class QCLOUDMUSICAPI_EXPORT Request {
public:
    static QString chooseUserAgent(QString uaType);
    static QVariantMap createRequest(QNetworkAccessManager::Operation method,
                                     QString url,
                                     QVariantMap data,
                                     QVariantMap options);

    // 仿axios的网络请求，需要手动析构QNetworkAccessManager, QNetworkReply生命周期由其控制
    static QNetworkReply* axios(QNetworkAccessManager::Operation method,
                                QString url,
                                const QVariantMap &urlQuery,
                                const QVariantMap &headers,
                                const QByteArray &data,
                                QNetworkProxy proxy = QNetworkProxy::DefaultProxy);
};

}

#endif // REQUEST_H
