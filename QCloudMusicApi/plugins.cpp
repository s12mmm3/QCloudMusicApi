#include "plugins.h"

#include "util/request.h"

#include <QEventLoop>
#include <QJsonDocument>
#include <QNetworkReply>
#include <QRegularExpression>

const static auto &request = QCloudMusicApi::Request::createRequest;
const static auto &POST = QNetworkAccessManager::PostOperation;
const static auto &GET = QNetworkAccessManager::GetOperation;

using namespace QCloudMusicApi;
Plugins::Plugins(QObject *parent)
    : QObject{parent}
{}

QVariantMap Plugins::songUpload(QVariantMap query)
{
    QString ext = "mp3";
    if (query.value("songFile").toMap()["name"].toString().toLower().indexOf("flac") > -1) {
        ext = "flac";
    }
    QString filename = query.value("songFile").toMap()["name"].toString()
                           .replace("." + ext, "")
                           .replace(QRegularExpression("\\s"), "")
                           .replace(QRegularExpression("\\."), "_");
    const QString bucket = "jd-musicrep-privatecloud-audio-public";
    //   获取key和token
    const auto tokenRes = request(
        POST,
        "https://music.163.com/weapi/nos/token/alloc",
        {
            { "bucket", bucket },
            { "ext", ext },
            { "filename", filename },
            { "local", false },
            { "nos_product", 3 },
            { "type", "audio" },
            { "md5", query["songFile"].toMap()["md5"] },
        },
        {
            { "crypto", "weapi" },
            { "cookie", query["cookie"] },
            { "ua", query.value("ua", "") },
            { "proxy", query["proxy"] },
        }
        );
    const auto objectKey = tokenRes["body"].toMap()["result"].toMap()["objectKey"].toString().replace("/", "%2F");

    auto reply = Request::axios(
        QNetworkAccessManager::GetOperation,
        "https://wanproxy.127.net/lbs?version=1.0&bucketname=" + bucket,
        {}, {}, "");
    reply->manager()->deleteLater();
    const QVariantMap lbs = QJsonDocument::fromJson(reply->readAll()).toVariant().toMap();
    reply = Request::axios(
        QNetworkAccessManager::PostOperation,
        "http://" + lbs["upload"].toList().value(0).toString() + "/" + bucket + "/" + objectKey + "?offset=0&complete=true&version=1.0",
        {},
        {
            { "x-nos-token", tokenRes["body"].toMap()["result"].toMap()["token"] },
            { "Content-MD5", query["songFile"].toMap()["md5"] },
            { "Content-Type", "audio/mpeg" },
            { "Content-Length", query["songFile"].toMap()["size"] },
        },
        query["songFile"].toMap()["data"].toByteArray());
    reply->manager()->deleteLater();

    return tokenRes;
}

QVariantMap Plugins::upload(QVariantMap query)
{
    const QVariantMap data {
        { "bucket", "yyimgs" },
        { "ext", "jpg" },
        { "filename", query["imgFile"].toMap()["name"] },
        { "local", false },
        { "nos_product", 0 },
        { "return_body", "{\"code\":200,\"size\":\"$(ObjectSize)\"}" },
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
            { "ua", query.value("ua", "") },
            { "proxy", query["proxy"] },
        }
        );
    auto reply = Request::axios(QNetworkAccessManager::PostOperation,
                                "https://nosup-hz1.127.net/yyimgs/"
                                    + res["body"].toMap()["result"].toMap()["objectKey"].toString()
                                    + "?offset=0&complete=true&version=1.0",
                                {},
                                {
                                    { "x-nos-token", res["body"].toMap()["result"].toMap()["token"] },
                                    { "Content-Type", "image/jpeg" }
                                },
                                query["imgFile"].toMap()["data"].toByteArray());
    reply->manager()->deleteLater();

    // 读取响应内容
    auto body = reply->readAll();
    qDebug().noquote() << body.isEmpty() << "body" << body;

    QVariantMap res2 = QJsonDocument::fromJson(body).toVariant().toMap();

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
            { "ua", query.value("ua", "") },
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
