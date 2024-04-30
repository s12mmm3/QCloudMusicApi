#ifndef PLUGINS_H
#define PLUGINS_H

#include "global.h"

#include <QVariantMap>

namespace QCloudMusicApiNS {
class QCLOUDMUSICAPI_EXPORT Plugins : public QObject
{
    Q_OBJECT
public:
    explicit Plugins(QObject *parent = nullptr);

    Q_INVOKABLE QVariantMap upload(QVariantMap);
signals:
};
}

#endif // PLUGINS_H
