#include "module.h"

#include <iostream>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkInterface>
#include <QVariantMap>
#include <QRegularExpression>

#include "request.hpp"

NeteaseCloudMusicApi::NeteaseCloudMusicApi() {

}

NeteaseCloudMusicApi::~NeteaseCloudMusicApi() {

}

// 初始化名字
const QVariantMap NeteaseCloudMusicApi::activate_init_profile(QVariantMap query) {
    QVariantMap data = {
        { "nickname", query["nickname"] }
    };
    return createRequest(
        QNetworkAccessManager::PostOperation,
        "https://music.163.com/eapi/activate/initProfile",
        data,
        QVariantMap({
            { "crypto", "eapi" },
            { "cookie", query["cookie"] },
            { "proxy", query["proxy"] },
            { "realIP", query["realIP"] },
            { "url", "/api/activate/initProfile" }
        })
        );
}

// 专辑动态信息
const QVariantMap NeteaseCloudMusicApi::album_detail_dynamic(QVariantMap query) {
    const QVariantMap data = {
        { "id", query["id"] }
    };
    return createRequest(
        QNetworkAccessManager::PostOperation,
        "https://music.163.com/api/album/detail/dynamic",
        data,
        QVariantMap({
            { "crypto", "weapi" },
            { "cookie", query["cookie"] },
            { "proxy", query["proxy"] },
            { "realIP", query["realIP"] }
        })
        );
}

// 数字专辑详情
const QVariantMap NeteaseCloudMusicApi::album_detail(QVariantMap query) {
    const QVariantMap data = {
        { "id", query["id"] }
    };
    return createRequest(
        QNetworkAccessManager::PostOperation,
        "https://music.163.com/weapi/vipmall/albumproduct/detail",
        data,
        QVariantMap({
            { "crypto", "weapi" },
            { "cookie", query["cookie"] },
            { "proxy", query["proxy"] },
            { "realIP", query["realIP"] }
        })
        );
}

// 数字专辑-语种风格馆
const QVariantMap NeteaseCloudMusicApi::album_list_style(QVariantMap query) {
    const QVariantMap data = {
        { "limit", query.value("limit", 10) },
        { "offset", query.value("offset", 0) },
        { "total", true },
        { "area", query.value("area", "Z_H") }//Z_H:华语,E_A:欧美,KR:韩国,JP:日本
    };
    return createRequest(
        QNetworkAccessManager::PostOperation,
        "https://music.163.com/weapi/vipmall/appalbum/album/style",
        data,
        QVariantMap({
            { "crypto", "weapi" },
            { "cookie", query["cookie"] },
            { "proxy", query["proxy"] },
            { "realIP", query["realIP"] }
        })
        );
}

// 数字专辑-新碟上架
const QVariantMap NeteaseCloudMusicApi::album_list(QVariantMap query) {
    const QVariantMap data = {
        { "limit", query.value("limit", 30) },
        { "offset", query.value("offset", 0) },
        { "total", true },
        { "area", query.value("area", "ALL") },//ALL:全部,ZH:华语,EA:欧美,KR:韩国,JP:日本
        { "type", query["type"] }
    };
    return createRequest(
        QNetworkAccessManager::PostOperation,
        "https://music.163.com/weapi/vipmall/albumproduct/list",
        data,
        QVariantMap({
            { "crypto", "weapi" },
            { "cookie", query["cookie"] },
            { "proxy", query["proxy"] },
            { "realIP", query["realIP"] }
        })
        );
}

// 全部新碟
const QVariantMap NeteaseCloudMusicApi::album_new(QVariantMap query) {
    const QVariantMap data = {
        { "limit", query.value("limit", 30) },
        { "offset", query.value("offset", 0) },
        { "total", true },
        { "area", query.value("area", "ALL") },//ALL:全部,ZH:华语,EA:欧美,KR:韩国,JP:日本
    };
    return createRequest(
        QNetworkAccessManager::PostOperation,
        "https://music.163.com/weapi/album/new",
        data,
        QVariantMap({
            { "crypto", "weapi" },
            { "cookie", query["cookie"] },
            { "proxy", query["proxy"] },
            { "realIP", query["realIP"] }
        })
        );
}

// 最新专辑
const QVariantMap NeteaseCloudMusicApi::album_newest(QVariantMap query) {
    return createRequest(
        QNetworkAccessManager::PostOperation,
        "https://music.163.com/api/discovery/newAlbum",
        {},
        QVariantMap({
            { "crypto", "weapi" },
            { "cookie", query["cookie"] },
            { "proxy", query["proxy"] },
            { "realIP", query["realIP"] }
        })
        );
}

