#ifndef TABPLUGIN_H
#define TABPLUGIN_H

#include "../../QCloudMusicApi/apihelper.h"
#include "tabapi_c.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class TabPlugin;
}
QT_END_NAMESPACE

// 插件测试
class TabPlugin : public TabApi_c
{

public:
    explicit TabPlugin(QWidget *parent = nullptr);

private:
    bool libraryLoad(QString fileName) override;
    bool libraryUnload(QString fileName) override;

private:
    ApiHelper helper;
};

#endif // TABPLUGIN_H
