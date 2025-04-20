#include "mainwindow.h"
#include "testtool.h"

#include <QApplication>
#include <QLoggingCategory>

int main(int argc, char *argv[])
{
    qSetMessagePattern("%{time yyyy-MM-dd hh:mm:ss.zzz} : %{threadid} : %{category} : %{type} : %{line} : %{function} : %{message}");
    TestTool::init();

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
