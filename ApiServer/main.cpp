#include <QCoreApplication>

#include "server.h"

int main(int argc, char *argv[])
{
    qSetMessagePattern("%{time yyyy-MM-dd hh:mm:ss.zzz} : %{pid} : %{category} : %{type} : %{line} : %{function} : %{message}");
    QCoreApplication a(argc, argv);
    Server server;
    server.serveNcmApi({

    });
    return a.exec();
}
