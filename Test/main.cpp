#include "mainwindow.h"
#include "../QCloudMusicApi/util/crypto.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    []() {//测试aes加解密
        QString body = "This is a text";
        auto bodyEncrypt = Crypto::aesEncrypt(body.toUtf8(), EVP_aes_128_ecb, Crypto::eapiKey, "");
        auto bodyDecrypt = Crypto::aesDecrypt(bodyEncrypt, EVP_aes_128_ecb, Crypto::eapiKey, "");
        qDebug() << body;
        qDebug() << bodyEncrypt;
        qDebug() << bodyDecrypt;
        qDebug() << (body == bodyDecrypt);
    }();
    MainWindow w;
    w.show();
    return a.exec();
}