// 数字专辑&数字单曲-榜单
const QVariantMap NeteaseCloudMusicApi::album_songsaleboard(QVariantMap query) {
    QVariantMap data = {
        { "albumType", query.value("albumType", 0) } //0为数字专辑,1为数字单曲
    };
    const QString type = query.value("type", "daily").toString(); // daily,week,year,total
    if(type == "year") data["year"] = query["year"];
    return createRequest(
        QNetworkAccessManager::PostOperation,
        "https://music.163.com/api/feealbum/songsaleboard/" + type + "/type",
        data,
        QVariantMap({
            { "crypto", "weapi" },
            { "cookie", query["cookie"] },
            { "proxy", query["proxy"] },
            { "realIP", query["realIP"] }
        })
        );
}

// 收藏/取消收藏专辑
const QVariantMap NeteaseCloudMusicApi::album_sub(QVariantMap query) {
    query["t"] = query.value("t", 1) == 1 ? "sub" : "unsub";
    const QVariantMap data = {
        { "id", query["id"] }
    };
    return createRequest(
        QNetworkAccessManager::PostOperation,
        "https://music.163.com/api/album/" + query["t"].toString(),
        data,
        QVariantMap({
            { "crypto", "weapi" },
            { "cookie", query["cookie"] },
            { "proxy", query["proxy"] },
            { "realIP", query["realIP"] }
        })
        );
}

// 已收藏专辑列表
const QVariantMap NeteaseCloudMusicApi::album_sublist(QVariantMap query) {
    const QVariantMap data = {
        { "limit", query.value("limit", 25) },
        { "offset", query.value("offset", 0) },
        { "total", true }
    };
    return createRequest(
        QNetworkAccessManager::PostOperation,
        "https://music.163.com/weapi/album/sublist",
        data,
        QVariantMap({
            { "crypto", "weapi" },
            { "cookie", query["cookie"] },
            { "proxy", query["proxy"] },
            { "realIP", query["realIP"] }
        })
        );
}

// 专辑内容
const QVariantMap NeteaseCloudMusicApi::album(QVariantMap query) {
    return createRequest(
        QNetworkAccessManager::PostOperation,
        "https://music.163.com/weapi/v1/album/" + query["id"].toString(),
        {},
        QVariantMap({
            { "crypto", "weapi" },
            { "cookie", query["cookie"] },
            { "proxy", query["proxy"] },
            { "realIP", query["realIP"] }
        })
        );
}

// 歌手专辑列表
const QVariantMap NeteaseCloudMusicApi::artist_album(QVariantMap query) {
    const QVariantMap data = {
        { "limit", query.value("limit", 30) },
        { "offset", query.value("offset", 0) },
        { "total", true }
    };
    return createRequest(
        QNetworkAccessManager::PostOperation,
        "https://music.163.com/weapi/artist/albums/" + QString::number(query["id"].toInt()),
        data,
        QVariantMap({
            { "crypto", "weapi" },
            { "cookie", query["cookie"] },
            { "proxy", query["proxy"] },
            { "realIP", query["realIP"] }
        })
        );
}

// 歌手介绍
const QVariantMap NeteaseCloudMusicApi::artist_desc(QVariantMap query) {
    const QVariantMap data = {
        { "id", query["id"] }
    };
    return createRequest(
        QNetworkAccessManager::PostOperation,
        "https://music.163.com/weapi/artist/introduction",
        data,
        QVariantMap({
            { "crypto", "weapi" },
            { "cookie", query["cookie"] },
            { "proxy", query["proxy"] },
            { "realIP", query["realIP"] }
        })
        );
}

// 歌手详情
const QVariantMap NeteaseCloudMusicApi::artist_detail(QVariantMap query) {
    const QVariantMap data = {
        { "id", query["id"] }
    };
    return createRequest(
        QNetworkAccessManager::PostOperation,
        "https://music.163.com/api/artist/head/info/get",
        data,
        QVariantMap({
            { "crypto", "weapi" },
            { "cookie", query["cookie"] },
            { "proxy", query["proxy"] },
            { "realIP", query["realIP"] }
        })
        );
}

// 歌手粉丝
const QVariantMap NeteaseCloudMusicApi::artist_fans(QVariantMap query) {
    const QVariantMap data = {
        { "id", query["id"] },
        { "limit", query.value("limit", 20) },
        { "offset", query.value("offset", 0) }
    };
    return createRequest(
        QNetworkAccessManager::PostOperation,
        "https://music.163.com/weapi/artist/fans/get",
        data,
        QVariantMap({
            { "crypto", "weapi" },
            { "cookie", query["cookie"] },
            { "proxy", query["proxy"] },
            { "realIP", query["realIP"] }
        })
        );
}

// 歌手粉丝数量
const QVariantMap NeteaseCloudMusicApi::artist_follow_count(QVariantMap query) {
    const QVariantMap data = {
        { "id", query["id"] }
    };
    return createRequest(
        QNetworkAccessManager::PostOperation,
        "https://music.163.com/weapi/artist/follow/count/get",
        data,
        QVariantMap({
            { "crypto", "weapi" },
            { "cookie", query["cookie"] },
            { "proxy", query["proxy"] },
            { "realIP", query["realIP"] }
        })
        );
}

