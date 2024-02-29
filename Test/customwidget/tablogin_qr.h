#ifndef TABLOGIN_QR_H
#define TABLOGIN_QR_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class TabLogin_qr;
}
QT_END_NAMESPACE

class TabLogin_qr : public QWidget
{
    Q_OBJECT

public:
    explicit TabLogin_qr(QWidget *parent = nullptr);
    ~TabLogin_qr();

private:

    /**
     * @brief 反射调用API中的方法
     * @param funName 函数名称
     * @param arg 参数
     * @return QVariantMap 返回的数据
     */
    QVariantMap invoke(const QString funName, const QVariantMap arg);

    QPixmap generateQRCode(QString strUrl, qint32 temp_width, qint32 temp_height, int offset);

private slots:
    void on_pushButton_login_qr_create_clicked();

    void on_pushButton_login_qr_check_clicked();

    void on_pushButton_login_qr_key_clicked();

private:
    Ui::TabLogin_qr *ui;
};

#endif // TABLOGIN_QR_H
