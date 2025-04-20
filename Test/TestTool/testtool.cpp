#include "testtool.h"

#include <QFile>

static QJsonDocument g_config;
static ApiHelper g_helper;
namespace TestTool {
void init()
{
    //读取配置
    QFile file(":/config.json");
    file.open(QIODevice::ReadOnly);
    g_config = QJsonDocument::fromJson(file.readAll());
}
const QJsonDocument &config() { return g_config; }
ApiHelper *helper() { return &g_helper; }
}
