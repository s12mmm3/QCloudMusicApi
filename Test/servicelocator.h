#ifndef SERVICELOCATOR_H
#define SERVICELOCATOR_H

#include <QObject>
#include <QJsonDocument>

class ServiceLocator: public QObject
{
    Q_OBJECT
public:
    ServiceLocator();

public:
    static void init();
    static const QJsonDocument& config() { return m_config; }
private:
    static QJsonDocument m_config;
};

#endif // SERVICELOCATOR_H
