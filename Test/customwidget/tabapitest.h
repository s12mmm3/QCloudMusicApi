#ifndef TABAPITEST_H
#define TABAPITEST_H

#include <QJsonDocument>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class TabApiTest;
}
QT_END_NAMESPACE

// 三种加密方式测试
class TabApiTest : public QWidget
{
    Q_OBJECT

public:
    explicit TabApiTest(QWidget *parent = nullptr);
    ~TabApiTest();
    void setFunctions(QStringList functions);

private slots:
    void on_pushButton_tabApiTest_send_clicked();

private:
    Ui::TabApiTest *ui;
};

#endif // TABAPITEST_H