// 歌手分类

/*
    type 取值
    1:男歌手
    2:女歌手
    3:乐队

    area 取值
    -1:全部
    7华语
    96欧美
    8:日本
    16韩国
    0:其他

    initial 取值 a-z/A-Z
*/
const QVariantMap NeteaseCloudMusicApi::artist_list(QVariantMap query) {
    const QVariantMap data = {
        { "initial", (quint16)((query.value("initial", "\0").toString()[0]).toUpper()).unicode() },
        { "offset", query.value("offset", 0) },
        { "limit", query.value("limit", 30) },
        { "total", true },
        { "type", query.value("type", "1") },
        { "area", query["area"] },
        };
    return createRequest(
        QNetworkAccessManager::PostOperation,
        "https://music.163.com/api/v1/artist/list",
        data,
        QVariantMap({
            { "crypto", "weapi" },
            { "cookie", query["cookie"] },
            { "proxy", query["proxy"] },
            { "realIP", query["realIP"] }
        })
        );
}

// 歌手相关MV
const QVariantMap NeteaseCloudMusicApi::artist_mv(QVariantMap query) {
    const QVariantMap data = {
        { "artistId", query["id"] },
        { "limit", query["limit"] },
        { "offset", query["offset"] },
        { "total", true }
    };
    return createRequest(
        QNetworkAccessManager::PostOperation,
        "https://music.163.com/weapi/artist/mvs",
        data,
        QVariantMap({
            { "crypto", "weapi" },
            { "cookie", query["cookie"] },
            { "proxy", query["proxy"] },
            { "realIP", query["realIP"] }
        })
        );
}

// 关注歌手新 MV
const QVariantMap NeteaseCloudMusicApi::artist_new_mv(QVariantMap query) {
    QVariantMap cookie = query["cookie"].toMap();
    cookie["os"] = "ios";
    cookie["appver"] = "8.7.01";
    query["cookie"] = cookie;
    const QVariantMap data = {
        { "limit", query.value("limit", 20) },
        { "startTimestamp", query.value("before", QDateTime::currentDateTime().toMSecsSinceEpoch()) }
    };
    return createRequest(
        QNetworkAccessManager::PostOperation,
        "https://music.163.com/api/sub/artist/new/works/mv/list",
        data,
        QVariantMap({
            { "crypto", "weapi" },
            { "cookie", query["cookie"] },
            { "proxy", query["proxy"] },
            { "realIP", query["realIP"] }
        })
        );
}

// 关注歌手新歌
const QVariantMap NeteaseCloudMusicApi::artist_new_song(QVariantMap query) {
    QVariantMap cookie = query["cookie"].toMap();
    cookie["os"] = "ios";
    cookie["appver"] = "8.7.01";
    query["cookie"] = cookie;
    const QVariantMap data = {
        { "limit", query.value("limit", 20) },
        { "startTimestamp", query.value("before", QDateTime::currentDateTime().toMSecsSinceEpoch()) }
    };
    return createRequest(
        QNetworkAccessManager::PostOperation,
        "https://music.163.com/api/sub/artist/new/works/song/list",
        data,
        QVariantMap({
            { "crypto", "weapi" },
            { "cookie", query["cookie"] },
            { "proxy", query["proxy"] },
            { "realIP", query["realIP"] }
        })
        );
}

// 歌手全部歌曲
const QVariantMap NeteaseCloudMusicApi::artist_songs(QVariantMap query) {
    QVariantMap cookie = query["cookie"].toMap();
    cookie["os"] = "pc";
    query["cookie"] = cookie;
    const QVariantMap data = {
        { "id", query["id"] },
        { "private_cloud", "true" },
        { "work_type", 1 },
        { "order", query.value("order", "hot") }, //hot,time
        { "offset", query.value("offset", 0) },
        { "limit", query.value("limit", 100) }
    };
    return createRequest(
        QNetworkAccessManager::PostOperation,
        "https://music.163.com/api/v1/artist/songs",
        data,
        QVariantMap({
            { "crypto", "weapi" },
            { "cookie", query["cookie"] },
            { "proxy", query["proxy"] },
            { "realIP", query["realIP"] }
        })
        );
}

// 收藏与取消收藏歌手
const QVariantMap NeteaseCloudMusicApi::artist_sub(QVariantMap query) {
    query["t"] = query.value("t", 1) == 1 ? "sub" : "unsub";
    QStringList artistIds;
    artistIds.append(query["id"].toString());
    const QVariantMap data = {
        { "artistId", query["id"] },
        { "artistIds", artistIds }
    };
    return createRequest(
        QNetworkAccessManager::PostOperation,
        "https://music.163.com/weapi/artist/" + query["t"].toString(),
        data,
        QVariantMap({
            { "crypto", "weapi" },
            { "cookie", query["cookie"] },
            { "proxy", query["proxy"] },
            { "realIP", query["realIP"] }
        })
        );
}

