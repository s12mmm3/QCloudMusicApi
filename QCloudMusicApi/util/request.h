#include <QNetworkAccessManager>
#include <QVariantMap>

#include "util_global.h"

namespace QCloudMusicApiProject {
class QCLOUDMUSICAPI_EXPORT Request {
public:
    static QString chooseUserAgent(QString ua = "");
    static QVariantMap createRequest(QNetworkAccessManager::Operation method,
                                     QString url,
                                     QVariantMap data,
                                     QVariantMap options);
};
}
