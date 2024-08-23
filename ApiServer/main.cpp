#include <QCoreApplication>
#include <QLoggingCategory>

#include "server.h"
#include "generateconfig.h"

Server server;
void start() {
    // 检测是否存在 anonymous_token 文件,没有则生成
    QFile file(QDir(tmpPath).absoluteFilePath("anonymous_token"));
    if (!file.exists()) {
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        file.close();
    }
    // 启动时更新anonymous_token
    generateConfig();
    server.serveNcmApi({
        { "checkVersion", true }
        });
}

int main(int argc, char* argv[])
{
    qSetMessagePattern("%{time yyyy-MM-dd hh:mm:ss.zzz} : %{pid} : %{category} : %{type} : %{line} : %{function} : %{message}");
    QLoggingCategory::setFilterRules("QCloudMusicApi.debug=false");
    QCoreApplication a(argc, argv);
    start();
    return a.exec();
}
