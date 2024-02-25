#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QJsonDocument>
#include <QMainWindow>
#include <QObject>
#include <QTextEdit>

#include "../QCloudMusicApi/module.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_send_clicked();

    void on_comboBox_currentTextChanged(const QString &arg1);

    void on_checkBox_stateChanged(int arg1);

private:

    /**
     * @brief 反射调用API中的方法
     * @param funName 函数名称
     * @param arg 参数
     * @return QVariantMap 返回的数据
     */
    QVariantMap invoke(const QString funName, const QVariantMap arg);

    /**
     * @brief 更新储存的cookie
     * @param ret Api返回的数据
     * @return void
     */
    void updateCookie(const QVariantMap ret);

    Ui::MainWindow *ui;
    NeteaseCloudMusicApi api;
    QVariantMap cookie;
    QJsonDocument config;
};
#endif // MAINWINDOW_H
