#include "tablogin_cellphone.h"
#include "ui_tablogin_cellphone.h"

#include "../servicelocator.h"

#include <QJsonDocument>

TabLogin_cellphone::TabLogin_cellphone(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::TabLogin_cellphone)
{
    ui->setupUi(this);
}

TabLogin_cellphone::~TabLogin_cellphone()
{
    delete ui;
}

void TabLogin_cellphone::on_pushButton_send_clicked()
{
    QVariantMap arg{
        { "phone", ui->lineEdit_phone->text() },
        { "password", ui->lineEdit_password->text() }
    };
    auto invoke = [](const QString member, const QVariantMap arg) {
        QVariantMap ret = ServiceLocator::helper().invoke(member, arg);
        return ret;
        };
    QVariantMap ret = invoke("login_cellphone", arg);
    ui->textEdit_ret->setText(QJsonDocument::fromVariant(ret).toJson(QJsonDocument::Indented));
}

