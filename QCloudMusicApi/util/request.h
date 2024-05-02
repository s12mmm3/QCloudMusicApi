#ifndef REQUEST_H
#define REQUEST_H

#include <QFile>
#include <QNetworkAccessManager>
#include <QNetworkProxy>
#include <QVariantMap>

#include "../global.h"

namespace QCloudMusicApi {
class QCLOUDMUSICAPI_EXPORT Request {
public:
    static QString chooseUserAgent(QString ua = "");
    static QVariantMap createRequest(QNetworkAccessManager::Operation method,
                                     QString url,
                                     QVariantMap data,
                                     QVariantMap options);

    // 仿axios的网络请求，需要手动析构QNetworkReply和QNetworkAccessManager
    static QNetworkReply* axios(QNetworkAccessManager::Operation method,
                                QString url,
                                const QVariantMap &urlQuery,
                                const QVariantMap &headers,
                                const QByteArray &data,
                                QNetworkProxy proxy = QNetworkProxy::NoProxy);
};
}

#endif // REQUEST_H
