#include <QMetaMethod>
#include <QTextEdit>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QDebug>
#include <QtConcurrent>

#include "tabcommon.h"
#include "ui_tabcommon.h"


#include "../../QCloudMusicApi/module.h"
#include "../../QCloudMusicApi/util/index.h"

TabCommon::TabCommon(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TabCommon)
{
    ui->setupUi(this);
    //将Api中的方法名称取出
    NeteaseCloudMusicApi api;
    for(int i = QObject().metaObject()->methodCount(); i < api.metaObject()->methodCount(); i++) {
        ui->comboBox->addItem(api.metaObject()->method(i).name());
    }

    //读取配置
    QFile file(":/config.json");
    file.open(QIODevice::ReadOnly);
    config = QJsonDocument::fromJson(file.readAll());
}

TabCommon::~TabCommon()
{
    delete ui;
}

void TabCommon::updateCookie(const QVariantMap ret) {
    auto newCookie = Index::stringToMap(ret["cookie"].toString());
    if (!newCookie.isEmpty()) {
        cookie = Index::mergeMap(cookie, newCookie);
    }
    auto token = ret["body"].toMap()["token"].toString();
    if (!token.isEmpty()) {
        cookie["MUSIC_A"] = token;
    }
}

QVariantMap TabCommon::invoke(const QString funName, const QVariantMap arg) {
    NeteaseCloudMusicApi api;
    QVariantMap ret;
    QMetaObject::invokeMethod(&api, funName.toUtf8()
                              , Qt::DirectConnection
                              , Q_RETURN_ARG(QVariantMap, ret)
                              , Q_ARG(QVariantMap, arg));
    return ret;
}

void TabCommon::on_pushButton_send_clicked()
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


void TabCommon::on_comboBox_currentTextChanged(const QString &arg1)
{
    auto JsonFormat = ui->checkBox->isChecked() ? QJsonDocument::Indented : QJsonDocument::Compact;
    ui->textEdit_arg->setText(
        QJsonDocument(config[arg1].toObject())
            .toJson(JsonFormat)
        );
}


void TabCommon::on_checkBox_stateChanged(int arg1)
{
    auto JsonFormat = arg1 ? QJsonDocument::Indented : QJsonDocument::Compact;
    ui->textEdit_ret->setText(QJsonDocument::fromJson(ui->textEdit_ret->toPlainText().toUtf8()).toJson(JsonFormat));
}
