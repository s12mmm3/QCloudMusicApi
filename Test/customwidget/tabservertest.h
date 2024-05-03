#ifndef TABSERVERTEST_H
#define TABSERVERTEST_H

#include "../../QCloudMusicApi/apihelper.h"

#include <QWidget>
#include <QJsonDocument>
#include <QVariantMap>

QT_BEGIN_NAMESPACE
namespace Ui {
class TabServerTest;
}
QT_END_NAMESPACE

// 测试服务器Api
class TabServerTest : public QWidget
{
    Q_OBJECT

public:
    explicit TabServerTest(QWidget *parent = nullptr);
    ~TabServerTest();

private slots:
    void on_pushButton_send_clicked();

    void on_comboBox_currentTextChanged(const QString &arg1);

    void on_checkBox_stateChanged(int arg1);

    void update(QVariantMap ret);

    void setUrl();

private:
    Ui::TabServerTest *ui;
    ApiHelper helper;
};

#endif // TABSERVERTEST_H
