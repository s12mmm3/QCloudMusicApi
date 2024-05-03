#include <QTextEdit>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <cstring>

#include "../servicelocator.h"
#include "tabapi_c.h"
#include "ui_tabapi_c.h"

TabApi_c::TabApi_c(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TabApi_c)
{
    ui->setupUi(this);
    //将Api中的方法名称取出
    ui->comboBox->addItems(ApiHelper::memberList());
}

TabApi_c::~TabApi_c()
{
    delete ui;
    library.unload();
}

void TabApi_c::on_pushButton_send_clicked()
{
    ui->textEdit_ret->clear();

    auto JsonFormat = ui->checkBox->isChecked() ? QJsonDocument::Indented : QJsonDocument::Compact;
    ui->textEdit_arg->setText(QJsonDocument::fromJson(ui->textEdit_arg->toPlainText().toUtf8()).toJson(JsonFormat));

    typedef const char* (*Invoke)(char*, char*);
    Invoke invoke = (Invoke)library.resolve("invoke");
    if (invoke) {
        std::string member = ui->comboBox->currentText().toStdString();
        std::string arg = ui->textEdit_arg->toPlainText().toStdString();

        char* member_c = new char[member.size() + 1];
        std::strcpy(member_c, member.c_str());
        char* arg_c = new char[arg.size() + 1];
        std::strcpy(arg_c, arg.c_str());

        auto ret = invoke(member_c, arg_c);

        delete[] member_c;
        delete[] arg_c;

        update(QJsonDocument::fromJson(ret).toVariant().toMap());
    } else {
        libraryLoadFailed();
    }
}


void TabApi_c::on_comboBox_currentTextChanged(const QString &arg1)
{
    auto JsonFormat = ui->checkBox->isChecked() ? QJsonDocument::Indented : QJsonDocument::Compact;
    ui->textEdit_arg->setText(
        QJsonDocument(ServiceLocator::config()[arg1].toObject())
            .toJson(JsonFormat)
        );
}


void TabApi_c::on_checkBox_stateChanged(int arg1)
{
    auto JsonFormat = arg1 ? QJsonDocument::Indented : QJsonDocument::Compact;
    ui->textEdit_ret->setText(QJsonDocument::fromJson(ui->textEdit_ret->toPlainText().toUtf8()).toJson(JsonFormat));
}

void TabApi_c::update(QVariantMap ret)
{
    if(ui->checkBox->isChecked()) {
        ui->textEdit_ret->setText(QJsonDocument::fromVariant(ret).toJson(QJsonDocument::Indented));
    }
    else {
        ui->textEdit_ret->setText(QJsonDocument::fromVariant(ret).toJson(QJsonDocument::Compact));
    }
}

void TabApi_c::on_pushButton_select_clicked()
{
    QFileInfo fileInfo = QFileInfo(QFileDialog::getOpenFileName(this));
    auto filePath = fileInfo.filePath();
    if (!filePath.isEmpty()) {
        library.unload();
        library.setFileName(filePath);
        if (library.load()) {
            ui->lineEdit_library_fileName->setText(filePath);
            libarayLoadSucceed();
        }
        else {
            libraryLoadFailed();
        }
    }
}

void TabApi_c::libarayLoadSucceed()
{
    QMessageBox::information(this, "", "动态库加载成功！");
}

void TabApi_c::libraryLoadFailed()
{
    QMessageBox::warning(this, "动态库加载失败！", library.errorString());
}

void TabApi_c::libarayUnloadSucceed()
{
    QMessageBox::information(this, "", "动态库卸载成功！");
}

void TabApi_c::libraryUnloadFailed()
{
    QMessageBox::warning(this, "动态库卸载失败！", library.errorString());
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

