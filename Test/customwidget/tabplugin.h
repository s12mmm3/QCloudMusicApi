#ifndef TABPLUGIN_H
#define TABPLUGIN_H

#include <QWidget>

#include "../../QCloudMusicApi/apihelper.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class TabPlugin;
}
QT_END_NAMESPACE

// 插件测试
class TabPlugin : public QWidget
{
    Q_OBJECT

public:
    explicit TabPlugin(QWidget *parent = nullptr);
    ~TabPlugin();

private slots:
    void on_pushButton_select_clicked();

    void on_pushButton_unload_clicked();

    void on_pushButton_load_clicked();

private:
    bool libraryLoad(QString fileName);
    bool libraryUnload(QString fileName);

    void libarayLoadSucceed();
    void libraryLoadFailed();

    void libarayUnloadSucceed();
    void libraryUnloadFailed();

private:
    Ui::TabPlugin *ui;
    ApiHelper helper;
};

#endif // TABPLUGIN_H
