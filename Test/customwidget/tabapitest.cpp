#include "../servicelocator.h"
#include "tabapitest.h"
#include "ui_tabapitest.h"
#include "../../QCloudMusicApi/apihelper.h"

#include <QMutex>
#include <QJsonDocument>
#include <QJsonObject>
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
    QStringList members = ui->textEdit_arg->toPlainText().split("\n");
    ui->textEdit_ret->clear();
    QVariantMap rets;
    QMutex mutex;
    QtConcurrent::map(members, [&](auto member) {
        QVariantMap arg = ServiceLocator::config().object()[member].toObject().toVariantMap();
        QVariantMap ret = ServiceLocator::helper().invoke(member, arg);

        mutex.lock();
        rets[member] = QJsonDocument::fromVariant(ret).toJson(QJsonDocument::Compact);
        mutex.unlock();
    }).waitForFinished();
    ui->textEdit_ret->setText(QJsonDocument::fromVariant(rets).toJson(QJsonDocument::Indented));
}

