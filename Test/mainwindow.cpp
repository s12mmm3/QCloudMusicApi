#include <QMetaMethod>
#include <QTextEdit>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QDebug>

#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "../QCloudMusicApi/module.h"
#include "../QCloudMusicApi/util/index.h"
#include "../QCloudMusicApi/util/crypto.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //将Api中的方法名称取出
    for(int i = QObject().metaObject()->methodCount(); i < api.metaObject()->methodCount(); i++) {
        ui->comboBox->addItem(api.metaObject()->method(i).name());
    }

    //读取配置
    QFile file(":/config.json");
    file.open(QIODevice::ReadOnly);
    config = QJsonDocument::fromJson(file.readAll());

    []() {//测试aes加解密
        QString body = "This is a text";
        auto bodyEncrypt = Crypto::aesEncrypt(body.toUtf8(), EVP_aes_128_ecb, Crypto::eapiKey, "");
        auto bodyDecrypt = Crypto::aesDecrypt(bodyEncrypt, EVP_aes_128_ecb, Crypto::eapiKey, "");
        qDebug() << body;
        qDebug() << bodyEncrypt;
        qDebug() << bodyDecrypt;
        qDebug() << (body == bodyDecrypt);
    }();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateCookie(const QVariantMap ret) {
    auto newMap = Index::stringToMap(ret["cookie"].toString());
    cookie = Index::mergeMap(cookie, newMap);;
    if (ret["body"].toMap()["token"].isValid()) {
        cookie["MUSIC_A"] = ret["body"].toMap()["token"];
    }
}

QVariantMap MainWindow::invoke(const QString funName, const QVariantMap arg) {
    QVariantMap ret;
    QMetaObject::invokeMethod(&api, funName.toUtf8()
                              , Qt::DirectConnection
                              , Q_RETURN_ARG(QVariantMap, ret)
                              , Q_ARG(QVariantMap, arg));
    return ret;
}

void MainWindow::on_pushButton_clicked()
{
    auto JsonFormat = ui->checkBox->isChecked() ? QJsonDocument::Indented : QJsonDocument::Compact;
    ui->textEdit_2->setText(QJsonDocument::fromJson(ui->textEdit_2->toPlainText().toUtf8()).toJson(JsonFormat));

    QVariantMap arg = QJsonDocument::fromJson(ui->textEdit_2->toPlainText().toUtf8()).toVariant().toMap();

    //Api只能处理map类型的cookie
    if(arg.contains("cookie")) {
        //如果传入新的cookie，替换原有的cookie
        if(arg["cookie"].userType() == QMetaType::QVariantMap) {
            cookie = arg["cookie"].toMap()/* + " SameSite=None; Secure"*/;
        }
        else if(arg["cookie"].userType() == QMetaType::QString) {
            cookie = Index::stringToMap(arg["cookie"].toString());
        }
    }
    else {
        //使用存储的cookie
        arg["cookie"] = arg.value("cookie", cookie);
    }

    QVariantMap ret = this->invoke(ui->comboBox->currentText(), arg);

    this->updateCookie(ret);

    if(ui->checkBox->isChecked()) {
        ui->textEdit->setText(QJsonDocument::fromVariant(ret).toJson(QJsonDocument::Indented));
    }
    else {
        ui->textEdit->setText(QJsonDocument::fromVariant(ret).toJson(QJsonDocument::Compact));
    }
}


void MainWindow::on_comboBox_currentTextChanged(const QString &arg1)
{
    auto JsonFormat = ui->checkBox->isChecked() ? QJsonDocument::Indented : QJsonDocument::Compact;
    ui->textEdit_2->setText(
        QJsonDocument(config[arg1].toObject())
            .toJson(JsonFormat)
        );
}


void MainWindow::on_checkBox_stateChanged(int arg1)
{
    auto JsonFormat = arg1 ? QJsonDocument::Indented : QJsonDocument::Compact;
    ui->textEdit->setText(QJsonDocument::fromJson(ui->textEdit->toPlainText().toUtf8()).toJson(JsonFormat));
}

