#include <QMetaMethod>
#include <QTextEdit>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QDebug>
#include <QtConcurrent>

#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "../QCloudMusicApi/module.h"
#include "../QCloudMusicApi/util/index.h"

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
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateCookie(const QVariantMap ret) {
    auto newCookie = Index::stringToMap(ret["cookie"].toString());
    if (!newCookie.isEmpty()) {
        cookie = Index::mergeMap(cookie, newCookie);
    }
    auto token = ret["body"].toMap()["token"].toString();
    if (!token.isEmpty()) {
        cookie["MUSIC_A"] = token;
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

void MainWindow::on_pushButton_send_clicked()
{
    ui->textEdit_ret->clear();

    auto JsonFormat = ui->checkBox->isChecked() ? QJsonDocument::Indented : QJsonDocument::Compact;
    ui->textEdit_arg->setText(QJsonDocument::fromJson(ui->textEdit_arg->toPlainText().toUtf8()).toJson(JsonFormat));

    QVariantMap arg = QJsonDocument::fromJson(ui->textEdit_arg->toPlainText().toUtf8()).toVariant().toMap();

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
        ui->textEdit_ret->setText(QJsonDocument::fromVariant(ret).toJson(QJsonDocument::Indented));
    }
    else {
        ui->textEdit_ret->setText(QJsonDocument::fromVariant(ret).toJson(QJsonDocument::Compact));
    }
}


void MainWindow::on_comboBox_currentTextChanged(const QString &arg1)
{
    auto JsonFormat = ui->checkBox->isChecked() ? QJsonDocument::Indented : QJsonDocument::Compact;
    ui->textEdit_arg->setText(
        QJsonDocument(config[arg1].toObject())
            .toJson(JsonFormat)
        );
}


void MainWindow::on_checkBox_stateChanged(int arg1)
{
    auto JsonFormat = arg1 ? QJsonDocument::Indented : QJsonDocument::Compact;
    ui->textEdit_ret->setText(QJsonDocument::fromJson(ui->textEdit_ret->toPlainText().toUtf8()).toJson(JsonFormat));
}

void MainWindow::test_send(QTextEdit* textEdit_ret, QStringList functions) {
    textEdit_ret->clear();
    QVariantMap rets;
    QMutex mutex;
    auto invoke = [](const QString funName, const QVariantMap arg) {
        NeteaseCloudMusicApi api;
        QVariantMap ret;
        QMetaObject::invokeMethod(&api, funName.toUtf8()
                                  , Qt::DirectConnection
                                  , Q_RETURN_ARG(QVariantMap, ret)
                                  , Q_ARG(QVariantMap, arg));
        return ret;
    };
    QtConcurrent::map(functions, [&](auto function) {
        QVariantMap arg = config[function].toObject().toVariantMap();
        QVariantMap ret = invoke(function, arg);

        mutex.lock();
        rets[function] = QJsonDocument::fromVariant(ret).toJson(QJsonDocument::Compact);
        mutex.unlock();
    }).waitForFinished();
    textEdit_ret->setText(QJsonDocument::fromVariant(rets).toJson(QJsonDocument::Indented));
}

void MainWindow::on_pushButton_api_test_send_clicked()
{
    test_send(ui->textEdit_api_test_ret,
              {
                  "banner",
                  "lyric",
                  "toplist"
              });
}


void MainWindow::on_pushButton_eapi_test_send_clicked()
{
    test_send(ui->textEdit_eapi_test_ret,
              {
               "lyric_new",
               "song_download_url",
               "song_url_v1"
              });
}


void MainWindow::on_pushButton_weapi_test_send_clicked()
{
    test_send(ui->textEdit_weapi_test_ret,
              {
                  "album",
                  "artist_detail",
                  "user_detail"
              });
}

