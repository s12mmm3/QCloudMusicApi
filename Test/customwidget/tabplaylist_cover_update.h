#ifndef TABPLAYLIST_COVER_UPDATE_H
#define TABPLAYLIST_COVER_UPDATE_H

#include <QWidget>

namespace Ui {
class TabPlaylist_cover_update;
}

// 歌单封面上传测试
class TabPlaylist_cover_update : public QWidget
{
    Q_OBJECT

public:
    explicit TabPlaylist_cover_update(QWidget *parent = nullptr);
    ~TabPlaylist_cover_update();

private slots:
    void on_pushButton_select_file_clicked();

    void on_pushButton_playlist_cover_update_clicked();

private:
    Ui::TabPlaylist_cover_update *ui;
};

#endif // TABPLAYLIST_COVER_UPDATE_H
