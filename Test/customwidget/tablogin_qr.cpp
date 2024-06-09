#include "tablogin_qr.h"
#include "ui_tablogin_qr.h"

#include "../servicelocator.h"

#include "../libqrencode/qrencode.h"

#include <QJsonDocument>
#include <QLabel>
#include <QPainter>

TabLogin_qr::TabLogin_qr(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::TabLogin_qr)
{
    ui->setupUi(this);

    connect(ui->lineEdit_url, &QLineEdit::textChanged, this, &TabLogin_qr::showQrCode);
}

TabLogin_qr::~TabLogin_qr()
{
    delete ui;
}

QImage TabLogin_qr::generateQRCode(QString url, qint32 width, qint32 height, qint32 offset) {
    QImage image;
    if (url.isEmpty()) {
        return image;
    }
    // 生成 QR 码的位图数组
    QSharedPointer<QRcode> qrcode(QRcode_encodeString(url.toStdString().c_str(), 2, QRecLevel::QR_ECLEVEL_Q, QRencodeMode::QR_MODE_8, 1));


    // QR 码的宽度
    qint32 qrcode_width = qrcode->width > 0 ? qrcode->width : 1;

    qint32 realWidth = width;
    qint32 realHeight = height;
    // 缩放比例
    double scale_x = 1;
    double scale_y = 1;

    if (realWidth > 0) scale_x = (double)realWidth / (double)qrcode_width;
    else realWidth = qrcode_width;
    if (realHeight > 0) scale_y = (double)realHeight / (double)qrcode_width;
    else realHeight = qrcode_width;

    image = QImage(realWidth + offset * 2, realHeight + offset * 2, QImage::Format_ARGB32);
    QPainter painter(&image);
    // 设置背景色为白色
    painter.setBrush(QColor(Qt::white));
    painter.setPen(Qt::NoPen);
    painter.drawRect(offset, offset, realWidth, realHeight);
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
    return image;
}

void TabLogin_qr::showQrCode(QString url)
{
    int size = qMin(ui->label->width(), ui->label->height());
    ui->label->setPixmap(QPixmap::fromImage(generateQRCode(url, size, size)));
}

QVariantMap TabLogin_qr::invoke(const QString member, const QVariantMap arg) {
    QVariantMap ret = ServiceLocator::helper().invoke(member, arg);
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

    ui->lineEdit_url->setText(qrurl);
}


void TabLogin_qr::on_pushButton_login_qr_check_clicked()
{
    QString unikey = ui->lineEdit_qr_key->text();
    QVariantMap ret = invoke("login_qr_check",
        {
            { "key", unikey }
        });
    ui->textEdit_ret->setText(QJsonDocument::fromVariant(ret).toJson(QJsonDocument::Indented));
    auto cookie = ret["cookie"].toString();
    if (!cookie.isEmpty()) {
        ServiceLocator::helper().set_cookie(cookie);
    }
}


void TabLogin_qr::on_pushButton_login_qr_key_clicked()
{
    QVariantMap ret = invoke("login_qr_key", {});
    QString unikey = ret["body"].toMap()["data"].toMap()["unikey"].toString();
    ui->lineEdit_qr_key->setText(unikey);
}

