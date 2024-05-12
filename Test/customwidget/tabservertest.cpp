#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QDebug>
#include <QtConcurrent>
#include <QUrlQuery>
#include <QNetworkReply>

#include "../logger.h"
#include "../../QCloudMusicApi/util/request.h"
#include "tabservertest.h"
#include "ui_tabservertest.h"

TabServerTest::TabServerTest(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TabServerTest)
{
    ui->setupUi(this);

    ui->tabCommonUnit->callback = [this](QString, QString arg_str) -> QVariantMap {
        QVariantMap ret;

        auto method = ui->checkBox_post->isChecked() ? QNetworkAccessManager::PostOperation : QNetworkAccessManager::GetOperation;
        QString url = ui->lineEdit_url->text();
        if (ui->checkBox_timestamp->isChecked()) {
            QUrl u(url);
            QUrlQuery urlQuery;
            urlQuery.addQueryItem("timestamp", QString::number(QDateTime::currentDateTime().toMSecsSinceEpoch()));
            u.setQuery(urlQuery);
            url = u.toString();
        }
        QVariantMap arg = QJsonDocument::fromJson(arg_str.toUtf8()).toVariant().toMap();

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
        reply->manager()->deleteLater();
        ret = QJsonDocument::fromJson(reply->readAll()).toVariant().toMap();
        DEBUG.noquote() << reply->rawHeaderPairs();

        return ret;
    };

    setUrl();
    connect(ui->lineEdit_address, &QLineEdit::textChanged, this, &TabServerTest::setUrl);
    connect(ui->lineEdit_port, &QLineEdit::textChanged, this, &TabServerTest::setUrl);
    connect(ui->tabCommonUnit->comboBox, &QComboBox::currentTextChanged, this, &TabServerTest::setUrl);
}

TabServerTest::~TabServerTest()
{
    delete ui;
}

void TabServerTest::setUrl()
{
    QUrl url;
    url.setUrl(ui->lineEdit_address->text());
    url.setPath("/" + ui->tabCommonUnit->comboBox->currentText().replace("_", "/"));
    url.setPort(ui->lineEdit_port->text().toInt());
    ui->lineEdit_url->setText(url.toString());
}