// 关注歌手列表
const QVariantMap NeteaseCloudMusicApi::artist_sublist(QVariantMap query) {
    const QVariantMap data = {
        { "limit", query.value("limit", 25) },
        { "offset", query.value("offset", 0) },
        { "total", true }
    };
    return createRequest(
        QNetworkAccessManager::PostOperation,
        "https://music.163.com/weapi/artist/sublist",
        data,
        QVariantMap({
            { "crypto", "weapi" },
            { "cookie", query["cookie"] },
            { "proxy", query["proxy"] },
            { "realIP", query["realIP"] }
        })
        );
}

// 歌手热门 50 首歌曲
const QVariantMap NeteaseCloudMusicApi::artist_top_song(QVariantMap query) {
    const QVariantMap data = {
        { "id", query["id"] }
    };
    return createRequest(
        QNetworkAccessManager::PostOperation,
        "https://music.163.com/api/artist/top/song",
        data,
        QVariantMap({
            { "crypto", "weapi" },
            { "cookie", query["cookie"] },
            { "proxy", query["proxy"] },
            { "realIP", query["realIP"] }
        })
        );
}

// 歌手相关视频
const QVariantMap NeteaseCloudMusicApi::artist_video(QVariantMap query) {
    const QVariantMap data = {
        { "artistId", query["id"] },
        { "page", QJsonDocument::fromVariant(QVariantMap({
                                                { "size", query.value("size", 10) },
                                                { "cursor", query.value("cursor", 0) }
                                            })).toJson(QJsonDocument::JsonFormat::Compact) },
        { "tab", 0 },
        { "order", query.value("order", 0) },
    };
    return createRequest(
        QNetworkAccessManager::PostOperation,
        "https://music.163.com/weapi/mlog/artist/video",
        data,
        QVariantMap({
            { "crypto", "weapi" },
            { "cookie", query["cookie"] },
            { "proxy", query["proxy"] },
            { "realIP", query["realIP"] }
        })
        );
}

// 歌手单曲
const QVariantMap NeteaseCloudMusicApi::artists(QVariantMap query) {
    return createRequest(
        QNetworkAccessManager::PostOperation,
        "https://music.163.com/weapi/v1/artist/" + QString::number(query["id"].toInt()),
        {},
        QVariantMap({
            { "crypto", "weapi" },
            { "cookie", query["cookie"] },
            { "proxy", query["proxy"] },
            { "realIP", query["realIP"] }
        })
        );
}

// 首页轮播图
const QVariantMap NeteaseCloudMusicApi::banner(QVariantMap query) {
    const auto type0 = query.value("type", 0).toInt();
    const QMap<int, QString> typeMap = {
        { 0, "pc" },
        { 1, "android" },
        { 2, "iphone" },
        { 3, "ipad" }
    };
    const QString type = typeMap.value(type0, "pc");
    return createRequest(
        QNetworkAccessManager::PostOperation,
        "https://music.163.com/api/v2/banner/get",
        {
            { "clientType", type }
        },
        QVariantMap({
            { "crypto", "api" },
            { "proxy", query["proxy"] },
            { "realIP", query["realIP"] }
        })
        );
}

// 搜索
const QVariantMap NeteaseCloudMusicApi::cloudsearch(QVariantMap query) {
    const QVariantMap data = {
        { "s", query["keywords"] },
        { "type", query.value("type", 1) },// 1: 单曲, 10: 专辑, 100: 歌手, 1000: 歌单, 1002: 用户, 1004: MV, 1006: 歌词, 1009: 电台, 1014: 视频
        { "limit", query.value("limit", 30) },
        { "offset", query.value("offset", 0) },
        { "total", true }
    };
    return createRequest(
        QNetworkAccessManager::PostOperation,
        "https://interface.music.163.com/eapi/cloudsearch/pc",
        data,
        QVariantMap({
            { "crypto", "eapi" },
            { "cookie", query["cookie"] },
            { "proxy", query["proxy"] },
            { "url", "/api/cloudsearch/pc" },
            { "realIP", query["realIP"] }
        })
        );
}

// 专辑评论
const QVariantMap NeteaseCloudMusicApi::comment_album(QVariantMap query) {
    QVariantMap cookie = query["cookie"].toMap();
    cookie["os"] = "pc";
    query["cookie"] = cookie;
    const QVariantMap data = {
        { "rid", query["id"] },
        { "limit", query.value("limit", 20) },
        { "offset", query.value("offset", 0) },
        { "beforeTime", query.value("before", 0) }
    };
    return createRequest(
        QNetworkAccessManager::PostOperation,
        "https://music.163.com/weapi/v1/resource/comments/R_AL_3_" + QString::number(query["id"].toInt()),
        data,
        QVariantMap({
            { "crypto", "weapi" },
            { "cookie", query["cookie"] },
            { "proxy", query["proxy"] },
            { "realIP", query["realIP"] }
        })
        );
}

