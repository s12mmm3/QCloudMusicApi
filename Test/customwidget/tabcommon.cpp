#include <QTextEdit>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QDebug>
#include <QtConcurrent>

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

    connect(this, &TabCommon::invoked, this, &TabCommon::update);
}

TabCommon::~TabCommon()
{
    delete ui;
}

void TabCommon::on_pushButton_send_clicked()
{
    ui->textEdit_ret->clear();

    auto JsonFormat = ui->checkBox->isChecked() ? QJsonDocument::Indented : QJsonDocument::Compact;
    ui->textEdit_arg->setText(QJsonDocument::fromJson(ui->textEdit_arg->toPlainText().toUtf8()).toJson(JsonFormat));

    QString member = ui->comboBox->currentText();
    QVariantMap arg = QJsonDocument::fromJson(ui->textEdit_arg->toPlainText().toUtf8()).toVariant().toMap();

    QtConcurrent::run([this](QString member, QVariantMap arg) {

        QVariantMap ret = helper.invoke(member, arg);

        emit invoked(ret);
    }, member, arg);
}


void TabCommon::on_comboBox_currentTextChanged(const QString &arg1)
{
    auto JsonFormat = ui->checkBox->isChecked() ? QJsonDocument::Indented : QJsonDocument::Compact;
    ui->textEdit_arg->setText(
        QJsonDocument(ServiceLocator::config()[arg1].toObject())
            .toJson(JsonFormat)
        );
}


void TabCommon::on_checkBox_stateChanged(int arg1)
{
    auto JsonFormat = arg1 ? QJsonDocument::Indented : QJsonDocument::Compact;
    ui->textEdit_ret->setText(QJsonDocument::fromJson(ui->textEdit_ret->toPlainText().toUtf8()).toJson(JsonFormat));
}

void TabCommon::update(QVariantMap ret)
{
    if(ui->checkBox->isChecked()) {
        ui->textEdit_ret->setText(QJsonDocument::fromVariant(ret).toJson(QJsonDocument::Indented));
    }
    else {
        ui->textEdit_ret->setText(QJsonDocument::fromVariant(ret).toJson(QJsonDocument::Compact));
    }
}
