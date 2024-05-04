#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkInterface>
#include <QVariantMap>
#include <QRegularExpression>
#include <QRandomGenerator>

#include "util/config.h"
#include "util/request.h"
#include "util/index.h"
#include "module.h"
#include "plugins.h"

//定义一些重复参数
#define _PARAM \
{ "cookie", query["cookie"] }, \
{ "proxy", query["proxy"] }, \
{ "realIP", query["realIP"] } \

using Api = NeteaseCloudMusicApi;
const static auto &request = QCloudMusicApi::Request::createRequest;
const static auto &POST = QNetworkAccessManager::PostOperation;
const static auto &GET = QNetworkAccessManager::GetOperation;
const static auto &resourceTypeMap = QCloudMusicApi::Config::resourceTypeMap;

const static auto &uploadPlugin = QCloudMusicApi::Plugins::upload;

// 初始化名字
NeteaseCloudMusicApi::NeteaseCloudMusicApi(QObject *parent)
    : QObject{parent}
{}

QVariantMap Api::activate_init_profile(QVariantMap query) {
    QVariantMap data {
        { "nickname", query["nickname"] }
    };
    return request(
        POST,
        "https://music.163.com/eapi/activate/initProfile",
        data,
        {
            { "crypto", "eapi" },
            _PARAM,
            { "url", "/api/activate/initProfile" }
        }
        );
}

