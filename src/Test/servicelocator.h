#ifndef SERVICELOCATOR_H
#define SERVICELOCATOR_H

#include <QObject>
#include <QJsonDocument>

#include "../QCloudMusicApi/apihelper.h"

class ServiceLocator: public QObject
{
    Q_OBJECT
public:
    ServiceLocator();

public:
    static void init();
    static const QJsonDocument& config() { return m_config; }
    static ApiHelper& helper() { return m_helper; }
private:
    static QJsonDocument m_config;
    static ApiHelper m_helper;
};

#endif // SERVICELOCATOR_H
