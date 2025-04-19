#ifndef SERVICELOCATOR_H
#define SERVICELOCATOR_H

#include <QObject>
#include <QJsonDocument>

#include "../QCloudMusicApi/apihelper.h"

namespace ServiceLocator
{
void init();
const QJsonDocument& config();
ApiHelper* helper();
};

#endif // SERVICELOCATOR_H
