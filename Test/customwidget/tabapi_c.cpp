#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <string>

#include "tabapi_c.h"
#include "ui_tabapi_c.h"

TabApi_c::TabApi_c(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TabApi_c)
{
    ui->setupUi(this);

    ui->tabCommonUnit->callback = [this](QString member, QString arg) -> QVariantMap {
        QVariantMap ret;
        // 调用动态库的C接口invoke
        typedef const char* (*Invoke)(const char*, const char*);
        Invoke invoke = (Invoke)library.resolve("invoke");
        if (invoke) {
            ret = (QJsonDocument::fromJson(invoke(member.toStdString().c_str(), arg.toStdString().c_str())).toVariant().toMap());
        } else {
            libraryLoadFailed();
        }
        return ret;
    };
}

TabApi_c::~TabApi_c()
{
    delete ui;
    library.unload();
}

void TabApi_c::on_pushButton_select_clicked()
{
    auto fileName = QFileDialog::getOpenFileName(this);
    libraryLoad(fileName);
}

void TabApi_c::libarayLoadSucceed()
{
    QMessageBox::information(this, "", "Library load succeed!");
}

void TabApi_c::libraryLoadFailed()
{
    QMessageBox::warning(this, "Library load failed!", library.errorString());
}

void TabApi_c::libarayUnloadSucceed()
{
    QMessageBox::information(this, "", "Library unload succeed!");
}

void TabApi_c::libraryUnloadFailed()
{
    QMessageBox::warning(this, "Library unload failed!", library.errorString());
}


void TabApi_c::on_pushButton_unload_clicked()
{
    if (library.unload()) {
        libarayUnloadSucceed();
    }
    else {
        libraryUnloadFailed();
    }
}


void TabApi_c::on_pushButton_load_clicked()
{
    libraryLoad(ui->lineEdit_library_fileName->text());
}

bool TabApi_c::libraryLoad(QString fileName)
{
    bool result = false;
    if (!fileName.isEmpty()) {
        library.unload();
        library.setFileName(fileName);
        result = library.load();
        if (result) {
            ui->lineEdit_library_fileName->setText(fileName);
            libarayLoadSucceed();
        }
        else {
            libraryLoadFailed();
        }
    }
    return result;
}

