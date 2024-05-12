#include <QTextEdit>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QDebug>

#include "../servicelocator.h"
#include "tabcommon.h"
#include "ui_tabcommon.h"

TabCommon::TabCommon(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TabCommon)
{
    ui->setupUi(this);
    //将Api中的方法名称取出
    ui->comboBox->addItems(ApiHelper::memberList());

    // for (int i = 0; i < 15; i++) {
    //     QVariantMap ret = ServiceLocator::helper().invoke("register_anonimous", {});
    //     auto cookie = ret.value("cookie", "").toString();
    //     if (!cookie.isEmpty()) {
    //         ServiceLocator::helper().set_cookie(cookie);
    //         break;
    //     }
    // }
}

TabCommon::~TabCommon()
{
    delete ui;
}

void TabCommon::on_pushButton_send_clicked()
{
    ui->textEdit_ret->clear();

    {
        // 更新arg的Json格式
        auto JsonFormat = ui->checkBox->isChecked() ? QJsonDocument::Indented : QJsonDocument::Compact;
        auto arg = ui->textEdit_arg->toPlainText();
        auto arg_new = QJsonDocument::fromJson(arg.toUtf8()).toJson(JsonFormat);
        if (arg != arg_new) ui->textEdit_arg->setText(arg_new);
    }

    QString member = ui->comboBox->currentText();
    QVariantMap arg = QJsonDocument::fromJson(ui->textEdit_arg->toPlainText().toUtf8()).toVariant().toMap();

    QVariantMap ret = ServiceLocator::helper().invoke(member, arg);

    update(ret);
}


void TabCommon::on_comboBox_currentTextChanged(const QString &arg1)
{
    // 从config中读取当前接口的测试数据
    auto JsonFormat = ui->checkBox->isChecked() ? QJsonDocument::Indented : QJsonDocument::Compact;
    ui->textEdit_arg->setText(
        QJsonDocument(ServiceLocator::config()[arg1].toObject())
            .toJson(JsonFormat)
        );
}


void TabCommon::on_checkBox_stateChanged(int arg1)
{
    // 更新ret的Json格式
    auto JsonFormat = arg1 ? QJsonDocument::Indented : QJsonDocument::Compact;
    ui->textEdit_ret->setText(QJsonDocument::fromJson(ui->textEdit_ret->toPlainText().toUtf8()).toJson(JsonFormat));
}

void TabCommon::update(QVariantMap ret)
{
    auto JsonFormat = ui->checkBox->isChecked() ? QJsonDocument::Indented : QJsonDocument::Compact;
    ui->textEdit_ret->setText(QJsonDocument::fromVariant(ret).toJson(JsonFormat));
}
