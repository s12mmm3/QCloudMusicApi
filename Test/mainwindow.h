#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "../QCloudMusicApi/module.h"

#include <QMainWindow>
#include <QObject>

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
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    NeteaseCloudMusicApi api;
};
#endif // MAINWINDOW_H
