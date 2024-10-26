#ifndef TABAPI_C_H
#define TABAPI_C_H

#include <QWidget>
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
    void on_pushButton_select_clicked();

    void on_pushButton_unload_clicked();

    void on_pushButton_load_clicked();

protected:
    virtual bool libraryLoad(QString fileName);
    virtual bool libraryUnload(QString fileName);

    void libarayLoadSucceed();
    void libraryLoadFailed();

    void libarayUnloadSucceed();
    void libraryUnloadFailed();

private:
    QLibrary library;

protected:
    Ui::TabApi_c *ui;
};

#endif // TABAPI_C_H
