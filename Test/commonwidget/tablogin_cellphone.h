#ifndef TABLOGIN_CELLPHONE_H
#define TABLOGIN_CELLPHONE_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class TabLogin_cellphone;
}
QT_END_NAMESPACE

class TabLogin_cellphone : public QWidget
{
    Q_OBJECT

public:
    explicit TabLogin_cellphone(QWidget *parent = nullptr);
    ~TabLogin_cellphone();

private slots:
    void on_pushButton_send_clicked();

private:

    /**
     * @brief 反射调用API中的方法
     * @param funName 函数名称
     * @param arg 参数
     * @return QVariantMap 返回的数据
     */
    QVariantMap invoke(const QString funName, const QVariantMap arg);

private:
    Ui::TabLogin_cellphone *ui;
};

#endif // TABLOGIN_CELLPHONE_H
