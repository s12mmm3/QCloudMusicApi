#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMetaMethod>
#include <QTextEdit>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
//#include <QNetworkCookie>

//#include "../QCloudMusicApi/config.h"
#include "../QCloudMusicApi/module.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    for(int i = QObject().metaObject()->methodCount(); i < api.metaObject()->methodCount(); i++) {
        ui->comboBox->addItem(api.metaObject()->method(i).name());
    }

//    auto register_anonimous = api.register_anonimous({});
//    qDebug() << "register_anonimous" << register_anonimous;
//    QString cookieStr = QJsonDocument::fromJson(register_anonimous).toVariant()
//                            .toMap()["body"].toMap()["cookie"].toString();
//    auto cookie = QNetworkCookie(cookieStr.toUtf8());

//    qDebug () << "Name:" << cookie.name ();
//    qDebug () << "Value:" << cookie.value ();
//    Config::anonymous_token = cookie.value ();
//    qDebug () << "Max-Age:" << cookie.expirationDate ().toSecsSinceEpoch () - QDateTime::currentSecsSinceEpoch ();
//    qDebug () << "Expires:" << cookie.expirationDate ();
//    qDebug () << "Path:" << cookie.path ();
//    qDebug () << "Domain:" << cookie.domain ();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    ui->textEdit_2->setText(QJsonDocument::fromJson(ui->textEdit_2->toPlainText().toUtf8()).toJson(QJsonDocument::Indented));
    QByteArray ret;
    QMetaObject::invokeMethod(&api, ui->comboBox->currentText().toUtf8()
                              , Qt::DirectConnection
                              , Q_RETURN_ARG(QByteArray, ret)
                              , Q_ARG(QVariantMap, QJsonDocument::fromJson(ui->textEdit_2->toPlainText().toUtf8()).toVariant().toMap()));
    if(ui->checkBox->isChecked()) {
        ui->textEdit->setText(QJsonDocument::fromJson(ret).toJson(QJsonDocument::Indented));
    }
    else {
        ui->textEdit->setText(QJsonDocument::fromJson(ret).toJson(QJsonDocument::Compact));
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