// 专辑动态信息
QVariantMap Api::album_detail_dynamic(QVariantMap query) {
    const QVariantMap data {
        { "id", query["id"] }
    };
    return request(
        POST,
        "https://music.163.com/api/album/detail/dynamic",
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 数字专辑详情
QVariantMap Api::album_detail(QVariantMap query) {
    const QVariantMap data {
        { "id", query["id"] }
    };
    return request(
        POST,
        "https://music.163.com/weapi/vipmall/albumproduct/detail",
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 数字专辑-语种风格馆
QVariantMap Api::album_list_style(QVariantMap query) {
    const QVariantMap data {
        { "limit", query.value("limit", 10) },
        { "offset", query.value("offset", 0) },
        { "total", true },
        { "area", query.value("area", "Z_H") }//Z_H:华语,E_A:欧美,KR:韩国,JP:日本
    };
    return request(
        POST,
        "https://music.163.com/weapi/vipmall/appalbum/album/style",
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 数字专辑-新碟上架
QVariantMap Api::album_list(QVariantMap query) {
    const QVariantMap data {
        { "limit", query.value("limit", 30) },
        { "offset", query.value("offset", 0) },
        { "total", true },
        { "area", query.value("area", "ALL") },//ALL:全部,ZH:华语,EA:欧美,KR:韩国,JP:日本
        { "type", query["type"] }
    };
    return request(
        POST,
        "https://music.163.com/weapi/vipmall/albumproduct/list",
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 全部新碟
QVariantMap Api::album_new(QVariantMap query) {
    const QVariantMap data {
        { "limit", query.value("limit", 30) },
        { "offset", query.value("offset", 0) },
        { "total", true },
        { "area", query.value("area", "ALL") },//ALL:全部,ZH:华语,EA:欧美,KR:韩国,JP:日本
    };
    return request(
        POST,
        "https://music.163.com/weapi/album/new",
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 最新专辑
QVariantMap Api::album_newest(QVariantMap query) {
    return request(
        POST,
        "https://music.163.com/api/discovery/newAlbum",
        {},
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 数字专辑&数字单曲-榜单
QVariantMap Api::album_songsaleboard(QVariantMap query) {
    QVariantMap data {
        { "albumType", query.value("albumType", 0) } //0为数字专辑,1为数字单曲
    };
    const QString type = query.value("type", "daily").toString(); // daily,week,year,total
    if(type == "year") data["year"] = query["year"];
    return request(
        POST,
        "https://music.163.com/api/feealbum/songsaleboard/" + type + "/type",
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 收藏/取消收藏专辑
QVariantMap Api::album_sub(QVariantMap query) {
    query["t"] = query.value("t", 1) == 1 ? "sub" : "unsub";
    const QVariantMap data {
        { "id", query["id"] }
    };
    return request(
        POST,
        "https://music.163.com/api/album/" + query["t"].toString(),
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 已收藏专辑列表
QVariantMap Api::album_sublist(QVariantMap query) {
    const QVariantMap data {
        { "limit", query.value("limit", 25) },
        { "offset", query.value("offset", 0) },
        { "total", true }
    };
    return request(
        POST,
        "https://music.163.com/weapi/album/sublist",
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 专辑内容
QVariantMap Api::album(QVariantMap query) {
    return request(
        POST,
        "https://music.163.com/weapi/v1/album/" + query["id"].toString(),
        {},
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 歌手专辑列表
QVariantMap Api::artist_album(QVariantMap query) {
    const QVariantMap data {
        { "limit", query.value("limit", 30) },
        { "offset", query.value("offset", 0) },
        { "total", true }
    };
    return request(
        POST,
        "https://music.163.com/weapi/artist/albums/" + query["id"].toString(),
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 歌手介绍
QVariantMap Api::artist_desc(QVariantMap query) {
    const QVariantMap data {
        { "id", query["id"] }
    };
    return request(
        POST,
        "https://music.163.com/weapi/artist/introduction",
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 歌手详情
QVariantMap Api::artist_detail(QVariantMap query) {
    const QVariantMap data {
        { "id", query["id"] }
    };
    return request(
        POST,
        "https://music.163.com/api/artist/head/info/get",
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 歌手粉丝
QVariantMap Api::artist_fans(QVariantMap query) {
    const QVariantMap data {
        { "id", query["id"] },
        { "limit", query.value("limit", 20) },
        { "offset", query.value("offset", 0) }
    };
    return request(
        POST,
        "https://music.163.com/weapi/artist/fans/get",
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 歌手粉丝数量
QVariantMap Api::artist_follow_count(QVariantMap query) {
    const QVariantMap data {
        { "id", query["id"] }
    };
    return request(
        POST,
        "https://music.163.com/weapi/artist/follow/count/get",
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
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
QVariantMap Api::artist_list(QVariantMap query) {
    const QVariantMap data {
        { "initial", (quint16)((query.value("initial", "\0").toString()[0]).toUpper()).unicode() },
        { "offset", query.value("offset", 0) },
        { "limit", query.value("limit", 30) },
        { "total", true },
        { "type", query.value("type", "1") },
        { "area", query["area"] },
        };
    return request(
        POST,
        "https://music.163.com/api/v1/artist/list",
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 歌手相关MV
QVariantMap Api::artist_mv(QVariantMap query) {
    const QVariantMap data {
        { "artistId", query["id"] },
        { "limit", query["limit"] },
        { "offset", query["offset"] },
        { "total", true }
    };
    return request(
        POST,
        "https://music.163.com/weapi/artist/mvs",
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 关注歌手新 MV
QVariantMap Api::artist_new_mv(QVariantMap query) {
    QVariantMap cookie = query["cookie"].toMap();
    cookie["os"] = "ios";
    cookie["appver"] = "8.10.90";
    query["cookie"] = cookie;
    const QVariantMap data {
        { "limit", query.value("limit", 20) },
        { "startTimestamp", query.value("before", QDateTime::currentDateTime().toMSecsSinceEpoch()) }
    };
    return request(
        POST,
        "https://music.163.com/api/sub/artist/new/works/mv/list",
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 关注歌手新歌
QVariantMap Api::artist_new_song(QVariantMap query) {
    QVariantMap cookie = query["cookie"].toMap();
    cookie["os"] = "ios";
    cookie["appver"] = "8.10.90";
    query["cookie"] = cookie;
    const QVariantMap data {
        { "limit", query.value("limit", 20) },
        { "startTimestamp", query.value("before", QDateTime::currentDateTime().toMSecsSinceEpoch()) }
    };
    return request(
        POST,
        "https://music.163.com/api/sub/artist/new/works/song/list",
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 歌手全部歌曲
QVariantMap Api::artist_songs(QVariantMap query) {
    QVariantMap cookie = query["cookie"].toMap();
    cookie["os"] = "pc";
    query["cookie"] = cookie;
    const QVariantMap data {
        { "id", query["id"] },
        { "private_cloud", "true" },
        { "work_type", 1 },
        { "order", query.value("order", "hot") }, //hot,time
        { "offset", query.value("offset", 0) },
        { "limit", query.value("limit", 100) }
    };
    return request(
        POST,
        "https://music.163.com/api/v1/artist/songs",
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 收藏与取消收藏歌手
QVariantMap Api::artist_sub(QVariantMap query) {
    query["t"] = query.value("t", 1) == 1 ? "sub" : "unsub";
    QStringList artistIds;
    artistIds.append(query["id"].toString());
    const QVariantMap data {
        { "artistId", query["id"] },
        { "artistIds", artistIds }
    };
    return request(
        POST,
        "https://music.163.com/weapi/artist/" + query["t"].toString(),
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 关注歌手列表
QVariantMap Api::artist_sublist(QVariantMap query) {
    const QVariantMap data {
        { "limit", query.value("limit", 25) },
        { "offset", query.value("offset", 0) },
        { "total", true }
    };
    return request(
        POST,
        "https://music.163.com/weapi/artist/sublist",
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 歌手热门 50 首歌曲
QVariantMap Api::artist_top_song(QVariantMap query) {
    const QVariantMap data {
        { "id", query["id"] }
    };
    return request(
        POST,
        "https://music.163.com/api/artist/top/song",
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 歌手相关视频
QVariantMap Api::artist_video(QVariantMap query) {
    const QVariantMap data {
        { "artistId", query["id"] },
        { "page", QJsonDocument(QJsonObject::fromVariantMap({
                                                { "size", query.value("size", 10) },
                                                { "cursor", query.value("cursor", 0) }
                               })).toJson(QJsonDocument::JsonFormat::Compact) },
        { "tab", 0 },
        { "order", query.value("order", 0) },
    };
    return request(
        POST,
        "https://music.163.com/weapi/mlog/artist/video",
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 歌手单曲
QVariantMap Api::artists(QVariantMap query) {
    return request(
        POST,
        "https://music.163.com/weapi/v1/artist/" + query["id"].toString(),
        {},
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 批量请求接口
QVariantMap Api::batch(QVariantMap query) {
    QVariantMap data {
        { "e_r", true }
    };
    for(auto i = query.begin(); i != query.end(); i++) {
        if(i.key().indexOf("/api/") == 0) {
            data[i.key()] = i.value();
        }
    }
    return request(
        POST,
        "https://music.163.com/eapi/batch",
        data,
        {
            { "crypto", "eapi" },
            _PARAM,
            { "url", "/api/batch" }
        }
        );
}

// 首页轮播图
QVariantMap Api::banner(QVariantMap query) {
    const auto type0 = query.value("type", 0).toInt();
    const QMap<int, QString> typeMap {
        { 0, "pc" },
        { 1, "android" },
        { 2, "iphone" },
        { 3, "ipad" }
    };
    const QString type = typeMap.value(type0, "pc");
    return request(
        POST,
        "https://music.163.com/api/v2/banner/get",
        {
            { "clientType", type }
        },
        {
            { "crypto", "api" },
            { "proxy", query["proxy"] },
            { "realIP", query["realIP"] }
        }
        );
}

// 音乐日历
QVariantMap Api::calendar(QVariantMap query) {
    const QVariantMap data {
        { "startTime", query.value("startTime", QDateTime::currentDateTime().toMSecsSinceEpoch()) },
        { "endTime", query.value("endTime", QDateTime::currentDateTime().toMSecsSinceEpoch()) }
    };
    return request(
        POST,
        "https://music.163.com/api/mcalendar/detail",
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 发送验证码
QVariantMap Api::captcha_sent(QVariantMap query) {
    const QVariantMap data {
        { "ctcode", query.value("ctcode", "86") },
        { "cellphone", query["cellphone"] }
    };
    return request(
        POST,
        "https://music.163.com/api/sms/captcha/sent",
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 校验验证码
QVariantMap Api::captcha_verify(QVariantMap query) {
    const QVariantMap data {
        { "ctcode", query.value("ctcode", "86") },
        { "cellphone", query["phone"] },
        { "captcha", query["captcha"] }
    };
    return request(
        POST,
        "https://music.163.com/weapi/sms/captcha/verify",
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 检测手机号码是否已注册
QVariantMap Api::cellphone_existence_check(QVariantMap query) {
    const QVariantMap data {
        { "cellphone", query["phone"] },
        { "countrycode", query["countrycode"] }
    };
    return request(
        POST,
        "https://music.163.com/eapi/cellphone/existence/check",
        data,
        {
            { "crypto", "eapi" },
            _PARAM,
            { "url", "/api/cellphone/existence/check" }
        }
        );
}

// 歌曲可用性
QVariantMap Api::check_music(QVariantMap query) {
    const QVariantMap data {
        { "ids", QStringList { query["id"].toString() } },
        { "br", query.value("br", 999000) }
    };
    QVariantMap result = request(
        POST,
        "https://music.163.com/weapi/song/enhance/player/url",
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
    auto playable = false;
    if(result["body"].toMap()["code"].toInt() == 200) {
        if(result["body"].toMap()["data"].toList()[0].toMap()["code"].toInt() == 200) {
            playable = true;
        }
    }
    if(playable) {
        result["body"] = QVariantMap {
            { "code", 200 },
            { "success", true },
            { "message", "ok" }
        };
        return result;
    }
    else {
        result["body"] = QVariantMap {
            { "code", 200 },
            { "success", false },
            { "message", "亲爱的,暂无版权" }
        };
        return result;
    }
}

// 云盘歌曲信息匹配纠正
QVariantMap Api::cloud_match(QVariantMap query) {
    QVariantMap cookie = query["cookie"].toMap();
    cookie["os"] = "ios";
    cookie["appver"] = "8.10.90";
    query["cookie"] = cookie;
    const QVariantMap data {
        { "userId", query["uid"] },
        { "songId", query["sid"] },
        { "adjustSongId", query["asid"] }
    };
    return request(
        POST,
        "https://music.163.com/api/cloud/user/song/match",
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 云盘上传
QVariantMap Api::cloud(QVariantMap query) {
    QString ext = "mp3";
    if (query["songFile"].toMap()["name"].toString().indexOf("flac") > -1) {
        ext = "flac";
    }
    QString filename = query["songFile"].toMap()["name"].toString();
    filename.replace("." + ext, "");
    filename.replace(QRegularExpression("\\s"), "");
    filename.replace(QRegularExpression("\\."), "_");
    QVariantMap cookie = query["cookie"].toMap();
    cookie["os"] = "pc";
    cookie["appver"] = "2.9.7";
    query["cookie"] = cookie;
    const auto bitrate = 999000;
    if (!query.contains("songFile")) {
        return {
            { "status", 500 },
            { "body", QVariantMap
                {
                    { "msg", "请上传音乐文件" },
                    { "code", 500 }
                }
            }
        };
    }
    if (!query["songFile"].toMap().contains("md5")) {
        auto songFile = query["songFile"].toMap();
        auto data = songFile["data"].toByteArray();
        QCryptographicHash hash(QCryptographicHash::Md5);
        hash.addData(data);
        songFile["md5"] = hash.result().toHex();
        songFile["size"] = data.length();
        query["songFile"] = songFile;
    }
    const QVariantMap res = request(
        POST,
        "https://interface.music.163.com/api/cloud/upload/check",
        {
         { "bitrate", QString::number(bitrate) },
         { "ext", "" },
         { "length", query["songFile"].toMap()["size"] },
         { "md5", query["songFile"].toMap()["md5"] },
         { "songId", "0" },
         { "version", 1 },
         },
        {
         { "crypto", "weapi" },
         _PARAM,
         }
        );
    QString artist = "";
    QString album = "";
    QString songName = "";

    const auto tokenRes = request(
        POST,
        "https://music.163.com/weapi/nos/token/alloc",
        {
            { "bucket", "" },
            { "ext", ext },
            { "filename", filename },
            { "local", false },
            { "nos_product", 3 },
            { "type", "audio" },
            { "md5", query["songFile"].toMap()["md5"] }
        },
        {
            { "crypto", "weapi" },
            { "cookie", query["cookie"] },
            { "proxy", query["proxy"] }
        }
        );

    if (res["body"].toMap()["needUpload"].toBool()) {
        const auto uploadInfo = uploadPlugin(query);
    }
    const auto res2 = request(
        POST,
        "https://music.163.com/api/upload/cloud/info/v2",
        {
            { "md5", query["songFile"].toMap()["md5"] },
            { "songid", res["body"].toMap()["songId"] },
            { "filename", query["songFile"].toMap()["name"] },
            { "song", !songName.isEmpty() ? songName : filename },
            { "album", !album.isEmpty() ? album : "未知专辑" },
            { "artist", !artist.isEmpty() ? artist : "未知艺术家" },
            { "bitrate", QString::number(bitrate) },
            { "resourceId", tokenRes["body"].toMap()["result"].toMap()["resourceId"] }
        },
        {
            { "crypto", "weapi" },
            _PARAM,
        }
        );
    const auto res3 = request(
        POST,
        "https://interface.music.163.com/api/cloud/pub/v2",
        {
            { "songid", res2["body"].toMap()["songId"] }
        },
        {
         { "crypto", "weapi" },
         _PARAM,
         }
        );
    return {
        { "status", 200 },
        { "body", QCloudMusicApi::Index::mergeMap(res["body"].toMap(), res3["body"].toMap()) },
        { "cookie", res["cookie"] }
    };
}

// 搜索
QVariantMap Api::cloudsearch(QVariantMap query) {
    const QVariantMap data {
        { "s", query["keywords"] },
        { "type", query.value("type", 1) },// 1: 单曲, 10: 专辑, 100: 歌手, 1000: 歌单, 1002: 用户, 1004: MV, 1006: 歌词, 1009: 电台, 1014: 视频
        { "limit", query.value("limit", 30) },
        { "offset", query.value("offset", 0) },
        { "total", true }
    };
    return request(
        POST,
        "https://interface.music.163.com/eapi/cloudsearch/pc",
        data,
        {
            { "crypto", "eapi" },
            _PARAM,
            { "url", "/api/cloudsearch/pc" }
        }
        );
}

// 专辑评论
QVariantMap Api::comment_album(QVariantMap query) {
    QVariantMap cookie = query["cookie"].toMap();
    cookie["os"] = "pc";
    query["cookie"] = cookie;
    const QVariantMap data {
        { "rid", query["id"] },
        { "limit", query.value("limit", 20) },
        { "offset", query.value("offset", 0) },
        { "beforeTime", query.value("before", 0) }
    };
    return request(
        POST,
        "https://music.163.com/weapi/v1/resource/comments/R_AL_3_" + query["id"].toString(),
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 电台评论
QVariantMap Api::comment_dj(QVariantMap query) {
    QVariantMap cookie = query["cookie"].toMap();
    cookie["os"] = "pc";
    query["cookie"] = cookie;
    const QVariantMap data {
        { "rid", query["id"] },
        { "limit", query.value("limit", 20) },
        { "offset", query.value("offset", 0) },
        { "beforeTime", query.value("before", 0) }
    };
    return request(
        POST,
        "https://music.163.com/weapi/v1/resource/comments/A_DJ_1_" + query["id"].toString(),
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 获取动态评论
QVariantMap Api::comment_event(QVariantMap query) {
    const QVariantMap data {
        { "limit", query.value("limit", 20) },
        { "offset", query.value("offset", 0) },
        { "beforeTime", query.value("before", 0) }
    };
    return request(
        POST,
        "https://music.163.com/weapi/v1/resource/comments/" + query["threadId"].toString(),
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 楼层评论
QVariantMap Api::comment_floor(QVariantMap query) {
    query["type"] = resourceTypeMap[query["type"].toString()];
    const QVariantMap data {
        { "parentCommentId", query["parentCommentId"] },
        { "threadId", query["type"].toString() + query["id"].toString() },
        { "time", query.value("time", -1) },
        { "limit", query.value("limit", 20) }
    };
    return request(
        POST,
        "https://music.163.com/api/resource/comment/floor/get",
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 热门评论
QVariantMap Api::comment_hot(QVariantMap query) {
    QVariantMap cookie = query["cookie"].toMap();
    cookie["os"] = "pc";
    query["cookie"] = cookie;
    query["type"] = resourceTypeMap[query["type"].toString()];
    const QVariantMap data {
        { "rid", query["id"] },
        { "limit", query.value("limit", 20) },
        { "offset", query.value("offset", 0) },
        { "beforeTime", query.value("before", 0) }
    };
    return request(
        POST,
        "https://music.163.com/weapi/v1/resource/hotcomments/" + query["type"].toString() + query["id"].toString(),
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 评论抱一抱列表
QVariantMap Api::comment_hug_list(QVariantMap query) {
    QVariantMap cookie = query["cookie"].toMap();
    cookie["os"] = "ios";
    cookie["appver"] = "8.10.90";
    query["cookie"] = cookie;
    query["type"] = resourceTypeMap[query.value("type", "0").toString()];
    const QString threadId = query["type"].toString() + query["sid"].toString();
    const QVariantMap data {
        { "targetUserId", query["uid"] },
        { "commentId", query["cid"] },
        { "cursor", query.value("cursor", "-1") },
        { "threadId", threadId },
        { "pageNo", query.value("page", 1) },
        { "idCursor", query.value("idCursor", -1) },
        { "pageSize", query.value("pageSize", 100) }
    };
    return request(
        POST,
        "https://music.163.com/api/v2/resource/comments/hug/list",
        data,
        {
            { "crypto", "api" },
            _PARAM
        }
        );
}

// 点赞与取消点赞评论
QVariantMap Api::comment_like(QVariantMap query) {
    QVariantMap cookie = query["cookie"].toMap();
    cookie["os"] = "pc";
    query["cookie"] = cookie;
    query["t"] = query["t"].toInt() == 1 ? "like" : "unlike";
    query["type"] = resourceTypeMap[query["type"].toString()];
    QVariantMap data {
        { "threadId", query["type"].toString() + query["sid"].toString() },
        { "commentId", query["cid"] }
    };
    if(query["type"].toString() == "A_EV_2_") {
        data["threadId"] = query["threadId"];
    }
    return request(
        POST,
        "https://music.163.com/weapi/v1/comment/" + query["t"].toString(),
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 歌曲评论
QVariantMap Api::comment_music(QVariantMap query) {
    QVariantMap cookie = query["cookie"].toMap();
    cookie["os"] = "pc";
    query["cookie"] = cookie;
    const QVariantMap data {
        { "rid", query["id"] },
        { "limit", query.value("limit", 20) },
        { "offset", query.value("offset", 0) },
        { "beforeTime", query.value("before", 0) }
    };
    return request(
        POST,
        "https://music.163.com/api/v1/resource/comments/R_SO_4_" + query["id"].toString(),
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// MV评论
QVariantMap Api::comment_mv(QVariantMap query) {
    QVariantMap cookie = query["cookie"].toMap();
    cookie["os"] = "pc";
    query["cookie"] = cookie;
    const QVariantMap data {
        { "rid", query["id"] },
        { "limit", query.value("limit", 20) },
        { "offset", query.value("offset", 0) },
        { "beforeTime", query.value("before", 0) }
    };
    return request(
        POST,
        "https://music.163.com/weapi/v1/resource/comments/R_MV_5_" + query["id"].toString(),
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 新版评论接口
QVariantMap Api::comment_new(QVariantMap query) {
    QVariantMap cookie = query["cookie"].toMap();
    cookie["os"] = "pc";
    query["cookie"] = cookie;
    query["type"] = resourceTypeMap[query["type"].toString()];
    const QString threadId = query["type"].toString() + query["id"].toString();
    const int pageSize = query.value("pageSize", 20).toInt();
    const int pageNo = query.value("pageNo", 1).toInt();
    int sortType = query.value("sortType", 99).toInt();
    if(sortType == 1) {
        sortType = 99;
    }
    QString cursor = "";
    switch (sortType) {
    case 99:
        cursor = QString::number((pageNo - 1) * pageSize);
        break;
    case 2:
        cursor = "normalHot#" + QString::number((pageNo - 1) * pageSize);
        break;
    case 3:
        cursor = query.value("cursor", "0").toString();
        break;
    default:
        break;
    }
    const QVariantMap data {
        { "threadId", threadId },
        { "pageNo", pageNo },
        { "showInner", query.value("showInner", true) },
        { "pageSize", pageSize },
        { "cursor", cursor },
        { "sortType", sortType } //99:按推荐排序,2:按热度排序,3:按时间排序
    };
    return request(
        POST,
        "https://music.163.com/api/v2/resource/comments",
        data,
        {
            { "crypto", "eapi" },
            _PARAM,
            { "url", "/api/v2/resource/comments" }
        }
        );
}

// 歌单评论
QVariantMap Api::comment_playlist(QVariantMap query) {
    QVariantMap cookie = query["cookie"].toMap();
    cookie["os"] = "pc";
    query["cookie"] = cookie;
    const QVariantMap data {
        { "rid", query["id"] },
        { "limit", query.value("limit", 20) },
        { "offset", query.value("offset", 0) },
        { "beforeTime", query.value("before", 0) }
    };
    return request(
        POST,
        "https://music.163.com/weapi/v1/resource/comments/A_PL_0_" + query["id"].toString(),
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 视频评论
QVariantMap Api::comment_video(QVariantMap query) {
    QVariantMap cookie = query["cookie"].toMap();
    cookie["os"] = "pc";
    query["cookie"] = cookie;
    const QVariantMap data {
        { "rid", query["id"] },
        { "limit", query.value("limit", 20) },
        { "offset", query.value("offset", 0) },
        { "beforeTime", query.value("before", 0) }
    };
    return request(
        POST,
        "https://music.163.com/weapi/v1/resource/comments/R_VI_62_" + query["id"].toString(),
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 发送与删除评论
QVariantMap Api::comment(QVariantMap query) {
    QVariantMap cookie = query["cookie"].toMap();
    cookie["os"] = "android";
    query["cookie"] = cookie;
    query["t"] = QMap<int, QString> {
        { 1, "add" },
        { 0, "delete" },
        { 2, "reply" }
    }[query["t"].toInt()];
    query["type"] = resourceTypeMap[query["type"].toString()];
    QVariantMap data {
        { "threadId", query["type"].toString() + query["id"].toString() }
    };
    if(query["type"].toString() == "A_EV_2_") {
        data["threadId"] = query["threadId"];
    }
    if(query["t"].toString() == "add") data["content"] = query["content"];
    else if(query["t"].toString() == "delete") data["commentId"] = query["commentId"];
    else if(query["t"].toString() == "reply") {
        data["commentId"] = query["commentId"];
        data["content"] = query["content"];
    }
    return request(
        POST,
        "https://music.163.com/weapi/resource/comments/" + query["t"].toString(),
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 国家编码列表
QVariantMap Api::countries_code_list(QVariantMap query) {
    const QVariantMap data { };
    return request(
        POST,
        "https://interface3.music.163.com/eapi/lbs/countries/v1",
        data,
        {
            { "crypto", "eapi" },
            _PARAM,
            { "url", "/api/lbs/countries/v1" }
        }
        );
}

// 获取达人用户信息
QVariantMap Api::creator_authinfo_get(QVariantMap query) {
    const QVariantMap data { };
    return request(
        POST,
        "https://interface.music.163.com/weapi/user/creator/authinfo/get",
        data,
        {
            { "crypto", "eapi" },
            _PARAM,
            { "url", "/api/user/creator/authinfo/get" }
        }
        );
}

// 签到

/*
    0为安卓端签到 3点经验, 1为网页签到,2点经验
    签到成功 {'android': {'point': 3, 'code': 200}, 'web': {'point': 2, 'code': 200}}
    重复签到 {'android': {'code': -2, 'msg': '重复签到'}, 'web': {'code': -2, 'msg': '重复签到'}}
    未登录 {'android': {'code': 301}, 'web': {'code': 301}}
    */
QVariantMap Api::daily_signin(QVariantMap query) {
    const QVariantMap data {
        { "type", query.value("type", 0) }
    };
    return request(
        POST,
        "https://music.163.com/weapi/point/dailyTask",
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 数字专辑详情
QVariantMap Api::digitalAlbum_detail(QVariantMap query) {
    const QVariantMap data {
        { "id", query["id"] }
    };
    return request(
        POST,
        "https://music.163.com/weapi/vipmall/albumproduct/detail",
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 购买数字专辑
QVariantMap Api::digitalAlbum_ordering(QVariantMap query) {
    const QVariantMap data {
        { "business", "Album" },
        { "paymentMethod", query["payment"] },
        { "digitalResources", QJsonDocument { QJsonArray::fromVariantList({ QVariantMap {
                                                                                    {"business", "Album"},
                                                                                    {"resourceID", query["id"]},
                                                                                    {"quantity", query["quantity"]},
                                                                                    } }) }.toJson(QJsonDocument::Compact) },
        { "from", "web" }
    };
    return request(
        POST,
        "https://music.163.com/api/ordering/web/digital",
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 我的数字专辑
QVariantMap Api::digitalAlbum_purchased(QVariantMap query) {
    const QVariantMap data {
        { "limit", query.value("limit", 30) },
        { "offset", query.value("offset", 0) },
        { "total", true }
    };
    return request(
        POST,
        "https://music.163.com/api/digitalAlbum/purchased",
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 数字专辑销量
QVariantMap Api::digitalAlbum_sales(QVariantMap query) {
    const QVariantMap data {
        { "albumIds", query["ids"] }
    };
    return request(
        POST,
        "https://music.163.com/weapi/vipmall/albumproduct/album/query/sales",
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 电台banner
QVariantMap Api::dj_banner(QVariantMap query) {
    const QVariantMap data { };
    QVariantMap cookie = query["cookie"].toMap();
    cookie["os"] = "pc";
    query["cookie"] = cookie;
    return request(
        POST,
        "https://music.163.com/weapi/djradio/banner/get",
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 电台非热门类型
QVariantMap Api::dj_category_excludehot(QVariantMap query) {
    const QVariantMap data { };
    return request(
        POST,
        "https://music.163.com/weapi/djradio/category/excludehot",
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 电台推荐类型
QVariantMap Api::dj_category_recommend(QVariantMap query) {
    const QVariantMap data { };
    return request(
        POST,
        "https://music.163.com/weapi/djradio/home/category/recommend",
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 电台分类列表
QVariantMap Api::dj_catelist(QVariantMap query) {
    const QVariantMap data { };
    return request(
        POST,
        "https://music.163.com/weapi/djradio/category/get",
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 电台详情
QVariantMap Api::dj_detail(QVariantMap query) {
    const QVariantMap data {
        { "id", query["rid"] }
    };
    return request(
        POST,
        "https://music.163.com/api/djradio/v2/get",
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 热门电台
QVariantMap Api::dj_hot(QVariantMap query) {
    const QVariantMap data {
        { "limit", query.value("limit", 30) },
        { "offset", query.value("offset", 0) }
    };
    return request(
        POST,
        "https://music.163.com/weapi/djradio/hot/v1",
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 付费电台
QVariantMap Api::dj_paygift(QVariantMap query) {
    const QVariantMap data {
        { "limit", query.value("limit", 30) },
        { "offset", query.value("offset", 0) }
    };
    return request(
        POST,
        "https://music.163.com/weapi/djradio/home/paygift/list?_nmclfl=1",
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 电台个性推荐
QVariantMap Api::dj_personalize_recommend(QVariantMap query) {
    const QVariantMap data {
        { "limit", query.value("limit", 6) }
    };
    return request(
        POST,
        "https://music.163.com/api/djradio/personalize/rcmd",
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 电台节目详情
QVariantMap Api::dj_program_detail(QVariantMap query) {
    const QVariantMap data {
        { "id", query["id"] }
    };
    return request(
        POST,
        "https://music.163.com/api/dj/program/detail",
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 电台24小时节目榜
QVariantMap Api::dj_program_toplist_hours(QVariantMap query) {
    const QVariantMap data {
        { "limit", query.value("limit", 100) }
        // 不支持 offset
    };
    return request(
        POST,
        "https://music.163.com/api/djprogram/toplist/hours",
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 电台节目榜
QVariantMap Api::dj_program_toplist(QVariantMap query) {
    const QVariantMap data {
        { "limit", query.value("limit", 100) },
        { "offset", query.value("offset", 0) }
    };
    return request(
        POST,
        "https://music.163.com/api/program/toplist/v1",
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 电台节目列表
QVariantMap Api::dj_program(QVariantMap query) {
    const QVariantMap data {
        { "radioId", query["rid"] },
        { "limit", query.value("limit", 30) },
        { "offset", query.value("offset", 0) },
        { "asc", query["asc"] }
    };
    return request(
        POST,
        "https://music.163.com/weapi/dj/program/byradio",
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 类别热门电台
QVariantMap Api::dj_radio_hot(QVariantMap query) {
    const QVariantMap data {
        { "cateId", query["cateId"] },
        { "limit", query.value("limit", 30) },
        { "offset", query.value("offset", 0) }
    };
    return request(
        POST,
        "https://music.163.com/api/djradio/hot",
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 精选电台分类

/*
    有声书 10001
    知识技能 453050
    商业财经 453051
    人文历史 11
    外语世界 13
    亲子宝贝 14
    创作|翻唱 2001
    音乐故事 2
    3D|电子 10002
    相声曲艺 8
    情感调频 3
    美文读物 6
    脱口秀 5
    广播剧 7
    二次元 3001
    明星做主播 1
    娱乐|影视 4
    科技科学 453052
    校园|教育 4001
    旅途|城市 12
*/
QVariantMap Api::dj_recommend_type(QVariantMap query) {
    const QVariantMap data {
        { "cateId", query["type"] }
    };
    return request(
        POST,
        "https://music.163.com/weapi/djradio/recommend",
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 精选电台
QVariantMap Api::dj_recommend(QVariantMap query) {
    return request(
        POST,
        "https://music.163.com/weapi/djradio/recommend/v1",
        {},
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 订阅与取消电台
QVariantMap Api::dj_sub(QVariantMap query) {
    query["t"] = query["t"].toInt() == 1 ? "sub" : "unsub";
    const QVariantMap data {
        { "id", query["rid"] }
    };
    return request(
        POST,
        "https://music.163.com/weapi/djradio/" + query["t"].toString(),
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 订阅电台列表
QVariantMap Api::dj_sublist(QVariantMap query) {
    const QVariantMap data {
        { "limit", query.value("limit", 30) },
        { "offset", query.value("offset", 0) },
        { "total", true }
    };
    return request(
        POST,
        "https://music.163.com/weapi/djradio/get/subed",
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 电台详情
QVariantMap Api::dj_subscriber(QVariantMap query) {
    const QVariantMap data {
        { "time", query.value("time", "-1") },
        { "id", query["id"] },
        { "limit", query.value("limit", 20) },
        { "total", true }
    };
    return request(
        POST,
        "https://music.163.com/api/djradio/subscriber",
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 电台今日优选
QVariantMap Api::dj_today_perfered(QVariantMap query) {
    const QVariantMap data {
        { "page", query.value("page", 0) }
    };
    return request(
        POST,
        "https://music.163.com/weapi/djradio/home/today/perfered",
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 电台24小时主播榜
QVariantMap Api::dj_toplist_hours(QVariantMap query) {
    const QVariantMap data {
        { "limit", query.value("limit", 100) }
        // 不支持 offset
    };
    return request(
        POST,
        "https://music.163.com/api/dj/toplist/hours",
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 电台新人榜
QVariantMap Api::dj_toplist_newcomer(QVariantMap query) {
    const QVariantMap data {
        { "limit", query.value("limit", 100) },
        { "offset", query.value("offset", 0) }
    };
    return request(
        POST,
        "https://music.163.com/api/dj/toplist/newcomer",
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 付费精品
QVariantMap Api::dj_toplist_pay(QVariantMap query) {
    const QVariantMap data {
        { "limit", query.value("limit", 100) }
        // 不支持 offset
    };
    return request(
        POST,
        "https://music.163.com/api/djradio/toplist/pay",
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 电台最热主播榜
QVariantMap Api::dj_toplist_popular(QVariantMap query) {
    const QVariantMap data {
        { "limit", query.value("limit", 100) }
        // 不支持 offset
    };
    return request(
        POST,
        "https://music.163.com/api/dj/toplist/popular",
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 新晋电台榜/热门电台榜
QVariantMap Api::dj_toplist(QVariantMap query) {
    const QVariantMap typeMap {
        { "new", 0 },
        { "hot", 1 }
    };
    const QVariantMap data {
        { "limit", query.value("limit", 100) },
        { "offset", query.value("offset", 0) },
        { "type", typeMap.value(query.value("type", "new").toString(), "0") } //0为新晋,1为热门
    };
    return request(
        POST,
        "https://music.163.com/api/djradio/toplist",
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 删除动态
QVariantMap Api::event_del(QVariantMap query) {
    const QVariantMap data {
        { "id", query["evId"] }
    };
    return request(
        POST,
        "https://music.163.com/eapi/event/delete",
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 转发动态
QVariantMap Api::event_forward(QVariantMap query) {
    QVariantMap cookie = query["cookie"].toMap();
    cookie["os"] = "pc";
    query["cookie"] = cookie;
    const QVariantMap data {
        { "forwards", query["forwards"] },
        { "id", query["evId"] },
        { "eventUserId", query["uid"] }
    };
    return request(
        POST,
        "https://music.163.com/weapi/event/forward",
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 动态
QVariantMap Api::event(QVariantMap query) {
    const QVariantMap data {
        { "pagesize", query.value("pagesize", 20) },
        { "lasttime", query.value("lasttime", -1) }
    };
    return request(
        POST,
        "https://music.163.com/weapi/v1/event/get",
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 粉丝年龄比例
QVariantMap Api::fanscenter_basicinfo_age_get(QVariantMap query) {
    const QVariantMap data { };
    return request(
        POST,
        "https://interface.music.163.com/weapi/fanscenter/basicinfo/age/get",
        data,
        {
            { "crypto", "eapi" },
            _PARAM,
            { "url", "/api/fanscenter/basicinfo/age/get" }
        }
        );
}

// 粉丝性别比例
QVariantMap Api::fanscenter_basicinfo_gender_get(QVariantMap query) {
    const QVariantMap data { };
    return request(
        POST,
        "https://interface.music.163.com/weapi/fanscenter/basicinfo/gender/get",
        data,
        {
            { "crypto", "eapi" },
            _PARAM,
            { "url", "/api/fanscenter/basicinfo/gender/get" }
        }
        );
}

// 粉丝省份比例
QVariantMap Api::fanscenter_basicinfo_province_get(QVariantMap query) {
    const QVariantMap data { };
    return request(
        POST,
        "https://interface.music.163.com/weapi/fanscenter/basicinfo/province/get",
        data,
        {
            { "crypto", "eapi" },
            _PARAM,
            { "url", "/api/fanscenter/basicinfo/province/get" }
        }
        );
}

// 粉丝数量
QVariantMap Api::fanscenter_overview_get(QVariantMap query) {
    const QVariantMap data { };
    return request(
        POST,
        "https://interface.music.163.com/weapi/fanscenter/overview/get",
        data,
        {
            { "crypto", "eapi" },
            _PARAM,
            { "url", "/api/fanscenter/overview/get" }
        }
        );
}

// 粉丝来源
QVariantMap Api::fanscenter_trend_list(QVariantMap query) {
    const QVariantMap data {
        { "startTime", query.value("startTime", QDateTime::currentDateTime().toMSecsSinceEpoch() - 7 * 24 * 3600 * 1000) },
        { "endTime", query.value("endTime", QDateTime::currentDateTime().toMSecsSinceEpoch()) },
        { "type", query.value("type", 0) }//新增关注:0 新增取关:1
    };
    return request(
        POST,
        "https://interface.music.163.com/weapi/fanscenter/trend/list",
        data,
        {
            { "crypto", "eapi" },
            _PARAM,
            { "url", "/api/fanscenter/trend/list" }
        }
        );
}

// 垃圾桶
QVariantMap Api::fm_trash(QVariantMap query) {
    const QVariantMap data {
        { "songId", query["id"] }
    };
    return request(
        POST,
        "https://music.163.com/weapi/radio/trash/add?alg=RT&songId="
            + query["id"].toString()
            + QStringLiteral("&time=")
            + query.value("time", 25).toString(),
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 关注与取消关注用户
QVariantMap Api::follow(QVariantMap query) {
    QVariantMap cookie = query["cookie"].toMap();
    cookie["os"] = "pc";
    query["cookie"] = cookie;
    query["t"] = query["t"].toInt() == 1 ? "follow" : "delfollow";
    return request(
        POST,
        "https://music.163.com/weapi/user/"
            + query["t"].toString()
            + QStringLiteral("/")
            + query["id"].toString(),
        {},
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 根据nickname获取userid
QVariantMap Api::get_userids(QVariantMap query) {
    const QVariantMap data {
        { "nicknames", query["nicknames"] }
    };
    return request(
        POST,
        "https://music.163.com/api/user/getUserIds",
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 历史每日推荐歌曲详情
QVariantMap Api::history_recommend_songs_detail(QVariantMap query) {
    QVariantMap cookie = query["cookie"].toMap();
    cookie["os"] = "ios";
    query["cookie"] = cookie;
    const QVariantMap data {
        { "date", query.value("date", "") }
    };
    return request(
        POST,
        "https://music.163.com/api/discovery/recommend/songs/history/detail",
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 历史每日推荐歌曲
QVariantMap Api::history_recommend_songs(QVariantMap query) {
    QVariantMap cookie = query["cookie"].toMap();
    cookie["os"] = "ios";
    query["cookie"] = cookie;
    const QVariantMap data { };
    return request(
        POST,
        "https://music.163.com/api/discovery/recommend/songs/history/recent",
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 首页-发现 block page
// 这个接口为移动端接口，首页-发现页，数据结构可以参考 https://github.com/hcanyz/flutter-netease-music-api/blob/master/lib/src/api/uncategorized/bean.dart#L259 HomeBlockPageWrap
// query.refresh 是否刷新数据
QVariantMap Api::homepage_block_page(QVariantMap query) {
    QVariantMap cookie = query["cookie"].toMap();
    cookie["os"] = "ios";
    cookie["appver"] = "8.10.90";
    query["cookie"] = cookie;
    const QVariantMap data {
        { "refresh", query.value("refresh", false) },
        { "cursor", query["cursor"] }
    };
    return request(
        POST,
        "https://music.163.com/api/homepage/block/page",
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 首页-发现 dragon ball
// 这个接口为移动端接口，首页-发现页（每日推荐、歌单、排行榜 那些入口）
// 数据结构可以参考 https://github.com/hcanyz/flutter-netease-music-api/blob/master/lib/src/api/uncategorized/bean.dart#L290 HomeDragonBallWrap
// !需要登录或者游客登录，非登录返回 []
QVariantMap Api::homepage_dragon_ball(QVariantMap query) {
    QVariantMap cookie = query["cookie"].toMap();
    cookie["os"] = "ios";
    cookie["appver"] = "8.10.90";
    query["cookie"] = cookie;
    const QVariantMap data { };
    return request(
        POST,
        "https://music.163.com/eapi/homepage/dragon/ball/static",
        data,
        {
            { "crypto", "eapi" },
            _PARAM,
            { "url", "/api/homepage/dragon/ball/static"}
        }
        );
}

//热门话题
QVariantMap Api::hot_topic(QVariantMap query) {
    const QVariantMap data {
        { "limit", query.value("limit", 20) },
        { "offset", query.value("offset", 0) }
    };
    return request(
        POST,
        "https://music.163.com/api/act/hot",
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 抱一抱评论
QVariantMap Api::hug_comment(QVariantMap query) {
    QVariantMap cookie = query["cookie"].toMap();
    cookie["os"] = "ios";
    cookie["appver"] = "8.10.90";
    query["cookie"] = cookie;
    query["type"] = resourceTypeMap[query.value("type", 0).toString()];
    const QString threadId = query["type"].toString() + query["sid"].toString();
    const QVariantMap data {
        { "targetUserId", query["uid"] },
        { "commentId", query["cid"] },
        { "threadId", threadId }
    };
    return request(
        POST,
        "https://music.163.com/api/v2/resource/comments/hug/listener",
        data,
        {
            { "crypto", "api" },
            _PARAM
        }
        );
}

// 红心与取消红心歌曲
QVariantMap Api::like(QVariantMap query) {
    QVariantMap cookie = query["cookie"].toMap();
    cookie["os"] = "pc";
    cookie["appver"] = "2.9.7";
    query["cookie"] = cookie;
    query["like"] = query["like"].toString() == "false" ? false : true;
    const QVariantMap data {
        { "alg", "itembased" },
        { "trackId", query["id"] },
        { "like", query["like"] },
        { "time", "3" }
    };
    return request(
        POST,
        "https://music.163.com/api/radio/like",
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 喜欢音乐列表
QVariantMap Api::likelist(QVariantMap query) {
    const QVariantMap data {
        { "uid", query["uid"] }
    };
    return request(
        POST,
        "https://music.163.com/weapi/song/like/get",
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 一起听 结束房间
QVariantMap Api::listentogether_end(QVariantMap query) {
    const QVariantMap data {
        { "roomId", query["roomId"] }
    };
    return request(
        POST,
        "http://interface.music.163.com/eapi/listen/together/end/v2",
        data,
        {
            { "crypto", "eapi" },
            _PARAM,
            { "url", "/api/listen/together/end/v2" }
        }
        );
}

// 一起听 发送心跳
QVariantMap Api::listentogether_heatbeat(QVariantMap query) {
    const QVariantMap data {
        { "roomId", query["roomId"] },
        { "songId", query["songId"] },
        { "playStatus", query["playStatus"] },
        { "progress", query["progress"] }
    };
    return request(
        POST,
        "http://interface.music.163.com/eapi/listen/together/heartbeat",
        data,
        {
            { "crypto", "eapi" },
            _PARAM,
            { "url", "/api/listen/together/heartbeat" }
        }
        );
}

// 一起听 发送播放状态
QVariantMap Api::listentogether_play_command(QVariantMap query) {
    const QVariantMap data {
        { "roomId", query["roomId"] },
        { "commandInfo", QJsonDocument::fromVariant(QVariantMap
                            {
                                { "commandType", query["commandType"] },
                                { "progress", query.value("progress", 0) },
                                { "playStatus", query["playStatus"] },
                                { "formerSongId", query["formerSongId"] },
                                { "targetSongId", query["targetSongId"] },
                                { "clientSeq", query["clientSeq"] }
                            }
                            ).toJson(QJsonDocument::Compact) }
    };
    return request(
        POST,
        "http://interface.music.163.com/eapi/listen/together/play/command/report",
        data,
        {
            { "crypto", "eapi" },
            _PARAM,
            { "url", "/api/listen/together/play/command/report" }
        }
        );
}

// 一起听 房间情况
QVariantMap Api::listentogether_room_check(QVariantMap query) {
    const QVariantMap data {
        { "roomId", query["roomId"] }
    };
    return request(
        POST,
        "http://interface.music.163.com/eapi/listen/together/room/check",
        data,
        {
            { "crypto", "eapi" },
            _PARAM,
            { "url", "/api/listen/together/room/check" }
        }
        );
}

// 一起听创建房间
QVariantMap Api::listentogether_room_create(QVariantMap query) {
    const QVariantMap data {
        { "refer", "songplay_more" }
    };
    return request(
        POST,
        "http://interface.music.163.com/eapi/listen/together/room/create",
        data,
        {
            { "crypto", "eapi" },
            _PARAM,
            { "url", "/api/listen/together/room/create" }
        }
        );
}

// 一起听状态
QVariantMap Api::listentogether_status(QVariantMap query) {
    return request(
        POST,
        "https://music.163.com/api/listen/together/status/get",
        {},
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 一起听 更新播放列表
QVariantMap Api::listentogether_sync_list_command(QVariantMap query) {
    const QVariantMap data {
        { "roomId", query["roomId"] },
        { "commandInfo", QJsonDocument::fromVariant(
                            QVariantMap
                            {
                                { "commandType", query["commandType"] },
                                { "version", QVariantList
                                    {
                                        QVariantMap
                                        {
                                            { "userId", query["userId"] },
                                            { "version", query["version"] }
                                        }
                                    }
                                },
                                { "anchorSongId", "" },
                                { "anchorPosition", -1 },
                                { "randomList", query["randomList"].toString().split(",") },
                                { "displayList", query["displayList"].toString().split(",") }
                            }
                            ).toJson(QJsonDocument::Compact) }
    };
    return request(
        POST,
        "http://interface.music.163.com/eapi/listen/together/sync/list/command/report",
        data,
        {
            { "crypto", "eapi" },
            _PARAM,
            { "url", "/api/listen/together/sync/list/command/report" }
        }
        );
}

// 一起听 当前列表获取
QVariantMap Api::listentogether_sync_playlist_get(QVariantMap query) {
    const QVariantMap data {
        { "roomId", query["roomId"] }
    };
    return request(
        POST,
        "http://interface.music.163.com/eapi/listen/together/sync/playlist/get",
        data,
        {
            { "crypto", "eapi" },
            _PARAM,
            { "url", "/api/listen/together/sync/playlist/get" }
        }
        );
}

// 手机登录
QVariantMap Api::login_cellphone(QVariantMap query) {
    QVariantMap cookie = query["cookie"].toMap();
    cookie["os"] = "ios";
    cookie["appver"] = "8.10.90";
    query["cookie"] = cookie;
    const QVariantMap data {
        { "phone", query["phone"] },
        { "countrycode", query.value("countrycode", 86) },
//        { "captcha", query["captcha"] },
        { query.contains("captcha") ? "captcha" : "password",
         query.value("captcha",
                     query.value("md5_password",
                                 QCryptographicHash::hash(query["password"].toString().toUtf8(), QCryptographicHash::Md5).toHex())) },
        { "rememberLogin", "true" }
    };
    QVariantMap result = request(
        POST,
        "https://music.163.com/weapi/login/cellphone",
        data,
        {
            { "crypto", "weapi" },
            { "ua", "pc" },
            _PARAM
        }
        );
    if(result["body"].toMap()["code"].toInt() == 200) {
        auto body = result["body"].toMap();
        body["cookie"] = result["cookie"];
        result = QVariantMap {
            { "status", 200 },
            { "body", body },
            { "cookie", result["cookie"] }
        };
    }
    return result;
}

// 二维码检测扫码状态接口
QVariantMap Api::login_qr_check(QVariantMap query) {
    const QVariantMap data {
        { "key", query["key"] },
        { "type", 1 }
    };
    QVariantMap result = request(
        POST,
        "https://music.163.com/weapi/login/qrcode/client/login",
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
    auto body = result["body"].toMap();
    body["cookie"] = result["cookie"];
    result = QVariantMap {
        { "status", 200 },
        { "body", body },
        { "cookie", result["cookie"] }
    };
    return result;
}

// 二维码 key 生成接口
QVariantMap Api::login_qr_key(QVariantMap query) {
    const QVariantMap data {
        { "type", 1 }
    };
    QVariantMap result = request(
        POST,
        "https://music.163.com/weapi/login/qrcode/unikey",
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
    result = QVariantMap {
        { "status", 200 },
        { "body", QVariantMap {
                     { "data", result["body"] },
                     { "code", 200}
                 } },
        { "cookie", result["cookie"] }
    };
    return result;
}

// 二维码生成接口
QVariantMap Api::login_qr_create(QVariantMap query) {
    const QString url = "https://music.163.com/login?codekey=" + query["key"].toString();
    auto result = QVariantMap {
        { "code", 200 },
        { "status", 200 },
        { "body", QVariantMap {
                     { "code", 200},
                     { "data", QVariantMap {
                                  { "qrurl", url }
                              } }
                 } }
    };
    return result;
}

// 登录刷新
QVariantMap Api::login_refresh(QVariantMap query) {
    QVariantMap result = request(
        POST,
        "https://music.163.com/weapi/login/token/refresh",
        {},
        {
            { "crypto", "weapi" },
            { "ua", "pc" },
            _PARAM
        }
        );
    if(result["body"].toMap()["code"].toInt() == 200) {
        auto body = result["body"].toMap();
        body["cookie"] = result["cookie"];
        result = QVariantMap {
            { "status", 200 },
            { "body", body },
            { "cookie", result["cookie"] }
        };
    }
    return result;
}

// 登录状态
QVariantMap Api::login_status(QVariantMap query) {
    QVariantMap result = request(
        POST,
        "https://music.163.com/weapi/w/nuser/account/get",
        {},
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
    if(result["body"].toMap()["code"].toInt() == 200) {
        auto body = result["body"].toMap();
        body["cookie"] = result["cookie"];
        result = QVariantMap {
            { "status", 200 },
            { "body", QVariantMap
                {
                    { "data", result["body"] }
                }
            },
            { "cookie", result["cookie"] }
        };
    }
    return result;
}

// 退出登录
QVariantMap Api::logout(QVariantMap query) {
    return request(
        POST,
        "https://music.163.com/weapi/logout",
        {},
        {
            { "crypto", "eapi" },
            { "ua", "pc" },
            _PARAM
        }
        );
}

// 新版歌词 - 包含逐字歌词
QVariantMap Api::lyric_new(QVariantMap query) {
    const QVariantMap data {
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
    return request(
        POST,
        "https://interface3.music.163.com/eapi/song/lyric/v1",
        data,
        {
            { "crypto", "eapi" },
            _PARAM,
            { "url", "/api/song/lyric/v1" }
        }
        );
}

// 歌词
QVariantMap Api::lyric(QVariantMap query) {
    QVariantMap cookie = query["cookie"].toMap();
    cookie["os"] = "ios";
    query["cookie"] = cookie;

    const QVariantMap data {
        { "id", query["id"] },
        { "tv", -1 },
        { "lv", -1 },
        { "rv", -1 },
        { "kv", -1 }
    };
    return request(
        POST,
        "https://music.163.com/api/song/lyric?_nmclfl=1",
        data,
        {
            { "crypto", "api" },
            _PARAM
        }
        );
}

// 重复昵称检测
QVariantMap Api::nickname_check(QVariantMap query) {
    QVariantMap data {
        { "nickname", query["nickname"] }
    };
    return request(
        POST,
        "https://music.163.com/api/nickname/duplicated",
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 游客登录
QVariantMap Api::register_anonimous(QVariantMap query) {
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
    const QVariantMap data {
                              /* A base64 encoded string. */
                              { "username", username },
                              };

    QVariantMap result = request(
        POST,
        "https://music.163.com/api/register/anonimous",
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
    if(result["body"].toMap()["code"].toInt() == 200) {
        auto body = result["body"].toMap();
        body["cookie"] = result["cookie"].toStringList().join(';');
        result = QVariantMap {
            { "status", 200 },
            { "body", body },
            { "cookie", result["cookie"] }
        };
    }
    return result;
}

// 歌单封面上传
QVariantMap Api::playlist_cover_update(QVariantMap query) {
    if (!query.contains("imgFile")) {
        return {
            { "status", 400 },
            { "body", QVariantMap {
                                 { "code", 400 },
                                 { "msg", "imgFile is required" },
                                 } }
        };
    }
    const auto uploadInfo = uploadPlugin(query);
    if (uploadInfo.isEmpty()) return {};
    const auto res = request(
        POST,
        "https://music.163.com/weapi/playlist/cover/update",
        {
            { "id", query["id"] },
            { "coverImgId", uploadInfo["imgId"] }
        },
        {
            { "crypto", "weapi" },
            { "ua", query.value("ua", "") },
            _PARAM
        }
        );
    return {
        { "status", 200 },
        { "body", QVariantMap {
                             { "code", 200 },
                             { "data", QCloudMusicApi::Index::mergeMap(uploadInfo, res["body"].toMap()) },
                             } }
    };
}

// 相关歌单
QVariantMap Api::related_playlist(QVariantMap query) {
    QVariantMap result = request(
        GET,
        "https://music.163.com/playlist?id=" + query["id"].toString(),
        {},
        {
            { "ua", "pc" },
            _PARAM
        }
        );
    QRegularExpression pattern("<div class=\"cver u-cover u-cover-3\">[\\s\\S]*?<img src=\"([^\"]+)\">[\\s\\S]*?<a class=\"sname f-fs1 s-fc0\" href=\"([^\"]+)\"[^>]*>([^<]+?)<\\/a>[\\s\\S]*?<a class=\"nm nm f-thide s-fc3\" href=\"([^\"]+)\"[^>]*>([^<]+?)<\\/a>");
    QRegularExpressionMatchIterator it = pattern.globalMatch(result["body"].toString());
    QJsonArray playlists;
    while (it.hasNext()) {
        auto result = it.next().capturedTexts();
        playlists.push_back(
            QJsonValue::fromVariant(
                QVariantMap {
                    {
                        "creator", {
                            QVariantMap {
                                { "userId", result[4].split("/user/home?id=")[1] },
                                { "nickname", result[5] }
                            }
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
                }
                )
            );
    }
    result["body"] = QVariantMap {
        { "code", 200 },
        { "playlists", QVariant(playlists) }
    };
    return result;
}

// 搜索
QVariantMap Api::search(QVariantMap query) {
    if (query.contains("type") && query["type"].toString() == "2000") {
        const QVariantMap data {
            { "keyword", query["keywords"] },
            { "scene", "normal"},
            { "limit", query.value("limit", 20) },
            { "offset", query.value("offset", 0) }
        };
        return request(
            POST,
            "https://music.163.com/api/search/voice/get",
            data,
            {
                { "crypto", "weapi" },
                _PARAM
            }
            );
    }
    const QVariantMap data {
        { "s", query["keywords"] },
        { "type", query.value("type", 1) },// 1: 单曲, 10: 专辑, 100: 歌手, 1000: 歌单, 1002: 用户, 1004: MV, 1006: 歌词, 1009: 电台, 1014: 视频
        { "limit", query.value("limit", 20) },
        { "offset", query.value("offset", 0) }
    };
    return request(
        POST,
        "https://music.163.com/weapi/search/get",
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 获取客户端歌曲下载链接
QVariantMap Api::song_download_url(QVariantMap query) {
    QVariantMap data {
        { "id", query["id"] },
        { "br", query.value("br", 999000) }
    };
    return request(
        POST,
        "https://interface.music.163.com/eapi/song/enhance/download/url",
        data,
        {
            { "crypto", "eapi" },
            _PARAM,
            { "url", "/api/song/enhance/download/url" }
        }
        );
}

// 歌曲链接 - v1
// 此版本不再采用 br 作为音质区分的标准
// 而是采用 standard, exhigh, lossless, hires, jyeffect(高清环绕声), sky(沉浸环绕声), jymaster(超清母带) 进行音质判断
QVariantMap Api::song_url_v1(QVariantMap query) {
    QVariantMap data {
        { "ids", "[" + query["id"].toString() + "]" },
        { "level", query["level"].toString() },
        { "encodeType", "flac" }
    };
    if(data["level"].toString() == "sky") {
        data["immerseType"] = "c51";
    }
    return request(
        POST,
        "https://interface.music.163.com/eapi/song/enhance/player/url/v1",
        data,
        {
            { "crypto", "eapi" },
            _PARAM,
            { "url", "/api/song/enhance/player/url/v1" }
        }
        );
}

// 音乐百科基础信息
QVariantMap Api::song_wiki_summary(QVariantMap query) {
    QVariantMap data {
        { "songId", query["id"].toInt() }
    };
    return request(
        POST,
        "https://interface3.music.163.com/eapi/music/wiki/home/song/get",
        data,
        {
            { "crypto", "eapi" },
            _PARAM,
            { "url", "/api/song/play/about/block/page" }
        }
        );
}

// 年度听歌报告2017-2022
QVariantMap Api::summary_annual(QVariantMap query) {
    QVariantMap data { };
    const QString key = QStringList { "2017", "2018", "2019 "}.indexOf(query["year"].toString()) > -1 ? "userdata" : "data";
    return request(
        POST,
        "https://music.163.com/weapi/activity/summary/annual/" + query["year"].toString() + "/" + key,
        data,
        {
            { "crypto", "eapi" },
            _PARAM,
            { "url", "/api/activity/summary/annual/" + query["year"].toString() + "/" + key }
        }
        );
}

// 获取达人达标信息
QVariantMap Api::threshold_detail_get(QVariantMap query) {
    const QVariantMap data { };
    return request(
        POST,
        "https://music.163.com/weapi/influencer/web/apply/threshold/detail/get",
        data,
        {
            { "crypto", "eapi" },
            _PARAM,
            { "url", "/api/influencer/web/apply/threshold/detail/get" }
        }
        );
}

// 所有榜单介绍
QVariantMap Api::toplist(QVariantMap query) {
    return request(
        POST,
        "https://music.163.com/api/toplist",
        {},
        {
            { "crypto", "api" },
            _PARAM
        }
        );
}

// 专辑简要百科信息
QVariantMap Api::ugc_album_get(QVariantMap query) {
    const QVariantMap data {
        { "albumId", query["id"] }
    };
    return request(
        POST,
        "https://music.163.com/weapi/rep/ugc/album/get",
        data,
        {
            { "crypto", "eapi" },
            _PARAM,
            { "url", "/api/rep/ugc/album/get" }
        }
        );
}

// 歌手简要百科信息
QVariantMap Api::ugc_artist_get(QVariantMap query) {
    const QVariantMap data {
        { "artistId", query["id"] }
    };
    return request(
        POST,
        "https://music.163.com/weapi/rep/ugc/artist/get",
        data,
        {
            { "crypto", "eapi" },
            _PARAM,
            { "url", "/api/rep/ugc/artist/get" }
        }
        );
}

// 搜索歌手
// 可传关键字或者歌手id
QVariantMap Api::ugc_artist_search(QVariantMap query) {
    const QVariantMap data {
        { "keyword", query["keyword"] },
        { "limit", query.value("limit", 40) }
    };
    return request(
        POST,
        "https://music.163.com/api/rep/ugc/artist/search",
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// mv简要百科信息
QVariantMap Api::ugc_mv_get(QVariantMap query) {
    const QVariantMap data {
        { "mvId", query["id"] }
    };
    return request(
        POST,
        "https://music.163.com/weapi/rep/ugc/mv/get",
        data,
        {
            { "crypto", "eapi" },
            _PARAM,
            { "url", "/api/rep/ugc/mv/get" }
        }
        );
}

// 歌曲简要百科信息
QVariantMap Api::ugc_song_get(QVariantMap query) {
    const QVariantMap data {
        { "songId", query["id"] }
    };
    return request(
        POST,
        "https://music.163.com/weapi/rep/ugc/song/get",
        data,
        {
            { "crypto", "eapi" },
            _PARAM,
            { "url", "/api/rep/ugc/song/get" }
        }
        );
}

// 获取账号信息
QVariantMap Api::user_account(QVariantMap query) {
    const QVariantMap data {};
    return request(
        POST,
        "https://music.163.com/api/nuser/account/get",
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 用户创建的电台
QVariantMap Api::user_audio(QVariantMap query) {
    const QVariantMap data {
        { "userId", query["uid"] }
    };
    return request(
        POST,
        "https://music.163.com/weapi/djradio/get/byuser",
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 获取用户绑定信息
QVariantMap Api::user_binding(QVariantMap query) {
    const QVariantMap data { };
    return request(
        POST,
        "https://music.163.com/api/v1/user/bindings/" + query["uid"].toString(),
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 云盘歌曲删除
QVariantMap Api::user_cloud_del(QVariantMap query) {
    const QVariantMap data {
        { "songIds", QVariantList{ query["id"] } }
    };
    return request(
        POST,
        "https://music.163.com/weapi/cloud/del",
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 云盘数据详情
QVariantMap Api::user_cloud_detail(QVariantMap query) {
    const auto id = query["id"].toString().replace("\\s", "").split(",");
    const QVariantMap data {
        { "songIds", id }
    };
    return request(
        POST,
        "https://music.163.com/weapi/v1/cloud/get/byids",
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 云盘数据
QVariantMap Api::user_cloud(QVariantMap query) {
    const QVariantMap data {
        { "limit", query.value("limit", 30) },
        { "offset", query.value("offset", 0) }
    };
    return request(
        POST,
        "https://music.163.com/api/v1/cloud/get",
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 获取用户历史评论
QVariantMap Api::user_comment_history(QVariantMap query) {
    QVariantMap cookie = query["cookie"].toMap();
    cookie["os"] = "ios";
    cookie["appver"] = "8.10.90";
    query["cookie"] = cookie;
    const QVariantMap data {
        { "compose_reminder", "true" },
        { "compose_hot_comment", "true" },
        { "limit", query.value("limit", 10) },
        { "user_id", query["uid"] },
        { "time", query.value("time", 0) }
    };
    return request(
        POST,
        "https://music.163.com/api/comment/user/comment/history",
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 用户详情
QVariantMap Api::user_detail(QVariantMap query) {
    return request(
        POST,
        "https://music.163.com/weapi/v1/user/detail/" + query["uid"].toString(),
        {},
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 用户电台节目
QVariantMap Api::user_dj(QVariantMap query) {
    const QVariantMap data {
        { "limit", query.value("limit", 30) },
        { "offset", query.value("offset", 0) }
    };
    return request(
        POST,
        "https://music.163.com/weapi/dj/program/" + query["uid"].toString(),
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 用户动态
QVariantMap Api::user_event(QVariantMap query) {
    QVariantMap cookie = query["cookie"].toMap();
    cookie["os"] = "ios";
    cookie["appver"] = "8.10.90";
    query["cookie"] = cookie;
    const QVariantMap data {
        { "getcounts", true },
        { "time", query.value("lasttime", -1) },
        { "limit", query.value("limit", 30) },
        { "total", false }
    };
    return request(
        POST,
        "https://music.163.com/api/event/get/" + query["uid"].toString(),
        data,
        {
            { "crypto", "api" },
            _PARAM
        }
        );
}

// 关注TA的人(粉丝)
QVariantMap Api::user_followeds(QVariantMap query) {
    const QVariantMap data {
        { "userId", query["uid"] },
        { "time", "0" },
        { "limit", query.value("limit", 30) },
        { "offset", query.value("offset", 0) },
        { "getcounts", "true" }
    };
    return request(
        POST,
        "https://music.163.com/eapi/user/getfolloweds/" + query["uid"].toString(),
        data,
        {
            { "crypto", "eapi" },
            _PARAM,
            { "url", "/api/user/getfolloweds" }
        }
        );
}

// TA关注的人(关注)
QVariantMap Api::user_follows(QVariantMap query) {
    const QVariantMap data {
        { "offset", query.value("offset", 0) },
        { "limit", query.value("limit", 30) },
        { "order", true }
    };
    return request(
        POST,
        "https://music.163.com/weapi/user/getfollows/" + query["uid"].toString(),
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 获取用户等级信息
QVariantMap Api::user_level(QVariantMap query) {
    const QVariantMap data { };
    return request(
        POST,
        "https://music.163.com/weapi/user/level",
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 用户歌单
QVariantMap Api::user_playlist(QVariantMap query) {
    const QVariantMap data {
        { "uid", query["uid"] },
        { "limit", query.value("limit", 30) },
        { "offset", query.value("offset", 0) },
        { "includeVideo", true },
        };
    return request(
        POST,
        "https://music.163.com/api/user/playlist",
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 听歌排行
QVariantMap Api::user_record(QVariantMap query) {
    const QVariantMap data {
        { "uid", query["uid"] },
        { "type", query.value("type", 0) } // 1: 最近一周, 0: 所有时间
    };
    return request(
        POST,
        "https://music.163.com/weapi/v1/play/record",
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 用户绑定手机
QVariantMap Api::user_replacephone(QVariantMap query) {
    const QVariantMap data {
        { "phone", query["phone"] },
        { "captcha", query["captcha"] },
        { "oldcaptcha", query["oldcaptcha"] },
        { "countrycode", query.value("countrycode", 86) }
    };
    return request(
        POST,
        "https://music.163.com/api/user/replaceCellphone",
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 收藏计数
QVariantMap Api::user_subcount(QVariantMap query) {
    return request(
        POST,
        "https://music.163.com/weapi/subcount",
        {},
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 编辑用户信息
QVariantMap Api::user_update(QVariantMap query) {
    QVariantMap cookie = query["cookie"].toMap();
    cookie["os"] = "ios";
    cookie["appver"] = "8.10.90";
    query["cookie"] = cookie;
    const QVariantMap data {
        { "birthday", query["birthday"] },
        { "city", query["city"] },
        { "gender", query["gender"] },
        { "nickname", query["nickname"] },
        { "province", query["province"] },
        { "signature", query["signature"] },
    };
    return request(
        POST,
        "https://music.163.com/api/user/profile/update",
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 云贝 todo 任务
QVariantMap Api::yunbei_tasks_todo(QVariantMap query) {
    const QVariantMap data {};
    // /api/point/today/get
    return request(
        POST,
        "https://music.163.com/api/usertool/task/todo/query",
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 云贝所有任务
QVariantMap Api::yunbei_tasks(QVariantMap query) {
    const QVariantMap data {};
    // /api/point/today/get
    return request(
        POST,
        "https://music.163.com/api/usertool/task/list/all",
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 云贝今日签到信息
QVariantMap Api::yunbei_today(QVariantMap query) {
    const QVariantMap data {};
    // /api/point/today/get
    return request(
        POST,
        "https://music.163.com/api/point/today/get",
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 云贝
QVariantMap Api::yunbei(QVariantMap query) {
    const QVariantMap data {};
    // /api/point/today/get
    return request(
        POST,
        "https://music.163.com/api/point/signed/get",
        data,
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

