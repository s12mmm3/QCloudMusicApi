#ifndef TABLOGIN_CELLPHONE_H
#define TABLOGIN_CELLPHONE_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class TabLogin_cellphone;
}
QT_END_NAMESPACE

// 手机号登录测试
class TabLogin_cellphone : public QWidget
{
    Q_OBJECT

public:
    explicit TabLogin_cellphone(QWidget *parent = nullptr);
    ~TabLogin_cellphone();

private slots:
    void on_pushButton_send_clicked();

private:
    Ui::TabLogin_cellphone *ui;
};

#endif // TABLOGIN_CELLPHONE_H
