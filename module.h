#include <iostream>
#include <QJsonDocument>
#include <QNetworkRequest>
#include <QNetworkAccessManager>

#include "crypto.hpp"
namespace Module {
const void song_url_v1(QVariantMap query, void request(QVariantList)) {
    const QVariantMap data = {
        { "ids", '[' + query["id"].toString() + ']' },
        { "level", query["level"].toString() },
        { "encodeType", "flac" }
    };
    if(data["level"].toString() == "sky") {
        data["immerseType"] = "c51";
    }
    qDebug().noquote() << data;
    request({
        QNetworkAccessManager::PostOperation,
        "https://interface.music.163.com/eapi/song/enhance/player/url/v1",
        data,
        QVariantMap({
            { "crypto", "eapi" },
            { "cookie", query["cookie"].toString() },
            { "proxy", query["proxy"].toString() },
            { "realIP", query["realIP"].toString() },
            { "url", "/api/song/enhance/player/url/v1" }
        })
    });
}
}
