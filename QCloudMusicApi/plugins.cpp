#include "plugins.h"

#include "util/request.h"

#include <QEventLoop>
#include <QJsonDocument>
#include <QNetworkReply>

const static auto &request = QCloudMusicApi::Request::createRequest;
const static auto &POST = QNetworkAccessManager::PostOperation;
const static auto &GET = QNetworkAccessManager::GetOperation;

using namespace QCloudMusicApi;
Plugins::Plugins(QObject *parent)
    : QObject{parent}
{}

QVariantMap Plugins::upload(QVariantMap query)
{
    const QVariantMap data {
        { "bucket", "yyimgs" },
        { "ext", "jpg" },
        { "filename", query["imgFile"].toMap()["name"] },
        { "local", false },
        { "nos_product", 0 },
        { "return_body", "{\"code\",200,\"size\",\"$(ObjectSize)\"}" },
        { "type", "other" }
    };
    //   获取key和token
    const auto res = request(
        POST,
        "https://music.163.com/weapi/nos/token/alloc",
        data,
        {
         { "crypto", "weapi" },
         { "cookie", query["cookie"] },
         { "proxy", query["proxy"] },
         { "ua", query.value("ua", "") },
         }
        );
    auto reply = Request::axios(QNetworkAccessManager::PostOperation,
                                "https://nosup-hz1.127.net/yyimgs/"
                                    + res["body"].toMap()["result"].toMap()["objectKey"].toString()
                                    + "?offset=0&complete=true&version=1.0",
                                {
                                    { "x-nos-token", res["body"].toMap()["result"].toMap()["token"].toByteArray() },
                                    { "Content-Type", "image/jpeg" }
                                },
                                query["imgFile"].toMap());
    reply->deleteLater();
    reply->manager()->deleteLater();

    // 读取响应内容
    auto body = reply->readAll();
    qDebug().noquote() << "body" << body;

    QVariantMap res2 {
        { "status", 500 },
        { "body", {} },
        { "cookie", {} }
    };

    if(!QJsonDocument::fromJson(body).isNull()) res2["body"] = QJsonDocument::fromJson(body).toVariant().toMap();
    else res2["body"] = QString::fromUtf8(body);

    //   获取裁剪后图片的id
    const int imgSize = query.value("imgSize", 300).toInt();
    const int imgX = query.value("imgX", 0).toInt();
    const int imgY = query.value("imgY", 0).toInt();

    const auto res3 = request(
        GET,
        QString("https://music.163.com/upload/img/op?id=%1&op=%2y%3y%4y%5")
            .arg(res["body"].toMap()["result"].toMap()["docId"].toString())
            .arg(imgX)
            .arg(imgY)
            .arg(imgSize)
            .arg(imgSize),
        {},
        {
         { "crypto", "weapi" },
         { "cookie", query["cookie"] },
         { "proxy", query["proxy"] },
         { "ua", query.value("ua", "") },
         }
        );

    return {
            // ...res.body.result,
            // ...res2.data,
            // ...res3.body,
            { "url_pre", "https://p1.music.126.net/" + res["body"].toMap()["result"].toMap()["objectKey"].toString() },
            { "url", res3["body"].toMap()["url"] },
            { "imgId", res3["body"].toMap()["id"] },
            };
}
