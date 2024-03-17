#include "../servicelocator.h"
#include "tabapitest.h"
#include "ui_tabapitest.h"
#include "../../QCloudMusicApi/module.h"

#include <QMutex>
#include <QJsonDocument>
#include <QDebug>
#include <QtConcurrent>

TabApiTest::TabApiTest(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TabApiTest)
{
    ui->setupUi(this);
}

TabApiTest::~TabApiTest()
{
    delete ui;
}

void TabApiTest::setFunctions(QStringList functions)
{
    ui->textEdit_arg->setText(functions.join("\n"));
}

void TabApiTest::on_pushButton_tabApiTest_send_clicked() {
    QStringList functions = ui->textEdit_arg->toPlainText().split("\n");
    ui->textEdit_ret->clear();
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
        QVariantMap arg = ServiceLocator::config()[function].toObject().toVariantMap();
        QVariantMap ret = invoke(function, arg);

        mutex.lock();
        rets[function] = QJsonDocument::fromVariant(ret).toJson(QJsonDocument::Compact);
        mutex.unlock();
    }).waitForFinished();
    ui->textEdit_ret->setText(QJsonDocument::fromVariant(rets).toJson(QJsonDocument::Indented));
}

