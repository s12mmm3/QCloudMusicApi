#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMetaMethod>
#include <QTextEdit>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>

#include "../QCloudMusicApi/module.h"
#include "../QCloudMusicApi/util/index.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    for(int i = QObject().metaObject()->methodCount(); i < api.metaObject()->methodCount(); i++) {
        ui->comboBox->addItem(api.metaObject()->method(i).name());
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateCookie(QVariantMap ret) {
    auto newMap = Index::stringToMap(ret["cookie"].toString());
    auto map = Index::stringToMap(cookie);
    if (ret["body"].toMap()["token"].isValid()) {
        map["MUSIC_A"] = ret["body"].toMap()["token"];
    }
    Index::mergeMap(map, newMap);
    cookie = Index::mapToString(map);
}

QVariantMap MainWindow::invoke(QString funName, QVariantMap arg) {
    arg["cookie"] = arg.value("cookie", cookie);
    cookie = arg["cookie"].toString() + " SameSite=None; Secure";
    QVariantMap ret;
    QMetaObject::invokeMethod(&api, funName.toUtf8()
                              , Qt::DirectConnection
                              , Q_RETURN_ARG(QVariantMap, ret)
                              , Q_ARG(QVariantMap, arg));
    this->updateCookie(ret);
    return ret;
}

void MainWindow::on_pushButton_clicked()
{
    ui->textEdit_2->setText(QJsonDocument::fromJson(ui->textEdit_2->toPlainText().toUtf8()).toJson(QJsonDocument::Indented));
    QVariantMap arg = QJsonDocument::fromJson(ui->textEdit_2->toPlainText().toUtf8()).toVariant().toMap();
    QVariantMap ret = this->invoke(ui->comboBox->currentText(), arg);
    if(ui->checkBox->isChecked()) {
        ui->textEdit->setText(QJsonDocument::fromVariant(ret).toJson(QJsonDocument::Indented));
    }
    else {
        ui->textEdit->setText(QJsonDocument::fromVariant(ret).toJson(QJsonDocument::Compact));
    }
}


void MainWindow::on_comboBox_currentTextChanged(const QString &arg1)
{
    QFile file(":/config.json");
    file.open(QIODevice::ReadOnly);
    auto config = QJsonDocument::fromJson(file.readAll());
    ui->textEdit_2->setText(
        QJsonDocument(config[arg1].toObject())
            .toJson(QJsonDocument::Indented)
        );
}


void MainWindow::on_checkBox_stateChanged(int arg1)
{
    if(arg1) {
        ui->textEdit->setText(QJsonDocument::fromJson(ui->textEdit->toPlainText().toUtf8()).toJson(QJsonDocument::Indented));
    }
    else {
        ui->textEdit->setText(QJsonDocument::fromJson(ui->textEdit->toPlainText().toUtf8()).toJson(QJsonDocument::Compact));
    }
}

