#include "tablogin_cellphone.h"
#include "ui_tablogin_cellphone.h"

#include "../../QCloudMusicApi/module.h"

#include <QJsonDocument>

TabLogin_cellphone::TabLogin_cellphone(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TabLogin_cellphone)
{
    ui->setupUi(this);
}

TabLogin_cellphone::~TabLogin_cellphone()
{
    delete ui;
}

QVariantMap TabLogin_cellphone::invoke(const QString funName, const QVariantMap arg) {
    NeteaseCloudMusicApi api;
    QVariantMap ret;
    QMetaObject::invokeMethod(&api, funName.toUtf8()
                              , Qt::DirectConnection
                              , Q_RETURN_ARG(QVariantMap, ret)
                              , Q_ARG(QVariantMap, arg));
    return ret;
}

void TabLogin_cellphone::on_pushButton_send_clicked()
{
    QVariantMap arg {
        { "phone", ui->lineEdit_phone->text() },
        { "password", ui->lineEdit_password->text() }
    };
    QVariantMap ret = invoke("login_cellphone", arg);
    ui->textEdit_ret->setText(QJsonDocument::fromVariant(ret).toJson(QJsonDocument::Indented));
}

