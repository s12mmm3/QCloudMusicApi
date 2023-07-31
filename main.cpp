#include <QCoreApplication>
#include <QJsonDocument>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QTimer>
#include "crypto.hpp"

extern "C" {
#include <openssl/rc4.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
}

QString getLrc(QString source) {
    auto doc = QJsonDocument::fromJson(source.toUtf8());
    return doc["lrc"]["lyric"].toString();
}

/**
 * @brief 发送一个HTTP GET请求，并返回响应内容
 * @param request 要发送的请求
 * @return 如果请求成功，返回响应内容；如果请求失败或超时，返回空字节数组
 */
QByteArray get(QNetworkRequest request) {
    QByteArray result; // 定义一个空字节数组作为结果
    result.clear();
    // 设置超时处理定时器
    QTimer timer;
    timer.setInterval(10000);  // 设置超时时间 10 秒
    timer.setSingleShot(true);  // 单次触发

    // 设置网络访问管理器
    QNetworkAccessManager* manager = new QNetworkAccessManager();

    // 开启一个局部的事件循环，等待响应结束，退出
    QNetworkReply* reply = manager->get(request); // 发送GET请求
    QEventLoop eventLoop;
    QObject::connect(&timer, &QTimer::timeout, &eventLoop, &QEventLoop::quit); // 定时器超时时退出事件循环
    QObject::connect(manager, &QNetworkAccessManager::finished, &eventLoop, &QEventLoop::quit); // 请求结束时退出事件循环
    timer.start(); // 启动定时器
    eventLoop.exec(); // 启动事件循环

    if(timer.isActive()) { // 处理响应，定时器激活状态
        timer.stop(); // 停止定时器
        if(reply->error() != QNetworkReply::NoError) { // http请求出错，进行错误处理
            qDebug() << "http请求出错 : " << reply->errorString();
                                                     reply->deleteLater();
            return result;
        }
        else {
            // http - 响应状态码
            int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            qDebug() << "服务器返回的Code : " << statusCode;
                if(statusCode == 200) { // http请求响应正常
                result = reply->readAll(); // 读取响应内容
                return result;
            }
            else {
                reply->deleteLater();
                return result;
            }
        }
    }
    else { // 超时处理
        QObject::disconnect(manager, &QNetworkAccessManager::finished, &eventLoop, &QEventLoop::quit); // 断开连接
        reply->abort(); // 中止请求
        qDebug() << "http请求超时 ";
        return result;
    }
    reply->deleteLater();
    qDebug() << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toString();
    return result;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    auto buffer = QByteArray::fromStdString(
        R"(/api/song/enhance/player/url/v1-36cd479b6b5-{"ids":"[33894312]","level":"exhigh","encodeType":"flac","header":{"appver":"8.10.05","versioncode":"140","buildver":"1690802117","resolution":"1920x1080","__csrf":"","os":"android","requestId":"1690802117125_0513","MUSIC_A":"bf8bfeabb1aa84f9c8c3906c04a04fb864322804c83f5d607e91a04eae463c9436bd1a17ec353cf780b396507a3f7464e8a60f4bbc019437993166e004087dd32d1490298caf655c2353e58daa0bc13cc7d5c198250968580b12c1b8817e3f5c807e650dd04abd3fb8130b7ae43fcc5b"}}-36cd479b6b5-ce3152d271ad1b582b16631d24cbcbf5)"
        );
    QString encrypt = Crypto::aesEncrypt(buffer, EVP_aes_128_ecb, Crypto::eapiKey, QString());
    qDebug() << encrypt;
    qDebug() << Crypto::aesDecrypt(encrypt, EVP_aes_128_ecb, Crypto::eapiKey, QString());

    return a.exec();
}
//    QNetworkRequest request; // 创建一个网络请求对象
//    QUrl qUrl("http://music.163.com/api/song/lyric?os=pc&id=2065967510&lv=-1&kv=-1&tv=-1"); // 创建一个URL对象
//    request.setHeader(QNetworkRequest::UserAgentHeader,
//                      "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko)"
//                      " Chrome/106.0.0.0 Safari/537.36 Edg/106.0.1370.34"); // 设置请求头中的用户代理字段
//    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json;charset=UTF-8"); // 设置请求头中的内容类型字段
//    request.setUrl(qUrl); // 设置请求的URL
//    qDebug() << get(request); // 调用get函数发送请求，并打印返回结果
