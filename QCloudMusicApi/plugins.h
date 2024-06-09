#ifndef PLUGINS_H
#define PLUGINS_H

#include "global.h"

#include <QVariantMap>

namespace QCloudMusicApi {

    class QCLOUDMUSICAPI_EXPORT Plugins : public QObject
    {
        Q_OBJECT
    public:
        explicit Plugins(QObject* parent = nullptr);

        Q_INVOKABLE static QVariantMap songUpload(QVariantMap);

        Q_INVOKABLE static QVariantMap upload(QVariantMap);
    signals:
    };

}

#endif // PLUGINS_H
