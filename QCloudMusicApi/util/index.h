#ifndef INDEX_H
#define INDEX_H

#include "../global.h"

#include <QVariantMap>

namespace QCloudMusicApiNS {
class QCLOUDMUSICAPI_EXPORT Index {
public:
    static QVariantMap stringToMap(const QString &cookie);
    static QString mapToString(const QVariantMap &cookie);
    static QVariantMap mergeMap(const QVariantMap &map0, const QVariantMap &map1);
};
}
#endif // INDEX_H
