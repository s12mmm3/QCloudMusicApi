#include "tablogin_qr.h"
#include "ui_tablogin_qr.h"

#include "../../QCloudMusicApi/apihelper.h"

#include "../libqrencode/qrencode.h"

#include <QJsonDocument>
#include <QLabel>
#include <QPainter>

TabLogin_qr::TabLogin_qr(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TabLogin_qr)
{
    ui->setupUi(this);
}

TabLogin_qr::~TabLogin_qr()
{
    delete ui;
}
QPixmap TabLogin_qr::generateQRCode(QString strUrl, qint32 temp_width, qint32 temp_height, int offset) {
    QPixmap mainmap;
    if (strUrl.isEmpty()) {
        return mainmap;
    }
    // 生成 QR 码的位图数组
    QRcode *qrcode = QRcode_encodeString(strUrl.toStdString().c_str(), 2, QRecLevel::QR_ECLEVEL_Q, QRencodeMode::QR_MODE_8, 1);
    // QR 码的宽度
    qint32 qrcode_width = qrcode->width > 0 ? qrcode->width : 1;
    // 缩放比例
    double scale_x = (double)temp_width / (double)qrcode_width;
    double scale_y = (double)temp_height / (double)qrcode_width;

    QImage mainimg = QImage(temp_width + offset * 2, temp_height + offset * 2, QImage::Format_ARGB32);
    QPainter painter(&mainimg);
    // 设置背景色为白色
    painter.setBrush(QColor(Qt::white));
    painter.setPen(Qt::NoPen);
    painter.drawRect(offset, offset, temp_width, temp_height);
    // 设置前景色为黑色
    painter.setBrush(QColor(Qt::black));
    // 绘制黑色方块
    for (qint32 y = 0; y < qrcode_width; y++) {
        for (qint32 x = 0; x < qrcode_width; x++) {
            unsigned char b = qrcode->data[y * qrcode_width + x];
            if (b & 0x01) {
                QRectF r(offset + x * scale_x, offset + y * scale_y, scale_x, scale_y);
                painter.drawRects(&r, 1);
            }
        }
    }
    mainmap = QPixmap::fromImage(mainimg);
    delete qrcode;
    return mainmap;
}

QVariantMap TabLogin_qr::invoke(const QString member, const QVariantMap arg) {
    ApiHelper helper;
    QVariantMap ret = helper.invoke(member, arg);
    return ret;
}

void TabLogin_qr::on_pushButton_login_qr_create_clicked()
{
    QString unikey = ui->lineEdit_qr_key->text();
    QVariantMap ret = invoke("login_qr_create",
                 {
                     { "key", unikey }
                 });
    QString qrurl = ret["body"].toMap()["data"].toMap()["qrurl"].toString();
    int size = qMin(ui->label->width(), ui->label->height());
    ui->label->setPixmap(generateQRCode(qrurl, size, size, 0));
}


void TabLogin_qr::on_pushButton_login_qr_check_clicked()
{
    QString unikey = ui->lineEdit_qr_key->text();
    QVariantMap ret = invoke("login_qr_check",
                             {
                                 { "key", unikey }
                             });
    ui->textEdit_ret->setText(QJsonDocument::fromVariant(ret).toJson(QJsonDocument::Indented));
}


void TabLogin_qr::on_pushButton_login_qr_key_clicked()
{
    QVariantMap ret = invoke("login_qr_key", {});
    QString unikey = ret["body"].toMap()["data"].toMap()["unikey"].toString();
    ui->lineEdit_qr_key->setText(unikey);
}

