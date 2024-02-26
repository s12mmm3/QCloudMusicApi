#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->tabApiTest->setFunctions({
        "banner",
        "lyric",
        "toplist"
    });
    ui->tabEapiTest->setFunctions({
        "lyric_new",
        "song_download_url",
        "song_url_v1"
    });
    ui->tabWeapiTest->setFunctions({
        "album",
        "artist_detail",
        "user_detail"
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