// 电台评论
const QVariantMap NeteaseCloudMusicApi::comment_dj(QVariantMap query) {
    QVariantMap cookie = query["cookie"].toMap();
    cookie["os"] = "pc";
    query["cookie"] = cookie;
    const QVariantMap data = {
        { "rid", query["id"] },
        { "limit", query.value("limit", 20) },
        { "offset", query.value("offset", 0) },
        { "beforeTime", query.value("before", 0) }
    };
    return createRequest(
        QNetworkAccessManager::PostOperation,
        "https://music.163.com/weapi/v1/resource/comments/A_DJ_1_" + QString::number(query["id"].toInt()),
        data,
        QVariantMap({
            { "crypto", "weapi" },
            { "cookie", query["cookie"] },
            { "proxy", query["proxy"] },
            { "realIP", query["realIP"] }
        })
        );
}

// 获取动态评论
const QVariantMap NeteaseCloudMusicApi::comment_event(QVariantMap query) {
    const QVariantMap data = {
        { "limit", query.value("limit", 20) },
        { "offset", query.value("offset", 0) },
        { "beforeTime", query.value("before", 0) }
    };
    return createRequest(
        QNetworkAccessManager::PostOperation,
        "https://music.163.com/weapi/v1/resource/comments/" + QString::number(query["threadId"].toInt()),
        data,
        QVariantMap({
            { "crypto", "weapi" },
            { "cookie", query["cookie"] },
            { "proxy", query["proxy"] },
            { "realIP", query["realIP"] }
        })
        );
}

// 歌曲评论
const QVariantMap NeteaseCloudMusicApi::comment_music(QVariantMap query) {
    QVariantMap cookie = query["cookie"].toMap();
    cookie["os"] = "pc";
    query["cookie"] = cookie;
    const QVariantMap data = {
        { "rid", query["id"] },
        { "limit", query.value("limit", 20) },
        { "offset", query.value("offset", 0) },
        { "beforeTime", query.value("before", 0) }
    };
    return createRequest(
        QNetworkAccessManager::PostOperation,
        "https://music.163.com/api/v1/resource/comments/R_SO_4_" + QString::number(query["id"].toInt()),
        data,
        QVariantMap({
            { "crypto", "weapi" },
            { "cookie", query["cookie"] },
            { "proxy", query["proxy"] },
            { "realIP", query["realIP"] }
        })
        );
}

// 国家编码列表
const QVariantMap NeteaseCloudMusicApi::countries_code_list(QVariantMap query) {
    const QVariantMap data = { };
    return createRequest(
        QNetworkAccessManager::PostOperation,
        "https://interface3.music.163.com/eapi/lbs/countries/v1",
        data,
        QVariantMap({
            { "crypto", "eapi" },
            { "cookie", query["cookie"] },
            { "proxy", query["proxy"] },
            { "url", "/api/lbs/countries/v1" },
            { "realIP", query["realIP"] }
        })
        );
}

// 手机登录
const QVariantMap NeteaseCloudMusicApi::login_cellphone(QVariantMap query) {
    QVariantMap cookie = query["cookie"].toMap();
    cookie["os"] = "ios";
    cookie["appver"] = "8.7.01";
    query["cookie"] = cookie;
    const QVariantMap data = {
        { "phone", query["phone"] },
        { "countrycode", query.value("countrycode", 86) },
//        { "captcha", query["captcha"] },
        { query.contains("captcha") ? "captcha" : "password",
         query.value("captcha",
                     query.value("md5_password",
                                 QCryptographicHash::hash(query["password"].toString().toUtf8(), QCryptographicHash::Md5).toHex())) },
        { "rememberLogin", "true" }
    };
    QVariantMap result = createRequest(
        QNetworkAccessManager::PostOperation,
        "https://music.163.com/weapi/login/cellphone",
        data,
        QVariantMap({
            { "crypto", "weapi" },
            { "ua", "pc" },
            { "cookie", query["cookie"] },
            { "proxy", query["proxy"] },
            { "realIP", query["realIP"] }
        })
        );
    if(result["body"].toMap()["code"].toInt() == 200) {
        auto body = result["body"].toMap();
        body["cookie"] = result["cookie"];
        result = QVariantMap({
            { "status", 200 },
            { "body", body },
            { "cookie", result["cookie"] }
        });
    }
    return result;
}

// 退出登录
const QVariantMap NeteaseCloudMusicApi::logout(QVariantMap query) {
    return createRequest(
        QNetworkAccessManager::PostOperation,
        "https://music.163.com/weapi/logout",
        {},
        QVariantMap({
            { "crypto", "eapi" },
            { "ua", "pc" },
            { "cookie", query["cookie"] },
            { "proxy", query["proxy"] },
            { "realIP", query["realIP"] }
        })
        );
}

