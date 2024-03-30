#include "mainwindow.h"
#include "servicelocator.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    ServiceLocator::init();

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
