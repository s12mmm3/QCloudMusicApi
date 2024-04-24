#ifndef PLUGINS_H
#define PLUGINS_H

#include "global.h"

#include <QVariantMap>

namespace QCloudMusicApiNS {
class QCLOUDMUSICAPI_EXPORT plugins : public QObject
{
    Q_OBJECT
public:
    explicit plugins(QObject *parent = nullptr);

    Q_INVOKABLE QVariantMap songUpload(QVariantMap);
signals:
};
}

#endif // PLUGINS_H