// 新版歌词 - 包含逐字歌词
const QVariantMap NeteaseCloudMusicApi::lyric_new(QVariantMap query) {
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
        "https://interface3.music.163.com/eapi/song/lyric/v1",
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

// 歌词
const QVariantMap NeteaseCloudMusicApi::lyric(QVariantMap query) {
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
        "https://music.163.com/api/song/lyric?_nmclfl=1",
        data,
        QVariantMap({
            { "crypto", "api" },
            { "cookie", query["cookie"] },
            { "proxy", query["proxy"] },
            { "realIP", query["realIP"] }
        })
        );
}

// 重复昵称检测
const QVariantMap NeteaseCloudMusicApi::nickname_check(QVariantMap query) {
    QVariantMap data = {
        { "nickname", query["nickname"] }
    };
    return createRequest(
        QNetworkAccessManager::PostOperation,
        "https://music.163.com/api/nickname/duplicated",
        data,
        QVariantMap({
            { "crypto", "weapi" },
            { "cookie", query["cookie"] },
            { "proxy", query["proxy"] },
            { "realIP", query["realIP"] }
        })
        );
}

// 游客登录
const QVariantMap NeteaseCloudMusicApi::register_anonimous(QVariantMap query) {
    auto cookie = query["cookie"].toMap();
    cookie["os"] = "iOS";
    query["cookie"] = cookie;

    const QByteArray ID_XOR_KEY_1 = QByteArray::fromStdString("3go8&$833h0k(2)2");

    auto cloudmusic_dll_encode_id = [ID_XOR_KEY_1](QString some_id) {
        QString xored;
        for (int i = 0; i < some_id.size(); i++) {
            xored.append(QChar(some_id[i].unicode() ^ ID_XOR_KEY_1[i % ID_XOR_KEY_1.size()]));
        }
        QByteArray digest = QCryptographicHash::hash(xored.toUtf8(), QCryptographicHash::Md5);
        return digest.toBase64();
    };

    auto createRandomDeviceId = []() {
        const QString t = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
        const int e = 6;
        QString n = "";
        for (int i = 0; i < e; i++) n += t.at(QRandomGenerator::global()->bounded(e));
        return n;
    };

    const auto deviceId = createRandomDeviceId();
    const auto encodedId = QString(deviceId + " " + cloudmusic_dll_encode_id(deviceId)).toUtf8();
    const auto username = encodedId.toBase64();
    const QVariantMap data = {
                              /* A base64 encoded string. */
                              { "username", username },
                              };

    QVariantMap result = createRequest(
        QNetworkAccessManager::PostOperation,
        "https://music.163.com/api/register/anonimous",
        data,
        QVariantMap({
            { "crypto", "weapi" },
            { "cookie", query["cookie"] },
            { "proxy", query["proxy"] },
            { "realIP", query["realIP"] }
        })
        );
    if(result["body"].toMap()["code"].toInt() == 200) {
        auto body = result["body"].toMap();
        body["cookie"] = result["cookie"].toStringList().join(';');
        result = QVariantMap({
            { "status", 200 },
            { "body", body },
            { "cookie", result["cookie"] }
        });
    }
    return result;
}

// 相关歌单
const QVariantMap NeteaseCloudMusicApi::related_playlist(QVariantMap query) {
    QVariantMap result = createRequest(
        QNetworkAccessManager::GetOperation,
        "https://music.163.com/playlist?id=" + query["id"].toString(),
        {},
        QVariantMap({
            { "ua", "pc" },
            { "cookie", query["cookie"] },
            { "proxy", query["proxy"] },
            { "realIP", query["realIP"] }
        })
        );
    QRegularExpression pattern("<div class=\"cver u-cover u-cover-3\">[\\s\\S]*?<img src=\"([^\"]+)\">[\\s\\S]*?<a class=\"sname f-fs1 s-fc0\" href=\"([^\"]+)\"[^>]*>([^<]+?)<\\/a>[\\s\\S]*?<a class=\"nm nm f-thide s-fc3\" href=\"([^\"]+)\"[^>]*>([^<]+?)<\\/a>");
    QRegularExpressionMatchIterator it = pattern.globalMatch(result["body"].toString());
    QJsonArray playlists;
    while (it.hasNext()) {
        auto result = it.next().capturedTexts();
        playlists.push_back(
            QJsonValue::fromVariant(
                QVariantMap({
                    {
                        "creator", {
                            QVariantMap({
                                { "userId", result[4].split("/user/home?id=")[1] },
                                { "nickname", result[5] }
                            })
                        }
                    },
                    {
                        "coverImgUrl", result[1].split("?param=50y50")[0]
                    },
                    {
                        "name", result[3]
                    },
                    {
                        "id", result[2].split("/playlist?id=")[1]
                    }
                })
                )
            );
    }
    result["body"] = QVariantMap({
        { "code", 200 },
        { "playlists", QVariant(playlists) }
    });
    return result;
}

