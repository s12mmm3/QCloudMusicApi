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
    bool result = false;
    if (!fileName.isEmpty()) {
        result = helper.loadPlugin(fileName);
        if (result) {
            ui->lineEdit_library_fileName->setText(fileName);
            ui->tabCommonUnit->setFunctions(helper.memberList());
            libarayLoadSucceed();
        }
        else {
            libraryLoadFailed();
        }
    }
    return result;
}

bool TabPlugin::libraryUnload(QString fileName)
{
    bool result = false;
    if (helper.unloadPlugin(fileName)) {
        libarayUnloadSucceed();
        result = true;
    }
    else {
        libraryUnloadFailed();
    }
    ui->tabCommonUnit->setFunctions(helper.memberList());
    return result;
}
