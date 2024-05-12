#ifndef TABSERVERTEST_H
#define TABSERVERTEST_H

#include <QWidget>

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
    void setUrl();

private:
    Ui::TabServerTest *ui;
};

#endif // TABSERVERTEST_H