// 搜索
const QVariantMap NeteaseCloudMusicApi::search(QVariantMap query) {
    if (query.contains("type") && query["type"].toString() == "2000") {
        const QVariantMap data = {
            { "keyword", query["keywords"] },
            { "scene", "normal"},
            { "limit", query.value("limit", 20) },
            { "offset", query.value("offset", 0) }
        };
        return createRequest(
            QNetworkAccessManager::PostOperation,
            "https://music.163.com/api/search/voice/get",
            data,
            QVariantMap({
                { "crypto", "weapi" },
                { "cookie", query["cookie"] },
                { "proxy", query["proxy"] },
                { "realIP", query["realIP"] }
            })
            );
    }
    const QVariantMap data = {
        { "s", query["keywords"] },
        { "type", query.value("type", 1) },// 1: 单曲, 10: 专辑, 100: 歌手, 1000: 歌单, 1002: 用户, 1004: MV, 1006: 歌词, 1009: 电台, 1014: 视频
        { "limit", query.value("limit", 20) },
        { "offset", query.value("offset", 0) }
    };
    return createRequest(
        QNetworkAccessManager::PostOperation,
        "https://music.163.com/weapi/search/get",
        data,
        QVariantMap({
            { "crypto", "weapi" },
            { "cookie", query["cookie"] },
            { "proxy", query["proxy"] },
            { "realIP", query["realIP"] }
        })
        );
}

// 获取客户端歌曲下载链接
const QVariantMap NeteaseCloudMusicApi::song_download_url(QVariantMap query) {
    QVariantMap data = {
        { "id", query["id"] },
        { "br", query.value("br", 999000) }
    };
    return createRequest(
        QNetworkAccessManager::PostOperation,
        "https://interface.music.163.com/eapi/song/enhance/download/url",
        data,
        QVariantMap({
            { "crypto", "eapi" },
            { "cookie", query["cookie"] },
            { "proxy", query["proxy"] },
            { "realIP", query["realIP"] },
            { "url", "/api/song/enhance/download/url" }
        })
        );
}

