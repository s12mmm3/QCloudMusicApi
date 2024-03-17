#include "servicelocator.h"

#include <QFile>

ServiceLocator::ServiceLocator() {}
QJsonDocument ServiceLocator::m_config;

void ServiceLocator::init()
{
    //读取配置
    QFile file(":/config.json");
    file.open(QIODevice::ReadOnly);
    m_config = QJsonDocument::fromJson(file.readAll());
}
