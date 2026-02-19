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
    auto ret = helper.loadPlugin(fileName);
    ui->tabCommonUnit->setFunctions(helper.memberList());
    return ret;
}

bool TabPlugin::libraryUnload(QString fileName)
{
    return helper.unloadPlugin(fileName);
}
