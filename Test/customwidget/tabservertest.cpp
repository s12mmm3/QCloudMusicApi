#include <QTextEdit>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QDebug>
#include <QtConcurrent>
#include <QNetworkReply>

#include "../servicelocator.h"
#include "../../QCloudMusicApi/util/request.h"
#include "tabservertest.h"
#include "ui_tabservertest.h"

TabServerTest::TabServerTest(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TabServerTest)
{
    ui->setupUi(this);
    //将Api中的方法名称取出
    ui->comboBox->addItems(ApiHelper::memberList());

    setUrl();
    connect(ui->lineEdit_address, &QLineEdit::textChanged, this, &TabServerTest::setUrl);
    connect(ui->lineEdit_port, &QLineEdit::textChanged, this, &TabServerTest::setUrl);
    connect(ui->comboBox, &QComboBox::currentTextChanged, this, &TabServerTest::setUrl);
}

TabServerTest::~TabServerTest()
{
    delete ui;
}

void TabServerTest::on_pushButton_send_clicked()
{
    ui->textEdit_ret->clear();

    auto JsonFormat = ui->checkBox->isChecked() ? QJsonDocument::Indented : QJsonDocument::Compact;
    ui->textEdit_arg->setText(QJsonDocument::fromJson(ui->textEdit_arg->toPlainText().toUtf8()).toJson(JsonFormat));

    auto method = ui->checkBox_post->isChecked() ? QNetworkAccessManager::PostOperation : QNetworkAccessManager::GetOperation;
    QString url = ui->lineEdit_url->text();
    if (ui->checkBox_timestamp->isChecked()) {
        QUrl u(url);
        QUrlQuery urlQuery;
        urlQuery.addQueryItem("timestamp", QString::number(QDateTime::currentDateTime().toMSecsSinceEpoch()));
        u.setQuery(urlQuery);
        url = u.toString();
    }
    QVariantMap arg = QJsonDocument::fromJson(ui->textEdit_arg->toPlainText().toUtf8()).toVariant().toMap();

    // QtConcurrent::run
    QVariantMap headers;
    // headers["Content-Type"] = "application/x-www-form-urlencoded";
    QUrlQuery query;
    query.setQuery(QUrl(url).query());
    for(auto i = arg.constBegin(); i != arg.constEnd(); ++i) {
        query.addQueryItem(i.key(), i.value().toString());
    }
    auto reply = QCloudMusicApi::Request
        ::axios(method,
                url,
                arg,
                headers,
                query.toString().toUtf8());
    reply->deleteLater();
    reply->manager()->deleteLater();
    auto ret = QJsonDocument::fromJson(reply->readAll()).toVariant().toMap();
    qDebug().noquote() << reply->rawHeaderPairs();

    update(ret);
}


void TabServerTest::on_comboBox_currentTextChanged(const QString &arg1)
{
    auto JsonFormat = ui->checkBox->isChecked() ? QJsonDocument::Indented : QJsonDocument::Compact;
    ui->textEdit_arg->setText(
        QJsonDocument(ServiceLocator::config()[arg1].toObject())
            .toJson(JsonFormat)
        );
}


void TabServerTest::on_checkBox_stateChanged(int arg1)
{
    auto JsonFormat = arg1 ? QJsonDocument::Indented : QJsonDocument::Compact;
    ui->textEdit_ret->setText(QJsonDocument::fromJson(ui->textEdit_ret->toPlainText().toUtf8()).toJson(JsonFormat));
}

void TabServerTest::update(QVariantMap ret)
{
    if(ui->checkBox->isChecked()) {
        ui->textEdit_ret->setText(QJsonDocument::fromVariant(ret).toJson(QJsonDocument::Indented));
    }
    else {
        ui->textEdit_ret->setText(QJsonDocument::fromVariant(ret).toJson(QJsonDocument::Compact));
    }
}

void TabServerTest::setUrl()
{
    QUrl url;
    url.setUrl(ui->lineEdit_address->text());
    url.setPath("/" + ui->comboBox->currentText().replace("_", "/"));
    url.setPort(ui->lineEdit_port->text().toInt());
    ui->lineEdit_url->setText(url.toString());
}

