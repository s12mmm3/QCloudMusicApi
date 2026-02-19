#include <QTextEdit>
#include <QJsonObject>
#include <QFile>
#include <QDebug>

#include "testtool.h"
#include "tabcommonunit.h"
#include "ui_tabcommonunit.h"

TabCommonUnit::TabCommonUnit(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::TabCommonUnit)
{
    ui->setupUi(this);

    updateFunctions();
}

TabCommonUnit::~TabCommonUnit()
{
    delete ui;
}

QComboBox *TabCommonUnit::comboBox_function() { return ui->comboBox_function; }
QTextEdit *TabCommonUnit::textEdit_arg() { return ui->textEdit_arg; }
QTextEdit *TabCommonUnit::textEdit_ret() { return ui->textEdit_ret; }
QJsonDocument::JsonFormat TabCommonUnit::getJsonFormat() { return ui->checkBox->isChecked() ? QJsonDocument::Indented : QJsonDocument::Compact; }

void TabCommonUnit::on_pushButton_send_clicked()
{
    ui->textEdit_ret->clear();

    // 更新arg的Json格式
    auto arg = ui->textEdit_arg->toPlainText();
    auto arg_new = QJsonDocument::fromJson(arg.toUtf8()).toJson(getJsonFormat());
    if (arg != arg_new) ui->textEdit_arg->setText(arg_new);

    QString member = ui->comboBox_function->currentText();
    arg = ui->textEdit_arg->toPlainText();
    QVariantMap ret = callback ? callback(member, arg) : QVariantMap{};
    update(ret);
}


void TabCommonUnit::on_comboBox_function_currentTextChanged(const QString& arg1)
{
    // 从config中读取当前接口的测试数据
    ui->textEdit_arg->setText(
        QJsonDocument(TestTool::config()[arg1].toObject()).toJson(getJsonFormat())
    );
}


void TabCommonUnit::on_checkBox_stateChanged(int arg1)
{
    // 更新ret的Json格式
    ui->textEdit_ret->setText(QJsonDocument::fromJson(ui->textEdit_ret->toPlainText().toUtf8()).toJson(getJsonFormat()));
}

void TabCommonUnit::update(QVariantMap ret)
{
    ui->textEdit_ret->setText(QJsonDocument::fromVariant(ret).toJson(getJsonFormat()));
}

void TabCommonUnit::setFunctions(const QStringList& functions)
{
    ui->comboBox_function->addItems(functions);
}

void TabCommonUnit::updateFunctions()
{
    // 将Api中的方法名称取出
    setFunctions(TestTool::helper()->memberList());
}