// 歌曲链接 - v1
// 此版本不再采用 br 作为音质区分的标准
// 而是采用 standard, exhigh, lossless, hires, jyeffect(高清环绕声), sky(沉浸环绕声), jymaster(超清母带) 进行音质判断
const QVariantMap NeteaseCloudMusicApi::song_url_v1(QVariantMap query) {
    QVariantMap data = {
        { "ids", query["id"].toList() },
        { "level", query["level"].toString() },
        { "encodeType", "flac" }
    };
    if(data["level"].toString() == "sky") {
        data["immerseType"] = "c51";
    }
    return createRequest(
        QNetworkAccessManager::PostOperation,
        "https://interface.music.163.com/eapi/song/enhance/player/url/v1",
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

// 音乐百科基础信息
const QVariantMap NeteaseCloudMusicApi::song_wiki_summary(QVariantMap query) {
    QVariantMap data = {
        { "songId", query["id"].toInt() }
    };
    return createRequest(
        QNetworkAccessManager::PostOperation,
        "https://interface3.music.163.com/eapi/music/wiki/home/song/get",
        data,
        QVariantMap({
            { "crypto", "eapi" },
            { "cookie", query["cookie"] },
            { "proxy", query["proxy"] },
            { "realIP", query["realIP"] },
            { "url", "/api/song/play/about/block/page" }
        })
        );
}

// 年度听歌报告2017-2022
const QVariantMap NeteaseCloudMusicApi::summary_annual(QVariantMap query) {
    QVariantMap data = { };
    const QString key = QList<QString>({ "2017", "2018", "2019 "}).indexOf(query["year"].toString()) > -1 ? "userdata" : "data";
    return createRequest(
        QNetworkAccessManager::PostOperation,
        "https://music.163.com/weapi/activity/summary/annual/" + query["year"].toString() + "/" + key,
        data,
        QVariantMap({
            { "crypto", "eapi" },
            { "cookie", query["cookie"] },
            { "proxy", query["proxy"] },
            { "realIP", query["realIP"] },
            { "url", "/api/activity/summary/annual/" + query["year"].toString() + "/" + key }
        })
        );
}

// 所有榜单介绍
const QVariantMap NeteaseCloudMusicApi::toplist(QVariantMap query) {
    return createRequest(
        QNetworkAccessManager::PostOperation,
        "https://music.163.com/api/toplist",
        {},
        QVariantMap({
            { "crypto", "api" },
            { "cookie", query["cookie"] },
            { "proxy", query["proxy"] },
            { "realIP", query["realIP"] }
        })
        );
}

// 专辑简要百科信息
const QVariantMap NeteaseCloudMusicApi::ugc_album_get(QVariantMap query) {
    const QVariantMap data = {
        { "albumId", query["id"] }
    };
    return createRequest(
        QNetworkAccessManager::PostOperation,
        "https://music.163.com/weapi/rep/ugc/album/get",
        data,
        QVariantMap({
            { "crypto", "eapi" },
            { "cookie", query["cookie"] },
            { "proxy", query["proxy"] },
            { "realIP", query["realIP"] },
            { "url", "/api/rep/ugc/album/get" }
        })
        );
}

// 歌手简要百科信息
const QVariantMap NeteaseCloudMusicApi::ugc_artist_get(QVariantMap query) {
    const QVariantMap data = {
        { "artistId", query["id"] }
    };
    return createRequest(
        QNetworkAccessManager::PostOperation,
        "https://music.163.com/weapi/rep/ugc/artist/get",
        data,
        QVariantMap({
            { "crypto", "eapi" },
            { "cookie", query["cookie"] },
            { "proxy", query["proxy"] },
            { "realIP", query["realIP"] },
            { "url", "/api/rep/ugc/artist/get" }
        })
        );
}

// 搜索歌手
// 可传关键字或者歌手id
const QVariantMap NeteaseCloudMusicApi::ugc_artist_search(QVariantMap query) {
    const QVariantMap data = {
        { "keyword", query["keyword"] },
        { "limit", query.value("limit", 40) }
    };
    return createRequest(
        QNetworkAccessManager::PostOperation,
        "https://music.163.com/api/rep/ugc/artist/search",
        data,
        QVariantMap({
            { "crypto", "weapi" },
            { "cookie", query["cookie"] },
            { "proxy", query["proxy"] },
            { "realIP", query["realIP"] }
        })
        );
}

// mv简要百科信息
const QVariantMap NeteaseCloudMusicApi::ugc_mv_get(QVariantMap query) {
    const QVariantMap data = {
        { "mvId", query["id"] }
    };
    return createRequest(
        QNetworkAccessManager::PostOperation,
        "https://music.163.com/weapi/rep/ugc/mv/get",
        data,
        QVariantMap({
            { "crypto", "eapi" },
            { "cookie", query["cookie"] },
            { "proxy", query["proxy"] },
            { "realIP", query["realIP"] },
            { "url", "/api/rep/ugc/mv/get" }
        })
        );
}

// 歌曲简要百科信息
const QVariantMap NeteaseCloudMusicApi::ugc_song_get(QVariantMap query) {
    const QVariantMap data = {
        { "songId", query["id"] }
    };
    return createRequest(
        QNetworkAccessManager::PostOperation,
        "https://music.163.com/weapi/rep/ugc/song/get",
        data,
        QVariantMap({
            { "crypto", "eapi" },
            { "cookie", query["cookie"] },
            { "proxy", query["proxy"] },
            { "realIP", query["realIP"] },
            { "url", "/api/rep/ugc/song/get" }
        })
        );
}

// 获取账号信息
const QVariantMap NeteaseCloudMusicApi::user_account(QVariantMap query) {
    const QVariantMap data = {};
    return createRequest(
        QNetworkAccessManager::PostOperation,
        "https://music.163.com/api/nuser/account/get",
        data,
        QVariantMap({
            { "crypto", "weapi" },
            { "cookie", query["cookie"] },
            { "proxy", query["proxy"] },
            { "realIP", query["realIP"] }
        })
        );
}

// 获取用户历史评论
const QVariantMap NeteaseCloudMusicApi::user_comment_history(QVariantMap query) {
    QVariantMap cookie = query["cookie"].toMap();
    cookie["os"] = "ios";
    cookie["appver"] = "8.7.01";
    query["cookie"] = cookie;
    const QVariantMap data = {
        { "compose_reminder", "true" },
        { "compose_hot_comment", "true" },
        { "limit", query.value("limit", 10) },
        { "user_id", query["uid"] },
        { "time", query.value("time", 0) }
    };
    return createRequest(
        QNetworkAccessManager::PostOperation,
        "https://music.163.com/api/comment/user/comment/history",
        data,
        QVariantMap({
            { "crypto", "weapi" },
            { "cookie", query["cookie"] },
            { "proxy", query["proxy"] },
            { "realIP", query["realIP"] }
        })
        );
}

// 用户详情
const QVariantMap NeteaseCloudMusicApi::user_detail(QVariantMap query) {
    return createRequest(
        QNetworkAccessManager::PostOperation,
        "https://music.163.com/weapi/v1/user/detail/" + query["uid"].toString(),
        {},
        QVariantMap({
            { "crypto", "weapi" },
            { "cookie", query["cookie"] },
            { "proxy", query["proxy"] },
            { "realIP", query["realIP"] }
        })
        );
}

// 用户歌单
const QVariantMap NeteaseCloudMusicApi::user_playlist(QVariantMap query) {
    const QVariantMap data = {
        { "uid", query["uid"] },
        { "limit", query.value("limit", 30) },
        { "offset", query.value("offset", 0) },
        { "includeVideo", true },
        };
    return createRequest(
        QNetworkAccessManager::PostOperation,
        "https://music.163.com/api/user/playlist",
        data,
        QVariantMap({
            { "crypto", "weapi" },
            { "cookie", query["cookie"] },
            { "proxy", query["proxy"] },
            { "realIP", query["realIP"] }
        })
        );
}

