#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <string>

#include "../servicelocator.h"
#include "tabapi_c.h"
#include "ui_tabapi_c.h"
#include "../logger.h"

TabApi_c::TabApi_c(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TabApi_c)
{
    ui->setupUi(this);

    ui->tabCommonUnit->callback = [this](QString member, QString arg) -> QVariantMap {
        QVariantMap ret;
        // 调用动态库的C接口invoke
        using Invoke = const char* (*)(const char* memberName, const char* value);
        Invoke invoke = (Invoke)library.resolve("invoke");
        if (invoke) {
            ret = (QJsonDocument::fromJson(invoke(member.toStdString().c_str(), arg.toStdString().c_str())).toVariant().toMap());
        } else {
            libraryLoadFailed();
        }
        return ret;
    };
}

TabApi_c::~TabApi_c()
{
    delete ui;
    library.unload();
}

void TabApi_c::on_pushButton_select_clicked()
{
    auto fileName = QFileDialog::getOpenFileName(this);
    libraryLoad(fileName);
}

void TabApi_c::libarayLoadSucceed()
{
    QMessageBox::information(this, "", "Library load succeed!");
}

void TabApi_c::libraryLoadFailed()
{
    QMessageBox::warning(this, "Library load failed!", library.errorString());
}

void TabApi_c::libarayUnloadSucceed()
{
    QMessageBox::information(this, "", "Library unload succeed!");
}

void TabApi_c::libraryUnloadFailed()
{
    QMessageBox::warning(this, "Library unload failed!", library.errorString());
}


void TabApi_c::on_pushButton_unload_clicked()
{
    libraryUnload("");
}


void TabApi_c::on_pushButton_load_clicked()
{
    libraryLoad(ui->lineEdit_library_fileName->text());
}

bool TabApi_c::libraryLoad(QString fileName)
{
    bool result = false;
    if (!fileName.isEmpty()) {
        library.unload();
        library.setFileName(fileName);
        result = library.load();
        if (result) {
            ui->lineEdit_library_fileName->setText(fileName);
            libarayLoadSucceed();

            // 获取API中成员函数的数量
            using MemberCount = int (*)();
            MemberCount memberCount = (MemberCount)library.resolve("memberCount");
            int count = 0;
            if (memberCount) {
                count = memberCount();
            }

            // 通过索引检索成员的名称
            using MemberName = char* (*)(int index);
            MemberName memberName = (MemberName)library.resolve("memberName");
            if (memberName) {
                for (int i = 0; i < count; i++) {
                    DEBUG << i << memberName(i);
                }
            }
        }
        else {
            libraryLoadFailed();
        }
    }
    return result;
}

bool TabApi_c::libraryUnload(QString fileName)
{
    bool result = false;
    Q_UNUSED(fileName)
    if (library.unload()) {
        libarayUnloadSucceed();
        result = true;
    }
    else {
        libraryUnloadFailed();
    }
    return result;
}

