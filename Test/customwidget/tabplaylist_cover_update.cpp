#include "tabplaylist_cover_update.h"
#include "ui_tabplaylist_cover_update.h"
#include "../servicelocator.h"

#include <QFileDialog>
#include <QFileInfo>

TabPlaylist_cover_update::TabPlaylist_cover_update(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TabPlaylist_cover_update)
{
    ui->setupUi(this);
}

TabPlaylist_cover_update::~TabPlaylist_cover_update()
{
    delete ui;
}

void TabPlaylist_cover_update::on_pushButton_select_file_clicked()
{
    QFileInfo fileInfo = QFileInfo(QFileDialog::getOpenFileName(this));
    ui->label_path->setProperty("filePath", fileInfo.filePath());
    ui->label_path->setText(fileInfo.fileName());
}


void TabPlaylist_cover_update::on_pushButton_playlist_cover_update_clicked()
{
    auto filePath = ui->label_path->property("filePath").toString();
    QFile file(filePath);
    file.open(QFile::ReadOnly);
    QImage image(filePath);
    int imgSize = 300;
    if (!image.isNull()) {
        // 默认以图片宽度为上传尺寸
        imgSize = image.width();
    }
    auto data = file.readAll();
    auto arg = QVariantMap {
        { "id", ui->lineEdit_id->text() },
        { "imgSize", imgSize },
        { "imgFile", QVariantMap {
                           { "name", ui->label_path->text() },
                           { "data", data }
                       }
        }
    };
    QVariantMap ret = ServiceLocator::helper().invoke("playlist_cover_update", arg);
    ui->textEdit_ret->setText(QJsonDocument::fromVariant(ret).toJson(QJsonDocument::Indented));
}

