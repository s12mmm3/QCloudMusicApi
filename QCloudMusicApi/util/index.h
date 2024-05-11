#ifndef INDEX_H
#define INDEX_H

#include "../global.h"

#include <QVariantMap>

namespace QCloudMusicApi {

class QCLOUDMUSICAPI_EXPORT Index {
public:
    static bool toBoolean(const QVariant &val);
    static QVariantMap cookieToJson(const QString &cookie);
    static QString cookieObjToString(const QVariantMap &cookie);
    static QVariantMap mergeMap(const QVariantMap &map0, const QVariantMap &map1);
};

}
#endif // INDEX_H
