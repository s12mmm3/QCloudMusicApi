#include <QJsonDocument>
#include <QJsonObject>

#include "testtool.h"
#include "tabcommon.h"
#include "ui_tabcommon.h"

TabCommon::TabCommon(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::TabCommon)
{
    ui->setupUi(this);

    // ServiceLocator::helper()->invoke("register_anonimous", {});

    ui->tabCommonUnit->callback = [](QString member, QString arg) -> QVariantMap {
        return TestTool::helper()->invoke(member, QJsonDocument::fromJson(arg.toUtf8()).toVariant().toMap());
        };
}

TabCommon::~TabCommon()
{
    delete ui;
}
