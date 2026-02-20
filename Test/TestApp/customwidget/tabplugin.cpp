#include <QFileDialog>
#include <QJsonDocument>
#include <QMessageBox>

#include "tabplugin.h"
#include "ui_tabapi_c.h"

TabPlugin::TabPlugin(QWidget* parent) :
    TabApi_c(parent)
{
    ui->tabCommonUnit->callback = [&](QString member, QString arg) -> QVariantMap {
        return helper.invoke(member, QJsonDocument::fromJson(arg.toUtf8()).toVariant().toMap())["body"].toMap();
        };
}

bool TabPlugin::libraryLoad(QString fileName)
{
    auto pluginId = helper.loadPlugin(fileName);
    if (pluginId > 0) {
        loadedPluginIds[fileName] = pluginId;
    }
    ui->tabCommonUnit->setFunctions(helper.memberList());
    return pluginId > 0;
}

bool TabPlugin::libraryUnload(QString fileName)
{
    auto it = loadedPluginIds.find(fileName);
    if (it != loadedPluginIds.end()) {
        bool result = helper.unloadPlugin(it.value());
        if (result) {
            loadedPluginIds.erase(it);
        }
        return result;
    }
    return false;
}
