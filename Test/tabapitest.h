#ifndef TABAPITEST_H
#define TABAPITEST_H

#include <QJsonDocument>
#include <QTextEdit>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class TabApiTest;
}
QT_END_NAMESPACE

class TabApiTest : public QWidget
{
    Q_OBJECT

public:
    explicit TabApiTest(QWidget *parent = nullptr);
    ~TabApiTest();
    void setFunctions(QStringList functions);

private slots:
    void on_pushButton_send_clicked();

    void test_send(QTextEdit* textEdit_ret, QStringList functions);

private:
    Ui::TabApiTest *ui;
    QJsonDocument config;
};

#endif // TABAPITEST_H
