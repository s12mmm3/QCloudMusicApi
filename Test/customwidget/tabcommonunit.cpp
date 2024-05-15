#include <QTextEdit>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QDebug>

#include "../servicelocator.h"
#include "tabcommonunit.h"
#include "ui_tabcommonunit.h"

TabCommonUnit::TabCommonUnit(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TabCommonUnit)
{
    ui->setupUi(this);

    comboBox_function = ui->comboBox_function;
    textEdit_arg = ui->textEdit_arg;
    textEdit_ret = ui->textEdit_ret;

    //将Api中的方法名称取出
    ui->comboBox_function->addItems(ApiHelper::memberList());
}

TabCommonUnit::~TabCommonUnit()
{
    delete ui;
}

void TabCommonUnit::on_pushButton_send_clicked()
{
    ui->textEdit_ret->clear();

    // 更新arg的Json格式
    auto JsonFormat = ui->checkBox->isChecked() ? QJsonDocument::Indented : QJsonDocument::Compact;
    auto arg = ui->textEdit_arg->toPlainText();
    auto arg_new = QJsonDocument::fromJson(arg.toUtf8()).toJson(JsonFormat);
    if (arg != arg_new) ui->textEdit_arg->setText(arg_new);

    if (callback) {
        QString member = ui->comboBox_function->currentText();
        QString arg = ui->textEdit_arg->toPlainText();
        QVariantMap ret = callback(member, arg);

        update(ret);
    }
}


void TabCommonUnit::on_comboBox_function_currentTextChanged(const QString &arg1)
{
    // 从config中读取当前接口的测试数据
    auto JsonFormat = ui->checkBox->isChecked() ? QJsonDocument::Indented : QJsonDocument::Compact;
    ui->textEdit_arg->setText(
        QJsonDocument(ServiceLocator::config()[arg1].toObject())
            .toJson(JsonFormat)
        );
}


void TabCommonUnit::on_checkBox_stateChanged(int arg1)
{
    // 更新ret的Json格式
    auto JsonFormat = arg1 ? QJsonDocument::Indented : QJsonDocument::Compact;
    ui->textEdit_ret->setText(QJsonDocument::fromJson(ui->textEdit_ret->toPlainText().toUtf8()).toJson(JsonFormat));
}

void TabCommonUnit::update(QVariantMap ret)
{
    auto JsonFormat = ui->checkBox->isChecked() ? QJsonDocument::Indented : QJsonDocument::Compact;
    ui->textEdit_ret->setText(QJsonDocument::fromVariant(ret).toJson(JsonFormat));
}
