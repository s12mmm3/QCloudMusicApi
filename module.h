#include <iostream>
#include <QJsonDocument>
#include <QNetworkRequest>
#include <QNetworkAccessManager>

#include "crypto.hpp"
#include "request.hpp"
namespace Module {
const void song_url_v1(QVariantMap query) {
    const QVariantMap data = {
        { "ids", query["id"].toList() },
        { "level", query["level"].toString() },
        { "encodeType", "flac" }
    };
    if(data["level"].toString() == "sky") {
        data["immerseType"] = "c51";
    }
    qDebug().noquote() << data;
    createRequest(
        QNetworkAccessManager::PostOperation,
        QUrl("https://interface.music.163.com/eapi/song/enhance/player/url/v1"),
        data,
        QVariantMap({
            { "crypto", "eapi" },
            { "cookie", query["cookie"] },
            { "proxy", query["proxy"] },
            { "realIP", query["realIP"] },
            { "url", "/api/song/enhance/player/url/v1" }
        })
    );
}
}
