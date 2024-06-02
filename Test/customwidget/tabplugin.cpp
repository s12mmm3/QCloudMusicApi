#include <QFileDialog>
#include <QJsonDocument>
#include <QMessageBox>

#include "tabplugin.h"
#include "ui_tabplugin.h"

TabPlugin::TabPlugin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TabPlugin)
{
    ui->setupUi(this);

    ui->tabCommonUnit->callback = [&](QString member, QString arg) -> QVariantMap {
        return helper.invoke(member, QJsonDocument::fromJson(arg.toUtf8()).toVariant().toMap());
    };
}

TabPlugin::~TabPlugin()
{
    delete ui;
}

void TabPlugin::on_pushButton_select_clicked()
{
    auto fileName = QFileDialog::getOpenFileName(this);
    libraryLoad(fileName);
}

void TabPlugin::libarayLoadSucceed()
{
    QMessageBox::information(this, "", "Library load succeed!");
}

void TabPlugin::libraryLoadFailed()
{
    QMessageBox::warning(this, "Library load failed!", "");
}

void TabPlugin::libarayUnloadSucceed()
{
    QMessageBox::information(this, "", "Library unload succeed!");
}

void TabPlugin::libraryUnloadFailed()
{
    QMessageBox::warning(this, "Library unload failed!", "");
}


void TabPlugin::on_pushButton_unload_clicked()
{
    libraryUnload(ui->lineEdit_library_fileName->text());
}


void TabPlugin::on_pushButton_load_clicked()
{
    libraryLoad(ui->lineEdit_library_fileName->text());
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
