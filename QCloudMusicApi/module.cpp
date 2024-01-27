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
#include "module.h"

//入参与返回值类型为QVariantMap
#define APICPP(FUNCNAME) \
const QVariantMap NeteaseCloudMusicApi::FUNCNAME(QVariantMap query)

//定义一些重复参数
#define _PARAM \
{ "cookie", query["cookie"] }, \
{ "proxy", query["proxy"] }, \
{ "realIP", query["realIP"] } \

const static auto &request = Request::createRequest;
const static auto &POST = QNetworkAccessManager::PostOperation;
const static auto &GET = QNetworkAccessManager::GetOperation;
const static auto &resourceTypeMap = Config::resourceTypeMap;

// 初始化名字
APICPP(activate_init_profile) {
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
APICPP(album_detail_dynamic) {
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
APICPP(album_detail) {
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
APICPP(album_list_style) {
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
APICPP(album_list) {
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
APICPP(album_new) {
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
APICPP(album_newest) {
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
APICPP(album_songsaleboard) {
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
APICPP(album_sub) {
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
APICPP(album_sublist) {
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
APICPP(album) {
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
APICPP(artist_album) {
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
APICPP(artist_desc) {
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
APICPP(artist_detail) {
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
APICPP(artist_fans) {
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
APICPP(artist_follow_count) {
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
APICPP(artist_list) {
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
APICPP(artist_mv) {
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
APICPP(artist_new_mv) {
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
APICPP(artist_new_song) {
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
APICPP(artist_songs) {
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
APICPP(artist_sub) {
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
APICPP(artist_sublist) {
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
APICPP(artist_top_song) {
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
APICPP(artist_video) {
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
APICPP(artists) {
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
APICPP(batch) {
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
APICPP(banner) {
    const auto type0 = query.value("type", 0).toInt();
    const QMap<int, QString> typeMap = {
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
APICPP(calendar) {
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
APICPP(captcha_sent) {
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
APICPP(captcha_verify) {
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
APICPP(cellphone_existence_check) {
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
APICPP(check_music) {
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
APICPP(cloud_match) {
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

// 搜索
APICPP(cloudsearch) {
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
APICPP(comment_album) {
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
APICPP(comment_dj) {
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
APICPP(comment_event) {
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
APICPP(comment_floor) {
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
APICPP(comment_hot) {
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
APICPP(comment_hug_list) {
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
APICPP(comment_like) {
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
APICPP(comment_music) {
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
APICPP(comment_mv) {
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
APICPP(comment_new) {
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
APICPP(comment_playlist) {
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
APICPP(comment_video) {
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
APICPP(comment) {
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
APICPP(countries_code_list) {
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
APICPP(creator_authinfo_get) {
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
APICPP(daily_signin) {
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
APICPP(digitalAlbum_detail) {
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
APICPP(digitalAlbum_ordering) {
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
APICPP(digitalAlbum_purchased) {
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
APICPP(digitalAlbum_sales) {
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
APICPP(dj_banner) {
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
APICPP(dj_category_excludehot) {
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
APICPP(dj_category_recommend) {
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
APICPP(dj_catelist) {
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
APICPP(dj_detail) {
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
APICPP(dj_hot) {
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
APICPP(dj_paygift) {
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
APICPP(dj_personalize_recommend) {
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
APICPP(dj_program_detail) {
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
APICPP(dj_program_toplist_hours) {
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
APICPP(dj_program_toplist) {
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
APICPP(dj_program) {
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
APICPP(dj_radio_hot) {
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
APICPP(dj_recommend_type) {
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
APICPP(dj_recommend) {
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
APICPP(dj_sub) {
    query["t"] = query["t"].toInt() == 1 ? "sub" : "unsub";
    const QVariantMap data {
        { "id", query["rid"] }
    };
    return request(
        POST,
        "https://music.163.com/weapi/djradio/" + query["t"].toString(),
        {},
        {
            { "crypto", "weapi" },
            _PARAM
        }
        );
}

// 订阅电台列表
APICPP(dj_sublist) {
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
APICPP(dj_subscriber) {
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
APICPP(dj_today_perfered) {
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
APICPP(dj_toplist_hours) {
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
APICPP(dj_toplist_newcomer) {
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
APICPP(dj_toplist_pay) {
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
APICPP(dj_toplist_popular) {
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
APICPP(dj_toplist) {
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
APICPP(event_del) {
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
APICPP(event_forward) {
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
APICPP(event) {
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
APICPP(fanscenter_basicinfo_age_get) {
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
APICPP(fanscenter_basicinfo_gender_get) {
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
APICPP(fanscenter_basicinfo_province_get) {
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
APICPP(fanscenter_overview_get) {
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
APICPP(fanscenter_trend_list) {
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
APICPP(fm_trash) {
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
APICPP(follow) {
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
APICPP(get_userids) {
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
APICPP(history_recommend_songs_detail) {
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
APICPP(history_recommend_songs) {
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

// 手机登录
APICPP(login_cellphone) {
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
APICPP(login_qr_check) {
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
APICPP(login_qr_key) {
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
APICPP(login_qr_create) {
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
APICPP(login_refresh) {
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
APICPP(login_status) {
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
            { "body", {
                         { "data", result["body"] }
                     } },
            { "cookie", result["cookie"] }
        };
    }
    return result;
}

// 退出登录
APICPP(logout) {
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
APICPP(lyric_new) {
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
APICPP(lyric) {
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
APICPP(nickname_check) {
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
APICPP(register_anonimous) {
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

// 相关歌单
APICPP(related_playlist) {
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
APICPP(search) {
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
APICPP(song_download_url) {
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
APICPP(song_url_v1) {
    QVariantMap data {
        { "ids", query["id"].toList() },
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
APICPP(song_wiki_summary) {
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
APICPP(summary_annual) {
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
APICPP(threshold_detail_get) {
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
APICPP(toplist) {
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
APICPP(ugc_album_get) {
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
APICPP(ugc_artist_get) {
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
APICPP(ugc_artist_search) {
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
APICPP(ugc_mv_get) {
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
APICPP(ugc_song_get) {
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
APICPP(user_account) {
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

// 获取用户历史评论
APICPP(user_comment_history) {
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
APICPP(user_detail) {
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

// 用户歌单
APICPP(user_playlist) {
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

// 云贝 todo 任务
APICPP(yunbei_tasks_todo) {
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
APICPP(yunbei_tasks) {
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
APICPP(yunbei_today) {
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
APICPP(yunbei) {
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

