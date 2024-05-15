#include "mainwindow.h"
#include "servicelocator.h"

#include <QApplication>
#include <QLoggingCategory>

int main(int argc, char *argv[])
{
    qSetMessagePattern("%{time yyyy-MM-dd hh:mm:ss.zzz} : %{pid} : %{category} : %{type} : %{line} : %{function} : %{message}");
    ServiceLocator::init();

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
