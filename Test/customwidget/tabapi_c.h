#ifndef TABAPI_C_H
#define TABAPI_C_H

#include <QWidget>
#include <QJsonDocument>
#include <QVariantMap>
#include <QLibrary>

QT_BEGIN_NAMESPACE
namespace Ui {
class TabApi_c;
}
QT_END_NAMESPACE

// Api动态库调用C接口测试
class TabApi_c : public QWidget
{
    Q_OBJECT

public:
    explicit TabApi_c(QWidget *parent = nullptr);
    ~TabApi_c();

private slots:
    void on_pushButton_send_clicked();

    void on_comboBox_currentTextChanged(const QString &arg1);

    void on_checkBox_stateChanged(int arg1);

    void update(QVariantMap ret);

    void on_pushButton_select_clicked();

private:
    Ui::TabApi_c *ui;
    QLibrary library;
};

#endif // TABAPI_C_H
