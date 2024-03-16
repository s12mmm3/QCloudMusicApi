#ifndef INDEX_H
#define INDEX_H

#include "../global.h"

#include <QNetworkCookie>
#include <QVariantMap>

namespace QCloudMusicApiProject {
class QCLOUDMUSICAPI_EXPORT Index {
public:
    static QVariantMap stringToMap(const QString &cookie);
    static QString mapToString(const QVariantMap &cookie);
    static QString cookieToString(const QList<QNetworkCookie> &cookie);
    static QList<QNetworkCookie> stringToCookie(const QString &cookie);
    static QList<QNetworkCookie> mapToCookie(QVariantMap &cookie);
    static QVariantMap mergeMap(const QVariantMap &map0, const QVariantMap &map1);
};
}
#endif // INDEX_H
