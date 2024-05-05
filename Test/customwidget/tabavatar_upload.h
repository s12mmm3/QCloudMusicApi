#ifndef TABAVATAR_UPLOAD_H
#define TABAVATAR_UPLOAD_H

#include <QWidget>

namespace Ui {
class TabAvatar_upload;
}

// 头像上传测试
class TabAvatar_upload : public QWidget
{
    Q_OBJECT

public:
    explicit TabAvatar_upload(QWidget *parent = nullptr);
    ~TabAvatar_upload();

private slots:
    void on_pushButton_select_file_clicked();

    void on_pushButton_send_clicked();

private:
    Ui::TabAvatar_upload *ui;
};

#endif // TABAVATAR_UPLOAD_H
