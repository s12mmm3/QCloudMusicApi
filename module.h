#include <iostream>
#include <QJsonDocument>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QJsonArray>

#include "request.hpp"

namespace Module {

// 歌曲链接 - v1
// 此版本不再采用 br 作为音质区分的标准
// 而是采用 standard, exhigh, lossless, hires, jyeffect(高清环绕声), sky(沉浸环绕声), jymaster(超清母带) 进行音质判断
const QByteArray song_url_v1(QVariantMap query) {
    const QVariantMap data = {
        { "ids", query["id"].toList() },
        { "level", query["level"].toString() },
        { "encodeType", "flac" }
    };
    if(data["level"].toString() == "sky") {
        data["immerseType"] = "c51";
    }
    qDebug().noquote() << data;
    return createRequest(
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

// 歌词
const QByteArray lyric(QVariantMap query) {
    QVariantMap cookie = query["cookie"].toMap();
    cookie["os"] = "ios";
    query["cookie"] = cookie;

    const QVariantMap data = {
        { "id", query["id"] },
        { "tv", -1 },
        { "lv", -1 },
        { "rv", -1 },
        { "kv", -1 }
    };
    return createRequest(
        QNetworkAccessManager::PostOperation,
        QUrl("https://music.163.com/api/song/lyric?_nmclfl=1"),
        data,
        QVariantMap({
            { "crypto", "api" },
            { "cookie", query["cookie"] },
            { "proxy", query["proxy"] },
            { "realIP", query["realIP"] }
        })
        );
}

// 新版歌词 - 包含逐字歌词
const QByteArray lyric_new(QVariantMap query) {
    const QVariantMap data = {
        { "id", query["id"] },
        { "cp", false},
        { "tv", 0 },
        { "lv", 0 },
        { "rv", 0 },
        { "kv", 0 },
        { "yv", 0 },
        { "ytv", 0 },
        { "yrv", 0 }
    };
    return createRequest(
        QNetworkAccessManager::PostOperation,
        QUrl("https://interface3.music.163.com/eapi/song/lyric/v1"),
        data,
        QVariantMap({
            { "crypto", "eapi" },
            { "cookie", query["cookie"] },
            { "proxy", query["proxy"] },
            { "realIP", query["realIP"] },
            { "url", "/api/song/lyric/v1" }
        })
        );
}

// 相关歌单
const QByteArray related_playlist(QVariantMap query) {
    QByteArray result = createRequest(
        QNetworkAccessManager::GetOperation,
        QUrl("https://music.163.com/playlist?id=" + query["id"].toString()),
        {},
        QVariantMap({
            { "ua", "pc" },
            { "cookie", query["cookie"] },
            { "proxy", query["proxy"] },
            { "realIP", query["realIP"] }
        })
        );
    QRegularExpression pattern("<div class=\"cver u-cover u-cover-3\">[\\s\\S]*?<img src=\"([^\"]+)\">[\\s\\S]*?<a class=\"sname f-fs1 s-fc0\" href=\"([^\"]+)\"[^>]*>([^<]+?)<\\/a>[\\s\\S]*?<a class=\"nm nm f-thide s-fc3\" href=\"([^\"]+)\"[^>]*>([^<]+?)<\\/a>");
    QRegularExpressionMatchIterator it = pattern.globalMatch(QString::fromUtf8(result));
    QJsonArray playlists;
    while (it.hasNext()) {
        auto result = it.next().capturedTexts();
        playlists.push_back(
            QJsonValue::fromVariant(
                QVariantMap({
                    {
                        "creator", {
                            QVariantMap({
                                { "userId", result[4].sliced(QString("/user/home?id=").length()) },
                                { "nickname", result[5] }
                            })
                        }
                    },
                    {
                        "coverImgUrl", result[1].sliced(0, -QString("?param=50y50").length())
                    },
                    {
                        "name", result[3]
                    },
                    {
                        "id", result[2].sliced(QString("/playlist?id=").length())
                    }
                })
                )
            );
    }
    QVariantMap response = {
        { "code", 200 },
        { "playlists", QVariant(playlists) }
    };
    return QJsonDocument::fromVariant(response).toJson (QJsonDocument::Indented);
}

// 歌曲评论
const QByteArray comment_music(QVariantMap query) {
    QVariantMap cookie = query["cookie"].toMap();
    cookie["os"] = "pc";
    query["cookie"] = cookie;
    const QVariantMap data = {
        { "rid", query["id"] },
        { "limit", query["limit"].isValid() ? query["limit"].toInt() : 20 },
        { "offset", query["offset"].isValid() ? query["offset"].toInt() : 0 },
        { "beforeTime", query["beforeTime"].isValid() ? query["beforeTime"].toInt() : 0 }
    };
    return createRequest(
        QNetworkAccessManager::PostOperation,
        QUrl("https://music.163.com/api/v1/resource/comments/R_SO_4_" + query["id"].toString()),
        data,
        QVariantMap({
            { "crypto", "weapi" },
            { "cookie", query["cookie"] },
            { "proxy", query["proxy"] },
            { "realIP", query["realIP"] }
        })
        );
}
}
