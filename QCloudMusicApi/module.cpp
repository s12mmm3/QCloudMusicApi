#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkInterface>
#include <QVariantMap>
#include <QRegularExpression>
#include <QRandomGenerator>
#include <QCryptographicHash>

#include <algorithm>

#include "util/config.h"
#include "util/request.h"
#include "util/index.h"
#include "util/crypto.h"
#include "module.h"
#include "plugins.h"

// 定义重复参数
#define _PARAM \
{ "cookie", query["cookie"] }, \
{ "ua", query.value("ua", "") }, \
{ "proxy", query["proxy"] }, \
{ "realIP", query["realIP"] } \

#define _WEAPI \
{ "crypto", "weapi" }, \
_PARAM \

using Api = NeteaseCloudMusicApi;
const static auto& request = QCloudMusicApi::Request::createRequest;
const static auto& POST = QNetworkAccessManager::PostOperation;
const static auto& GET = QNetworkAccessManager::GetOperation;
const static auto& resourceTypeMap = QCloudMusicApi::Config::resourceTypeMap;

using namespace QCloudMusicApi;
NeteaseCloudMusicApi::NeteaseCloudMusicApi(QObject* parent)
    : QObject{ parent }
{}

// 初始化名字
QVariantMap Api::activate_init_profile(QVariantMap query) {
    QVariantMap data{
        { "nickname", query["nickname"] }
    };
    return request(
        POST,
        "/api/activate/initProfile",
        data,
        Request::options(query)
    );
}

// 私人 DJ

// 实际请求参数如下, 部分内容省略, 敏感信息已进行混淆
// 可按需修改此 API 的代码
/* {"extInfo":"{\"lastRequestTimestamp\":1692358373509,\"lbsInfoList\":[{\"lat\":40.23076381,\"lon\":129.07545186,\"time\":1692358543},{\"lat\":40.23076381,\"lon\":129.07545186,\"time\":1692055283}],\"listenedTs\":false,\"noAidjToAidj\":true}","header":"{}","e_r":true} */
QVariantMap Api::aidj_content_rcmd(QVariantMap query) {
    QVariantMap extInfo{};
    if (query.contains("latitude")) {
        extInfo["lbsInfoList"] = QVariantList{
            QVariantMap {
                { "lat", query["latitude"] },
                { "lon", query["longitude"] },
                { "time", QDateTime::currentSecsSinceEpoch() }
            }
        };
    }
    extInfo["noAidjToAidj"] = false;
    extInfo["lastRequestTimestamp"] = QDateTime::currentMSecsSinceEpoch();
    extInfo["listenedTs"] = false;
    const QVariantMap data{
        { "extInfo", QJsonDocument::fromVariant(extInfo).toJson() }
    };
    return request(
        POST,
        "/api/aidj/content/rcmd/info",
        data,
        Request::options(query)
    );
}

// 专辑动态信息
QVariantMap Api::album_detail_dynamic(QVariantMap query) {
    const QVariantMap data{
        { "id", query["id"] }
    };
    return request(
        POST,
        "/api/album/detail/dynamic",
        data,
        Request::options(query)
    );
}

// 数字专辑详情
QVariantMap Api::album_detail(QVariantMap query) {
    const QVariantMap data{
        { "id", query["id"] }
    };
    return request(
        POST,
        "https://music.163.com/weapi/vipmall/albumproduct/detail",
        data,
        {
            _WEAPI
        }
    );
}

// 数字专辑-语种风格馆
QVariantMap Api::album_list_style(QVariantMap query) {
    const QVariantMap data{
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
            _WEAPI
        }
    );
}

// 数字专辑-新碟上架
QVariantMap Api::album_list(QVariantMap query) {
    const QVariantMap data{
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
            _WEAPI
        }
    );
}

// 全部新碟
QVariantMap Api::album_new(QVariantMap query) {
    const QVariantMap data{
        { "limit", query.value("limit", 30) },
        { "offset", query.value("offset", 0) },
        { "total", true },
        { "area", query.value("area", "ALL") },//ALL:全部,ZH:华语,EA:欧美,KR:韩国,JP:日本
    };
    return request(
        POST,
        "/api/album/new",
        data,
        Request::options(query)
    );
}

// 最新专辑
QVariantMap Api::album_newest(QVariantMap query) {
    return request(
        POST,
        "https://music.163.com/api/discovery/newAlbum",
        {},
        {
            _WEAPI
        }
        );
}

// 获取专辑歌曲的音质
QVariantMap Api::album_privilege(QVariantMap query) {
    const QVariantMap data{
        { "id", query["id"] }
    };
    return request(
        POST,
        "/api/album/privilege",
        data,
        Request::options(query)
    );
}

// 数字专辑&数字单曲-榜单
QVariantMap Api::album_songsaleboard(QVariantMap query) {
    QVariantMap data{
        { "albumType", query.value("albumType", 0) } //0为数字专辑,1为数字单曲
    };
    const QString type = query.value("type", "daily").toString(); // daily,week,year,total
    if (type == "year") data["year"] = query["year"];
    return request(
        POST,
        "https://music.163.com/api/feealbum/songsaleboard/" + type + "/type",
        data,
        {
            _WEAPI
        }
    );
}

// 收藏/取消收藏专辑
QVariantMap Api::album_sub(QVariantMap query) {
    query["t"] = query["t"] == 1 ? "sub" : "unsub";
    const QVariantMap data{
        { "id", query["id"] }
    };
    return request(
        POST,
        "https://music.163.com/api/album/" + query["t"].toString(),
        data,
        {
            _WEAPI
        }
    );
}

// 已收藏专辑列表
QVariantMap Api::album_sublist(QVariantMap query) {
    const QVariantMap data{
        { "limit", query.value("limit", 25) },
        { "offset", query.value("offset", 0) },
        { "total", true }
    };
    return request(
        POST,
        "https://music.163.com/weapi/album/sublist",
        data,
        {
            _WEAPI
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
            _WEAPI
        }
        );
}

// 歌手专辑列表
QVariantMap Api::artist_album(QVariantMap query) {
    const QVariantMap data{
        { "limit", query.value("limit", 30) },
        { "offset", query.value("offset", 0) },
        { "total", true }
    };
    return request(
        POST,
        "https://music.163.com/weapi/artist/albums/" + query["id"].toString(),
        data,
        {
            _WEAPI
        }
    );
}

// 歌手介绍
QVariantMap Api::artist_desc(QVariantMap query) {
    const QVariantMap data{
        { "id", query["id"] }
    };
    return request(
        POST,
        "https://music.163.com/weapi/artist/introduction",
        data,
        {
            _WEAPI
        }
    );
}

// 歌手动态信息
QVariantMap Api::artist_detail_dynamic(QVariantMap query) {
    const QVariantMap data{
        { "id", query["id"] }
    };
    return request(
        POST,
        "/api/artist/detail/dynamic",
        data,
        Request::options(query)
    );
}

// 歌手详情
QVariantMap Api::artist_detail(QVariantMap query) {
    const QVariantMap data{
        { "id", query["id"] }
    };
    return request(
        POST,
        "https://music.163.com/api/artist/head/info/get",
        data,
        {
            _WEAPI
        }
    );
}

// 歌手粉丝
QVariantMap Api::artist_fans(QVariantMap query) {
    const QVariantMap data{
        { "id", query["id"] },
        { "limit", query.value("limit", 20) },
        { "offset", query.value("offset", 0) }
    };
    return request(
        POST,
        "https://music.163.com/weapi/artist/fans/get",
        data,
        {
            _WEAPI
        }
    );
}

// 歌手粉丝数量
QVariantMap Api::artist_follow_count(QVariantMap query) {
    const QVariantMap data{
        { "id", query["id"] }
    };
    return request(
        POST,
        "https://music.163.com/weapi/artist/follow/count/get",
        data,
        {
            _WEAPI
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
    const QVariantMap data{
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
            _WEAPI
        }
    );
}

// 歌手相关MV
QVariantMap Api::artist_mv(QVariantMap query) {
    const QVariantMap data{
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
            _WEAPI
        }
    );
}

// 关注歌手新 MV
QVariantMap Api::artist_new_mv(QVariantMap query) {
    const QVariantMap data{
        { "limit", query.value("limit", 20) },
        { "startTimestamp", query.value("before", QDateTime::currentDateTime().toMSecsSinceEpoch()) }
    };
    return request(
        POST,
        "https://music.163.com/api/sub/artist/new/works/mv/list",
        data,
        {
            _WEAPI
        }
    );
}

// 关注歌手新歌
QVariantMap Api::artist_new_song(QVariantMap query) {
    const QVariantMap data{
        { "limit", query.value("limit", 20) },
        { "startTimestamp", query.value("before", QDateTime::currentDateTime().toMSecsSinceEpoch()) }
    };
    return request(
        POST,
        "https://music.163.com/api/sub/artist/new/works/song/list",
        data,
        {
            _WEAPI
        }
    );
}

// 歌手全部歌曲
QVariantMap Api::artist_songs(QVariantMap query) {
    QVariantMap cookie = query["cookie"].toMap();
    cookie["os"] = "pc";
    cookie["appver"] = "2.9.7";
    query["cookie"] = cookie;
    const QVariantMap data{
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
            _WEAPI
        }
    );
}

// 收藏与取消收藏歌手
QVariantMap Api::artist_sub(QVariantMap query) {
    query["t"] = query["t"] == 1 ? "sub" : "unsub";
    const QVariantMap data{
        { "artistId", query["id"] },
        { "artistIds", "[" + query["id"].toString() + "]" }
    };
    return request(
        POST,
        "https://music.163.com/weapi/artist/" + query["t"].toString(),
        data,
        {
            _WEAPI
        }
    );
}

// 关注歌手列表
QVariantMap Api::artist_sublist(QVariantMap query) {
    const QVariantMap data{
        { "limit", query.value("limit", 25) },
        { "offset", query.value("offset", 0) },
        { "total", true }
    };
    return request(
        POST,
        "https://music.163.com/weapi/artist/sublist",
        data,
        {
            _WEAPI
        }
    );
}

// 歌手热门 50 首歌曲
QVariantMap Api::artist_top_song(QVariantMap query) {
    const QVariantMap data{
        { "id", query["id"] }
    };
    return request(
        POST,
        "https://music.163.com/api/artist/top/song",
        data,
        {
            _WEAPI
        }
    );
}

// 歌手相关视频
QVariantMap Api::artist_video(QVariantMap query) {
    const QVariantMap data{
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
            _WEAPI
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
            _WEAPI
        }
        );
}

// 听歌识曲
QVariantMap Api::audio_match(QVariantMap query) {
    auto createRandomString = [](int len) {
        const QString str = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
        QString result;
        for (int i = 0; i < len; ++i) {
            int index = QRandomGenerator::global()->bounded(str.length());
            result.append(str.at(index));
        }
        return result;
        };
    QVariantMap cookie = query["cookie"].toMap();
    cookie["os"] = "pc";
    query["cookie"] = cookie;
    const QVariantMap data{
        { "algorithmCode", "shazam_v2" },
        { "times", 1 },
        { "sessionId", createRandomString(16) },
        { "duration", query["duration"] },
        { "from", "recognize-song" },
        { "decrypt", "1" },
        { "rawdata", query["audioFP"] },
    };
    return request(
        POST,
        "https://music.163.com/api/music/audio/match",
        data,
        {
            _WEAPI
        }
    );
}

// 更新头像
QVariantMap Api::avatar_upload(QVariantMap query) {
    auto uploadInfo = QCloudMusicApi::Plugins::upload(query);
    const QVariantMap res = request(
        POST,
        "https://music.163.com/weapi/user/avatar/upload/v1",
        {
            { "imgid", uploadInfo["imgId"] }
        },
        {
            _WEAPI
        }
        );
    return {
        { "status", 200 },
        { "body", QVariantMap
            {
                { "code", 200 },
                { "data", QCloudMusicApi::Index::mergeMap(uploadInfo, res["body"].toMap()) },
            }
        }
    };
}

// 批量请求接口
QVariantMap Api::batch(QVariantMap query) {
    QVariantMap data{
        { "e_r", true }
    };
    for (auto i = query.begin(); i != query.end(); i++) {
        if (i.key().indexOf("/api/") == 0) {
            data[i.key()] = i.value();
        }
    }
    return request(
        POST,
        "/api/batch",
        data,
        Request::options(query)
    );
}

// 首页轮播图
QVariantMap Api::banner(QVariantMap query) {
    const auto type0 = query.value("type", 0).toInt();
    const QMap<int, QString> typeMap{
        { 0, "pc" },
        { 1, "android" },
        { 2, "iphone" },
        { 3, "ipad" }
    };
    const QString type = typeMap.value(type0, "pc");
    return request(
        POST,
        "/api/v2/banner/get",
        {
            { "clientType", type }
        },
        Request::options(query)
        );
}

// 音乐日历
QVariantMap Api::calendar(QVariantMap query) {
    const QVariantMap data{
        { "startTime", query.value("startTime", QDateTime::currentDateTime().toMSecsSinceEpoch()) },
        { "endTime", query.value("endTime", QDateTime::currentDateTime().toMSecsSinceEpoch()) }
    };
    return request(
        POST,
        "https://music.163.com/api/mcalendar/detail",
        data,
        {
            _WEAPI
        }
    );
}

// 发送验证码
QVariantMap Api::captcha_sent(QVariantMap query) {
    const QVariantMap data{
        { "ctcode", query.value("ctcode", "86") },
        { "cellphone", query["cellphone"] }
    };
    return request(
        POST,
        "https://music.163.com/api/sms/captcha/sent",
        data,
        {
            _WEAPI
        }
    );
}

// 校验验证码
QVariantMap Api::captcha_verify(QVariantMap query) {
    const QVariantMap data{
        { "ctcode", query.value("ctcode", "86") },
        { "cellphone", query["phone"] },
        { "captcha", query["captcha"] }
    };
    return request(
        POST,
        "https://music.163.com/weapi/sms/captcha/verify",
        data,
        {
            _WEAPI
        }
    );
}

// 检测手机号码是否已注册
QVariantMap Api::cellphone_existence_check(QVariantMap query) {
    const QVariantMap data{
        { "cellphone", query["phone"] },
        { "countrycode", query["countrycode"] }
    };
    return request(
        POST,
        "/api/cellphone/existence/check",
        data,
        Request::options(query)
    );
}

// 歌曲可用性
QVariantMap Api::check_music(QVariantMap query) {
    const QVariantMap data{
        { "ids", "[" + query["id"].toString() + "]" },
        { "br", query.value("br", 999000) }
    };
    QVariantMap result = request(
        POST,
        "https://music.163.com/weapi/song/enhance/player/url",
        data,
        {
            _WEAPI
        }
    );
    auto playable = false;
    if (result["body"].toMap()["code"] == 200) {
        if (result["body"].toMap()["data"].toList().value(0).toMap()["code"] == 200) {
            playable = true;
        }
    }
    if (playable) {
        result["body"] = QVariantMap{
            { "code", 200 },
            { "success", true },
            { "message", "ok" }
        };
        return result;
    }
    else {
        result["body"] = QVariantMap{
            { "code", 200 },
            { "success", false },
            { "message", "亲爱的,暂无版权" }
        };
        return result;
    }
}

// 云盘歌曲信息匹配纠正
QVariantMap Api::cloud_match(QVariantMap query) {
    const QVariantMap data{
        { "userId", query["uid"] },
        { "songId", query["sid"] },
        { "adjustSongId", query["asid"] }
    };
    return request(
        POST,
        "https://music.163.com/api/cloud/user/song/match",
        data,
        {
            _WEAPI
        }
    );
}

// 云盘上传
QVariantMap Api::cloud(QVariantMap query) {
    QString ext = "mp3";
    if (query.value("songFile").toMap()["name"].toString().toLower().indexOf("flac") > -1) {
        ext = "flac";
    }
    QString filename = query.value("songFile").toMap()["name"].toString()
        .replace("." + ext, "")
        .replace(QRegularExpression("\\s"), "")
        .replace(QRegularExpression("\\."), "_");
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

    if (query.contains("dataAsBase64")) {
        auto songFile = query["songFile"].toMap();
        songFile["data"] = QByteArray::fromBase64(songFile["data"].toByteArray());
        query["songFile"] = songFile;
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
            _WEAPI,
        }
        );
    QString artist = "";
    QString album = "";
    QString songName = "";

    if (query.contains("artist")) {
        artist = query["artist"].toString();
    }
    if (query.contains("album")) {
        album = query["album"].toString();
    }
    if (query.contains("songName")) {
        songName = query["songName"].toString();
    }

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
        const auto uploadInfo = QCloudMusicApi::Plugins::songUpload(query);
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
            _WEAPI,
        }
        );
    const auto res3 = request(
        POST,
        "https://interface.music.163.com/api/cloud/pub/v2",
        {
            { "songid", res2["body"].toMap()["songId"] }
        },
        {
            _WEAPI
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
    const QVariantMap data{
        { "s", query["keywords"] },
        { "type", query.value("type", 1) },// 1: 单曲, 10: 专辑, 100: 歌手, 1000: 歌单, 1002: 用户, 1004: MV, 1006: 歌词, 1009: 电台, 1014: 视频
        { "limit", query.value("limit", 30) },
        { "offset", query.value("offset", 0) },
        { "total", true }
    };
    return request(
        POST,
        "/api/cloudsearch/pc",
        data,
        Request::options(query)
    );
}

// 专辑评论
QVariantMap Api::comment_album(QVariantMap query) {
    // QVariantMap cookie = query["cookie"].toMap();
    // cookie["os"] = "pc";
    // cookie["appver"] = "2.9.7";
    // query["cookie"] = cookie;
    const QVariantMap data{
        { "rid", query["id"] },
        { "limit", query.value("limit", 20) },
        { "offset", query.value("offset", 0) },
        { "beforeTime", query.value("before", 0) }
    };
    return request(
        POST,
        "https://music.163.com/api/v1/resource/comments/R_AL_3_" + query["id"].toString(),
        data,
        {
            _WEAPI
        }
    );
}

// 电台评论
QVariantMap Api::comment_dj(QVariantMap query) {
    QVariantMap cookie = query["cookie"].toMap();
    cookie["os"] = "pc";
    cookie["appver"] = "2.9.7";
    query["cookie"] = cookie;
    const QVariantMap data{
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
            _WEAPI
        }
    );
}

// 获取动态评论
QVariantMap Api::comment_event(QVariantMap query) {
    const QVariantMap data{
        { "limit", query.value("limit", 20) },
        { "offset", query.value("offset", 0) },
        { "beforeTime", query.value("before", 0) }
    };
    return request(
        POST,
        "https://music.163.com/weapi/v1/resource/comments/" + query["threadId"].toString(),
        data,
        {
            _WEAPI
        }
    );
}

// 楼层评论
QVariantMap Api::comment_floor(QVariantMap query) {
    query["type"] = resourceTypeMap[query["type"].toString()];
    const QVariantMap data{
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
            _WEAPI
        }
    );
}

// 热门评论
QVariantMap Api::comment_hot(QVariantMap query) {
    QVariantMap cookie = query["cookie"].toMap();
    cookie["os"] = "pc";
    cookie["appver"] = "2.9.7";
    query["cookie"] = cookie;
    query["type"] = resourceTypeMap[query["type"].toString()];
    const QVariantMap data{
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
            _WEAPI
        }
    );
}

// 评论抱一抱列表
QVariantMap Api::comment_hug_list(QVariantMap query) {
    query["type"] = resourceTypeMap[query.value("type", "0").toString()];
    const QString threadId = query["type"].toString() + query["sid"].toString();
    const QVariantMap data{
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
        "/api/v2/resource/comments/hug/list",
        data,
        Request::options(query)
    );
}

// 点赞与取消点赞评论
QVariantMap Api::comment_like(QVariantMap query) {
    QVariantMap cookie = query["cookie"].toMap();
    cookie["os"] = "pc";
    cookie["appver"] = "2.9.7";
    query["cookie"] = cookie;
    query["t"] = query["t"] == 1 ? "like" : "unlike";
    query["type"] = resourceTypeMap[query["type"].toString()];
    QVariantMap data{
        { "threadId", query["type"].toString() + query["sid"].toString() },
        { "commentId", query["cid"] }
    };
    if (query["type"] == "A_EV_2_") {
        data["threadId"] = query["threadId"];
    }
    return request(
        POST,
        "https://music.163.com/weapi/v1/comment/" + query["t"].toString(),
        data,
        {
            _WEAPI
        }
    );
}

// 歌曲评论
QVariantMap Api::comment_music(QVariantMap query) {
    QVariantMap cookie = query["cookie"].toMap();
    cookie["os"] = "pc";
    cookie["appver"] = "2.9.7";
    query["cookie"] = cookie;
    const QVariantMap data{
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
            _WEAPI
        }
    );
}

// MV评论
QVariantMap Api::comment_mv(QVariantMap query) {
    QVariantMap cookie = query["cookie"].toMap();
    cookie["os"] = "pc";
    cookie["appver"] = "2.9.7";
    query["cookie"] = cookie;
    const QVariantMap data{
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
            _WEAPI
        }
    );
}

// 新版评论接口
QVariantMap Api::comment_new(QVariantMap query) {
    QVariantMap cookie = query["cookie"].toMap();
    cookie["os"] = "pc";
    cookie["appver"] = "2.9.7";
    query["cookie"] = cookie;
    query["type"] = resourceTypeMap[query["type"].toString()];
    const QString threadId = query["type"].toString() + query["id"].toString();
    const int pageSize = query.value("pageSize", 20).toInt();
    const int pageNo = query.value("pageNo", 1).toInt();
    int sortType = query.value("sortType", 99).toInt();
    if (sortType == 1) {
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
    const QVariantMap data{
        { "threadId", threadId },
        { "pageNo", pageNo },
        { "showInner", query.value("showInner", true) },
        { "pageSize", pageSize },
        { "cursor", cursor },
        { "sortType", sortType } //99:按推荐排序,2:按热度排序,3:按时间排序
    };
    return request(
        POST,
        "/api/v2/resource/comments",
        data,
        Request::options(query)
    );
}

// 歌单评论
QVariantMap Api::comment_playlist(QVariantMap query) {
    QVariantMap cookie = query["cookie"].toMap();
    cookie["os"] = "pc";
    cookie["appver"] = "2.9.7";
    query["cookie"] = cookie;
    const QVariantMap data{
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
            _WEAPI
        }
    );
}

// 视频评论
QVariantMap Api::comment_video(QVariantMap query) {
    QVariantMap cookie = query["cookie"].toMap();
    cookie["os"] = "pc";
    cookie["appver"] = "2.9.7";
    query["cookie"] = cookie;
    const QVariantMap data{
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
            _WEAPI
        }
    );
}

// 发送与删除评论
QVariantMap Api::comment(QVariantMap query) {
    QVariantMap cookie = query["cookie"].toMap();
    cookie["os"] = "android";
    query["cookie"] = cookie;
    query["t"] = QMap<int, QString>{
        { 1, "add" },
        { 0, "delete" },
        { 2, "reply" }
    } [query["t"].toInt()] ;
    query["type"] = resourceTypeMap[query["type"].toString()];
    QVariantMap data{
        { "threadId", query["type"].toString() + query["id"].toString() }
    };
    if (query["type"] == "A_EV_2_") {
        data["threadId"] = query["threadId"];
    }
    if (query["t"] == "add") data["content"] = query["content"];
    else if (query["t"] == "delete") data["commentId"] = query["commentId"];
    else if (query["t"] == "reply") {
        data["commentId"] = query["commentId"];
        data["content"] = query["content"];
    }
    return request(
        POST,
        "https://music.163.com/weapi/resource/comments/" + query["t"].toString(),
        data,
        {
            _WEAPI
        }
    );
}

// 国家编码列表
QVariantMap Api::countries_code_list(QVariantMap query) {
    const QVariantMap data{ };
    return request(
        POST,
        "/api/lbs/countries/v1",
        data,
        Request::options(query)
    );
}

// 获取达人用户信息
QVariantMap Api::creator_authinfo_get(QVariantMap query) {
    const QVariantMap data{ };
    return request(
        POST,
        "/api/user/creator/authinfo/get",
        data,
        Request::options(query)
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
    const QVariantMap data{
        { "type", query.value("type", 0) }
    };
    return request(
        POST,
        "https://music.163.com/weapi/point/dailyTask",
        data,
        {
            _WEAPI
        }
    );
}

// 数字专辑详情
QVariantMap Api::digitalAlbum_detail(QVariantMap query) {
    const QVariantMap data{
        { "id", query["id"] }
    };
    return request(
        POST,
        "https://music.163.com/weapi/vipmall/albumproduct/detail",
        data,
        {
            _WEAPI
        }
    );
}

// 购买数字专辑
QVariantMap Api::digitalAlbum_ordering(QVariantMap query) {
    const QVariantMap data{
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
            _WEAPI
        }
    );
}

// 我的数字专辑
QVariantMap Api::digitalAlbum_purchased(QVariantMap query) {
    const QVariantMap data{
        { "limit", query.value("limit", 30) },
        { "offset", query.value("offset", 0) },
        { "total", true }
    };
    return request(
        POST,
        "https://music.163.com/api/digitalAlbum/purchased",
        data,
        {
            _WEAPI
        }
    );
}

// 数字专辑销量
QVariantMap Api::digitalAlbum_sales(QVariantMap query) {
    const QVariantMap data{
        { "albumIds", query["ids"] }
    };
    return request(
        POST,
        "https://music.163.com/weapi/vipmall/albumproduct/album/query/sales",
        data,
        {
            _WEAPI
        }
    );
}

// 电台banner
QVariantMap Api::dj_banner(QVariantMap query) {
    QVariantMap cookie = query["cookie"].toMap();
    cookie["os"] = "pc";
    cookie["appver"] = "2.9.7";
    query["cookie"] = cookie;
    return request(
        POST,
        "https://music.163.com/weapi/djradio/banner/get",
        {},
        {
            _WEAPI
        }
        );
}

// 电台非热门类型
QVariantMap Api::dj_category_excludehot(QVariantMap query) {
    const QVariantMap data{ };
    return request(
        POST,
        "https://music.163.com/weapi/djradio/category/excludehot",
        data,
        {
            _WEAPI
        }
    );
}

// 电台推荐类型
QVariantMap Api::dj_category_recommend(QVariantMap query) {
    const QVariantMap data{ };
    return request(
        POST,
        "https://music.163.com/weapi/djradio/home/category/recommend",
        data,
        {
            _WEAPI
        }
    );
}

// 电台分类列表
QVariantMap Api::dj_catelist(QVariantMap query) {
    const QVariantMap data{ };
    return request(
        POST,
        "https://music.163.com/weapi/djradio/category/get",
        data,
        {
            _WEAPI
        }
    );
}

// 电台详情
QVariantMap Api::dj_detail(QVariantMap query) {
    const QVariantMap data{
        { "id", query["rid"] }
    };
    return request(
        POST,
        "https://music.163.com/api/djradio/v2/get",
        data,
        {
            _WEAPI
        }
    );
}

// 热门电台
QVariantMap Api::dj_hot(QVariantMap query) {
    const QVariantMap data{
        { "limit", query.value("limit", 30) },
        { "offset", query.value("offset", 0) }
    };
    return request(
        POST,
        "https://music.163.com/weapi/djradio/hot/v1",
        data,
        {
            _WEAPI
        }
    );
}

// 付费电台
QVariantMap Api::dj_paygift(QVariantMap query) {
    const QVariantMap data{
        { "limit", query.value("limit", 30) },
        { "offset", query.value("offset", 0) }
    };
    return request(
        POST,
        "https://music.163.com/weapi/djradio/home/paygift/list?_nmclfl=1",
        data,
        {
            _WEAPI
        }
    );
}

// 电台个性推荐
QVariantMap Api::dj_personalize_recommend(QVariantMap query) {
    const QVariantMap data{
        { "limit", query.value("limit", 6) }
    };
    return request(
        POST,
        "https://music.163.com/api/djradio/personalize/rcmd",
        data,
        {
            _WEAPI
        }
    );
}

// 电台节目详情
QVariantMap Api::dj_program_detail(QVariantMap query) {
    const QVariantMap data{
        { "id", query["id"] }
    };
    return request(
        POST,
        "https://music.163.com/api/dj/program/detail",
        data,
        {
            _WEAPI
        }
    );
}

// 电台24小时节目榜
QVariantMap Api::dj_program_toplist_hours(QVariantMap query) {
    const QVariantMap data{
        { "limit", query.value("limit", 100) }
        // 不支持 offset
    };
    return request(
        POST,
        "https://music.163.com/api/djprogram/toplist/hours",
        data,
        {
            _WEAPI
        }
    );
}

// 电台节目榜
QVariantMap Api::dj_program_toplist(QVariantMap query) {
    const QVariantMap data{
        { "limit", query.value("limit", 100) },
        { "offset", query.value("offset", 0) }
    };
    return request(
        POST,
        "https://music.163.com/api/program/toplist/v1",
        data,
        {
            _WEAPI
        }
    );
}

// 电台节目列表
QVariantMap Api::dj_program(QVariantMap query) {
    const QVariantMap data{
        { "radioId", query["rid"] },
        { "limit", query.value("limit", 30) },
        { "offset", query.value("offset", 0) },
        { "asc", QCloudMusicApi::Index::toBoolean(query["asc"]) }
    };
    return request(
        POST,
        "https://music.163.com/weapi/dj/program/byradio",
        data,
        {
            _WEAPI
        }
    );
}

// 类别热门电台
QVariantMap Api::dj_radio_hot(QVariantMap query) {
    const QVariantMap data{
        { "cateId", query["cateId"] },
        { "limit", query.value("limit", 30) },
        { "offset", query.value("offset", 0) }
    };
    return request(
        POST,
        "https://music.163.com/api/djradio/hot",
        data,
        {
            _WEAPI
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
    const QVariantMap data{
        { "cateId", query["type"] }
    };
    return request(
        POST,
        "https://music.163.com/weapi/djradio/recommend",
        data,
        {
            _WEAPI
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
            _WEAPI
        }
        );
}

// 订阅与取消电台
QVariantMap Api::dj_sub(QVariantMap query) {
    query["t"] = query["t"] == 1 ? "sub" : "unsub";
    const QVariantMap data{
        { "id", query["rid"] }
    };
    return request(
        POST,
        "https://music.163.com/weapi/djradio/" + query["t"].toString(),
        data,
        {
            _WEAPI
        }
    );
}

// 订阅电台列表
QVariantMap Api::dj_sublist(QVariantMap query) {
    const QVariantMap data{
        { "limit", query.value("limit", 30) },
        { "offset", query.value("offset", 0) },
        { "total", true }
    };
    return request(
        POST,
        "https://music.163.com/weapi/djradio/get/subed",
        data,
        {
            _WEAPI
        }
    );
}

// 电台详情
QVariantMap Api::dj_subscriber(QVariantMap query) {
    const QVariantMap data{
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
            _WEAPI
        }
    );
}

// 电台今日优选
QVariantMap Api::dj_today_perfered(QVariantMap query) {
    const QVariantMap data{
        { "page", query.value("page", 0) }
    };
    return request(
        POST,
        "https://music.163.com/weapi/djradio/home/today/perfered",
        data,
        {
            _WEAPI
        }
    );
}

// 电台24小时主播榜
QVariantMap Api::dj_toplist_hours(QVariantMap query) {
    const QVariantMap data{
        { "limit", query.value("limit", 100) }
        // 不支持 offset
    };
    return request(
        POST,
        "https://music.163.com/api/dj/toplist/hours",
        data,
        {
            _WEAPI
        }
    );
}

// 电台新人榜
QVariantMap Api::dj_toplist_newcomer(QVariantMap query) {
    const QVariantMap data{
        { "limit", query.value("limit", 100) },
        { "offset", query.value("offset", 0) }
    };
    return request(
        POST,
        "https://music.163.com/api/dj/toplist/newcomer",
        data,
        {
            _WEAPI
        }
    );
}

// 付费精品
QVariantMap Api::dj_toplist_pay(QVariantMap query) {
    const QVariantMap data{
        { "limit", query.value("limit", 100) }
        // 不支持 offset
    };
    return request(
        POST,
        "https://music.163.com/api/djradio/toplist/pay",
        data,
        {
            _WEAPI
        }
    );
}

// 电台最热主播榜
QVariantMap Api::dj_toplist_popular(QVariantMap query) {
    const QVariantMap data{
        { "limit", query.value("limit", 100) }
        // 不支持 offset
    };
    return request(
        POST,
        "https://music.163.com/api/dj/toplist/popular",
        data,
        {
            _WEAPI
        }
    );
}

// 新晋电台榜/热门电台榜
QVariantMap Api::dj_toplist(QVariantMap query) {
    const QVariantMap typeMap{
        { "new", 0 },
        { "hot", 1 }
    };
    const QVariantMap data{
        { "limit", query.value("limit", 100) },
        { "offset", query.value("offset", 0) },
        { "type", typeMap.value(query.value("type", "new").toString(), "0") } //0为新晋,1为热门
    };
    return request(
        POST,
        "https://music.163.com/api/djradio/toplist",
        data,
        {
            _WEAPI
        }
    );
}

// 电台排行榜获取
QVariantMap Api::djRadio_top(QVariantMap query) {
    const QVariantMap data{
        { "djRadioId", query["djRadioId"] }, // 电台id
        { "sortIndex", query.value("sortIndex",1) }, // 排序 1:播放数 2:点赞数 3：评论数 4：分享数 5：收藏数
        { "dataGapDays", query.value("dataGapDays",7) }, // 天数 7:一周 30:一个月 90:三个月
        { "dataType", query.value("dataType",3) }, // 未知
    };
    return request(
        POST,
        "https://interface.music.163.com/weapi/expert/worksdata/works/top/get",
        data,
        {
            _WEAPI
        }
    );
}

// eapi 解析接口
QVariantMap Api::eapi_decrypt(QVariantMap query) {
    const auto hexString = query["hexString"].toString();
    const auto isReq = query["isReq"] != "false";
    if (hexString.isEmpty()) {
        return {
            { "status", 400 },
            { "body", QVariantMap {
                         { "code", 400 },
                         { "message", "hex string is required" },
                      }
            }
        };
    }
    // 去除空格
    auto pureHexString = hexString;
    pureHexString = pureHexString.replace(QRegularExpression(R"(\s)"), "");
    return {
        { "status", 200 },
        { "body", QVariantMap {
                     { "code", 200 },
                     { "data", isReq
                                  ? QCloudMusicApi::Crypto::eapiReqDecrypt(pureHexString.toUtf8())
                                    : QCloudMusicApi::Crypto::eapiResDecrypt(pureHexString.toUtf8())
                     },
            }
        }
    };
}

// 删除动态
QVariantMap Api::event_del(QVariantMap query) {
    const QVariantMap data{
        { "id", query["evId"] }
    };
    return request(
        POST,
        "https://music.163.com/eapi/event/delete",
        data,
        {
            _WEAPI
        }
    );
}

// 转发动态
QVariantMap Api::event_forward(QVariantMap query) {
    QVariantMap cookie = query["cookie"].toMap();
    cookie["os"] = "pc";
    cookie["appver"] = "2.9.7";
    query["cookie"] = cookie;
    const QVariantMap data{
        { "forwards", query["forwards"] },
        { "id", query["evId"] },
        { "eventUserId", query["uid"] }
    };
    return request(
        POST,
        "https://music.163.com/weapi/event/forward",
        data,
        {
            _WEAPI
        }
    );
}

// 动态
QVariantMap Api::event(QVariantMap query) {
    const QVariantMap data{
        { "pagesize", query.value("pagesize", 20) },
        { "lasttime", query.value("lasttime", -1) }
    };
    return request(
        POST,
        "https://music.163.com/weapi/v1/event/get",
        data,
        {
            _WEAPI
        }
    );
}

// 粉丝年龄比例
QVariantMap Api::fanscenter_basicinfo_age_get(QVariantMap query) {
    const QVariantMap data{ };
    return request(
        POST,
        "/api/fanscenter/basicinfo/age/get",
        data,
        Request::options(query)
    );
}

// 粉丝性别比例
QVariantMap Api::fanscenter_basicinfo_gender_get(QVariantMap query) {
    const QVariantMap data{ };
    return request(
        POST,
        "/api/fanscenter/basicinfo/gender/get",
        data,
        Request::options(query)
    );
}

// 粉丝省份比例
QVariantMap Api::fanscenter_basicinfo_province_get(QVariantMap query) {
    const QVariantMap data{ };
    return request(
        POST,
        "/api/fanscenter/basicinfo/province/get",
        data,
        Request::options(query)
    );
}

// 粉丝数量
QVariantMap Api::fanscenter_overview_get(QVariantMap query) {
    const QVariantMap data{ };
    return request(
        POST,
        "/api/fanscenter/overview/get",
        data,
        Request::options(query)
    );
}

// 粉丝来源
QVariantMap Api::fanscenter_trend_list(QVariantMap query) {
    const QVariantMap data{
        { "startTime", query.value("startTime", QDateTime::currentDateTime().toMSecsSinceEpoch() - 7 * 24 * 3600 * 1000) },
        { "endTime", query.value("endTime", QDateTime::currentDateTime().toMSecsSinceEpoch()) },
        { "type", query.value("type", 0) }//新增关注:0 新增取关:1
    };
    return request(
        POST,
        "/api/fanscenter/trend/list",
        data,
        Request::options(query)
    );
}

// 垃圾桶
QVariantMap Api::fm_trash(QVariantMap query) {
    const QVariantMap data{
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
            _WEAPI
        }
    );
}

// 关注与取消关注用户
QVariantMap Api::follow(QVariantMap query) {
    QVariantMap cookie = query["cookie"].toMap();
    cookie["os"] = "pc";
    cookie["appver"] = "2.9.7";
    query["cookie"] = cookie;
    query["t"] = query["t"] == 1 ? "follow" : "delfollow";
    return request(
        POST,
        "https://music.163.com/weapi/user/"
        + query["t"].toString()
        + QStringLiteral("/")
        + query["id"].toString(),
        {},
        {
            _WEAPI
        }
        );
}

// 根据nickname获取userid
QVariantMap Api::get_userids(QVariantMap query) {
    const QVariantMap data{
        { "nicknames", query["nicknames"] }
    };
    return request(
        POST,
        "https://music.163.com/api/user/getUserIds",
        data,
        {
            _WEAPI
        }
    );
}

// 历史每日推荐歌曲详情
QVariantMap Api::history_recommend_songs_detail(QVariantMap query) {
    QVariantMap cookie = query["cookie"].toMap();
    cookie["os"] = "ios";
    query["cookie"] = cookie;
    const QVariantMap data{
        { "date", query.value("date", "") }
    };
    return request(
        POST,
        "https://music.163.com/api/discovery/recommend/songs/history/detail",
        data,
        {
            _WEAPI
        }
    );
}

// 历史每日推荐歌曲
QVariantMap Api::history_recommend_songs(QVariantMap query) {
    QVariantMap cookie = query["cookie"].toMap();
    cookie["os"] = "ios";
    query["cookie"] = cookie;
    const QVariantMap data{ };
    return request(
        POST,
        "https://music.163.com/api/discovery/recommend/songs/history/recent",
        data,
        {
            _WEAPI
        }
    );
}

// 首页-发现 block page
// 这个接口为移动端接口，首页-发现页，数据结构可以参考 https://github.com/hcanyz/flutter-netease-music-api/blob/master/lib/src/api/uncategorized/bean.dart#L259 HomeBlockPageWrap
// query.refresh 是否刷新数据
QVariantMap Api::homepage_block_page(QVariantMap query) {
    const QVariantMap data{
        { "refresh", query.value("refresh", false) },
        { "cursor", query["cursor"] }
    };
    return request(
        POST,
        "https://music.163.com/api/homepage/block/page",
        data,
        {
            _WEAPI
        }
    );
}

// 首页-发现 dragon ball
// 这个接口为移动端接口，首页-发现页（每日推荐、歌单、排行榜 那些入口）
// 数据结构可以参考 https://github.com/hcanyz/flutter-netease-music-api/blob/master/lib/src/api/uncategorized/bean.dart#L290 HomeDragonBallWrap
// !需要登录或者游客登录，非登录返回 []
QVariantMap Api::homepage_dragon_ball(QVariantMap query) {
    const QVariantMap data{ };
    return request(
        POST,
        "/api/homepage/dragon/ball/static",
        data,
        Request::options(query)
    );
}

//热门话题
QVariantMap Api::hot_topic(QVariantMap query) {
    const QVariantMap data{
        { "limit", query.value("limit", 20) },
        { "offset", query.value("offset", 0) }
    };
    return request(
        POST,
        "https://music.163.com/api/act/hot",
        data,
        {
            _WEAPI
        }
    );
}

// 抱一抱评论
QVariantMap Api::hug_comment(QVariantMap query) {
    query["type"] = resourceTypeMap[query.value("type", 0).toString()];
    const QString threadId = query["type"].toString() + query["sid"].toString();
    const QVariantMap data{
        { "targetUserId", query["uid"] },
        { "commentId", query["cid"] },
        { "threadId", threadId }
    };
    return request(
        POST,
        "/api/v2/resource/comments/hug/listener",
        data,
        Request::options(query)
    );
}

// 红心与取消红心歌曲
QVariantMap Api::like(QVariantMap query) {
    query["like"] = query["like"] == "false" ? false : true;
    QVariantMap cookie = query["cookie"].toMap();
    cookie["os"] = "pc";
    cookie["appver"] = "2.9.7";
    query["cookie"] = cookie;
    const QVariantMap data{
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
            _WEAPI
        }
    );
}

// 喜欢音乐列表
QVariantMap Api::likelist(QVariantMap query) {
    const QVariantMap data{
        { "uid", query["uid"] }
    };
    return request(
        POST,
        "https://music.163.com/weapi/song/like/get",
        data,
        {
            _WEAPI
        }
    );
}

// 一起听 接受邀请
QVariantMap Api::listentogether_accept(QVariantMap query) {
    const QVariantMap data{
        { "refer", "inbox_invite" },
        { "roomId", query["roomId"] },
        { "inviterId", query["inviterId"] },
    };
    return request(
        POST,
        "/api/listen/together/play/invitation/accept",
        data,
        Request::options(query)
    );
}

// 一起听 结束房间
QVariantMap Api::listentogether_end(QVariantMap query) {
    const QVariantMap data{
        { "roomId", query["roomId"] }
    };
    return request(
        POST,
        "/api/listen/together/end/v2",
        data,
        Request::options(query)
    );
}

// 一起听 发送心跳
QVariantMap Api::listentogether_heatbeat(QVariantMap query) {
    const QVariantMap data{
        { "roomId", query["roomId"] },
        { "songId", query["songId"] },
        { "playStatus", query["playStatus"] },
        { "progress", query["progress"] }
    };
    return request(
        POST,
        "/api/listen/together/heartbeat",
        data,
        Request::options(query)
    );
}

// 一起听 发送播放状态
QVariantMap Api::listentogether_play_command(QVariantMap query) {
    const QVariantMap data{
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
        "/api/listen/together/play/command/report",
        data,
        Request::options(query)
    );
}

// 一起听 房间情况
QVariantMap Api::listentogether_room_check(QVariantMap query) {
    const QVariantMap data{
        { "roomId", query["roomId"] }
    };
    return request(
        POST,
        "/api/listen/together/room/check",
        data,
        Request::options(query)
    );
}

// 一起听创建房间
QVariantMap Api::listentogether_room_create(QVariantMap query) {
    const QVariantMap data{
        { "refer", "songplay_more" }
    };
    return request(
        POST,
        "/api/listen/together/room/create",
        data,
        Request::options(query)
    );
}

// 一起听状态
QVariantMap Api::listentogether_status(QVariantMap query) {
    return request(
        POST,
        "https://music.163.com/api/listen/together/status/get",
        {},
        {
            _WEAPI
        }
        );
}

// 一起听 更新播放列表
QVariantMap Api::listentogether_sync_list_command(QVariantMap query) {
    const QVariantMap data{
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
        "/api/listen/together/sync/list/command/report",
        data,
        Request::options(query)
    );
}

// 一起听 当前列表获取
QVariantMap Api::listentogether_sync_playlist_get(QVariantMap query) {
    const QVariantMap data{
        { "roomId", query["roomId"] }
    };
    return request(
        POST,
        "/api/listen/together/sync/playlist/get",
        data,
        Request::options(query)
    );
}

// 手机登录
QVariantMap Api::login_cellphone(QVariantMap query) {
    const QVariantMap data{
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
            _WEAPI,
            { "uaType", "pc" },
        }
    );
    if (result["body"].toMap()["code"] == 200) {
        auto body = result["body"].toMap();
        body["cookie"] = result["cookie"];
        result = QVariantMap{
            { "status", 200 },
            { "body", body },
            { "cookie", result["cookie"] }
        };
    }
    return result;
}

// 二维码检测扫码状态接口
QVariantMap Api::login_qr_check(QVariantMap query) {
    const QVariantMap data{
        { "key", query["key"] },
        { "type", 1 }
    };
    QVariantMap result = request(
        POST,
        "https://music.163.com/weapi/login/qrcode/client/login",
        data,
        {
            _WEAPI
        }
    );
    auto body = result["body"].toMap();
    body["cookie"] = result["cookie"];
    result = QVariantMap{
        { "status", 200 },
        { "body", body },
        { "cookie", result["cookie"] }
    };
    return result;
}

// 二维码 key 生成接口
QVariantMap Api::login_qr_key(QVariantMap query) {
    const QVariantMap data{
        { "type", 1 }
    };
    QVariantMap result = request(
        POST,
        "https://music.163.com/weapi/login/qrcode/unikey",
        data,
        {
            _WEAPI
        }
    );
    result = QVariantMap{
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
    auto result = QVariantMap{
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
            _WEAPI,
            { "uaType", "pc" },
        }
        );
    if (result["body"].toMap()["code"] == 200) {
        auto body = result["body"].toMap();
        body["cookie"] = result["cookie"];
        result = QVariantMap{
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
        "/api/w/nuser/account/get",
        {},
        Request::options(query)
        );
    if (result["body"].toMap()["code"] == 200) {
        auto body = result["body"].toMap();
        body["cookie"] = result["cookie"];
        result = QVariantMap{
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
            _WEAPI,
            { "uaType", "pc" },
        }
        );
}

// 新版歌词 - 包含逐字歌词
QVariantMap Api::lyric_new(QVariantMap query) {
    const QVariantMap data{
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
        "/api/song/lyric/v1",
        data,
        Request::options(query)
    );
}

// 歌词
QVariantMap Api::lyric(QVariantMap query) {
    QVariantMap cookie = query["cookie"].toMap();
    cookie["os"] = "ios";
    query["cookie"] = cookie;

    const QVariantMap data{
        { "id", query["id"] },
        { "tv", -1 },
        { "lv", -1 },
        { "rv", -1 },
        { "kv", -1 },
        { "_nmclfl", 1 }
    };
    return request(
        POST,
        "/api/song/lyric",
        data,
        Request::options(query)
    );
}

// 歌曲相关视频
QVariantMap Api::mlog_music_rcmd(QVariantMap query) {
    const QVariantMap data{
        { "id", query.value("mvid", 0) },
        { "type", 2 },
        { "rcmdType", 20 },
        { "limit", query.value("limit", 10) },
        { "extInfo", QJsonDocument::fromVariant(QVariantMap { { "songId", query["songid"] } }).toJson() }
    };
    return request(
        POST,
        "/api/mlog/rcmd/feed/list",
        data,
        Request::options(query)
    );
}

// 将mlog id转为video id
QVariantMap Api::mlog_to_video(QVariantMap query) {
    const QVariantMap data{
        { "mlogId", query["id"] },
    };
    return request(
        POST,
        "https://music.163.com/weapi/mlog/video/convert/id",
        data,
        {
            _WEAPI
        }
    );
}

// mlog链接
QVariantMap Api::mlog_url(QVariantMap query) {
    const QVariantMap data{
        { "id", query["id"] },
        { "resolution", query.value("res", 1080) },
        { "type", 1 },
    };
    return request(
        POST,
        "https://music.163.com/weapi/mlog/detail/v1",
        data,
        {
            _WEAPI
        }
    );
}

// 评论
QVariantMap Api::msg_comments(QVariantMap query) {
    const QVariantMap data{
        { "beforeTime", query.value("beforeTime", "-1") },
        { "limit", query.value("limit", 30) },
        { "total", "true" },
        { "uid", query["uid"] },
    };
    return request(
        POST,
        "https://music.163.com/api/v1/user/comments/" + query["uid"].toString(),
        data,
        {
            _WEAPI
        }
    );
}

// @我
QVariantMap Api::msg_forwards(QVariantMap query) {
    const QVariantMap data{
        { "offset", query.value("offset", "0") },
        { "limit", query.value("limit", 30) },
        { "total", "true" },
    };
    return request(
        POST,
        "https://music.163.com/api/forwards/get",
        data,
        {
            _WEAPI
        }
    );
}

// 通知
QVariantMap Api::msg_notices(QVariantMap query) {
    const QVariantMap data{
        { "limit", query.value("limit", 30) },
        { "time", query.value("lasttime", -1) },
    };
    return request(
        POST,
        "https://music.163.com/api/msg/notices",
        data,
        {
            _WEAPI
        }
    );
}

// 私信内容
QVariantMap Api::msg_private_history(QVariantMap query) {
    const QVariantMap data{
        { "userId", query["uid"] },
        { "limit", query.value("limit", 30) },
        { "time", query.value("before", "0") },
        { "total", "true" },
    };
    return request(
        POST,
        "https://music.163.com/api/msg/private/history",
        data,
        {
            _WEAPI
        }
    );
}

// 私信
QVariantMap Api::msg_private(QVariantMap query) {
    const QVariantMap data{
        { "offset", query.value("offset", "0") },
        { "limit", query.value("limit", 30) },
        { "total", "true" },
    };
    return request(
        POST,
        "https://music.163.com/api/msg/private/users",
        data,
        {
            _WEAPI
        }
    );
}

// 最近联系
QVariantMap Api::msg_recentcontact(QVariantMap query) {
    const QVariantMap data{};
    return request(
        POST,
        "https://music.163.com/api/msg/recentcontact/get",
        data,
        {
            _WEAPI
        }
    );
}

// 回忆坐标
QVariantMap Api::music_first_listen_info(QVariantMap query) {
    const QVariantMap data{
        { "songId", query["id"] },
    };
    return request(
        POST,
        "https://interface3.music.163.com/api/content/activity/music/first/listen/info",
        data,
        {
            _WEAPI
        }
    );
}

// 全部MV
QVariantMap Api::mv_all(QVariantMap query) {
    const QVariantMap data{
        { "tags", QJsonDocument::fromVariant(QVariantMap
                                            {
                                                { "地区", query.value("area", "全部") },
                                                { "类型", query.value("type", "全部") },
                                                { "排序", query.value("order", "上升最快") },
                                            }).toJson() },
        { "offset", query.value("offset", "0") },
        { "total", "true" },
        { "limit", query.value("limit", 30) },
    };
    return request(
        POST,
        "https://interface.music.163.com/api/mv/all",
        data,
        {
            _WEAPI
        }
    );
}

// MV 点赞转发评论数数据
QVariantMap Api::mv_detail_info(QVariantMap query) {
    const QVariantMap data{
        { "threadid", "R_MV_5_" + query["mvid"].toString() },
        { "composeliked", "true" },
    };
    return request(
        POST,
        "https://music.163.com/api/comment/commentthread/info",
        data,
        {
            _WEAPI
        }
    );
}

// MV详情
QVariantMap Api::mv_detail(QVariantMap query) {
    const QVariantMap data{
        { "id", query["mvid"] },
        { "composeliked", "true" },
    };
    return request(
        POST,
        "https://music.163.com/api/v1/mv/detail",
        data,
        {
            _WEAPI
        }
    );
}

// 网易出品
QVariantMap Api::mv_exclusive_rcmd(QVariantMap query) {
    const QVariantMap data{
        { "offset", query.value("offset", "0") },
        { "limit", query.value("limit", 30) },
    };
    return request(
        POST,
        "https://interface.music.163.com/api/mv/exclusive/rcmd",
        data,
        {
            _WEAPI
        }
    );
}

// 最新MV
QVariantMap Api::mv_first(QVariantMap query) {
    const QVariantMap data{
        // { "offset", query.value("offset", 0) },
        { "area", query.value("area", "") },
        { "limit", query.value("limit", 30) },
        { "total", "true" },
    };
    return request(
        POST,
        "https://interface.music.163.com/weapi/mv/first",
        data,
        {
            _WEAPI
        }
    );
}

// 收藏与取消收藏MV
QVariantMap Api::mv_sub(QVariantMap query) {
    query["t"] = query["t"] == 1 ? "sub" : "unsub";
    const QVariantMap data{
        { "mvId", query["mvid"] },
        { "mvIds", "[" + query["mvid"].toString() + "]" },
    };
    return request(
        POST,
        "https://music.163.com/weapi/mv/" + query["t"].toString(),
        data,
        {
            _WEAPI
        }
    );
}

// 全部MV
QVariantMap Api::mv_sublist(QVariantMap query) {
    const QVariantMap data{
        { "limit", query.value("limit", 25) },
        { "offset", query.value("offset", "0") },
        { "total", "true" },
    };
    return request(
        POST,
        "https://music.163.com/weapi/cloudvideo/allvideo/sublist",
        data,
        {
            _WEAPI
        }
    );
}

// MV链接
QVariantMap Api::mv_url(QVariantMap query) {
    const QVariantMap data{
        { "id", query["id"] },
        { "r", query.value("r", 1080) },
    };
    return request(
        POST,
        "https://music.163.com/weapi/song/enhance/play/mv/url",
        data,
        {
            _WEAPI
        }
    );
}

// 重复昵称检测
QVariantMap Api::nickname_check(QVariantMap query) {
    QVariantMap data{
        { "nickname", query["nickname"] }
    };
    return request(
        POST,
        "https://music.163.com/api/nickname/duplicated",
        data,
        {
            _WEAPI
        }
    );
}

// 私人FM - 模式选择

// aidj, DEFAULT, FAMILIAR, EXPLORE, SCENE_RCMD ( EXERCISE, FOCUS, NIGHT_EMO  )
// 来不及解释这几个了
QVariantMap Api::personal_fm_mode(QVariantMap query) {
    QVariantMap data{
        { "mode", query["mode"] },
        { "subMode", query["subMode"] },
        { "limit", query.value("limit", 3) },
    };
    return request(
        POST,
        "https://interface3.music.163.com/eapi/v1/radio/get",
        data,
        {
            _WEAPI,
            { "url", "/api/v1/radio/get" },
        }
    );
}

// 私人FM
QVariantMap Api::personal_fm(QVariantMap query) {
    return request(
        POST,
        "https://music.163.com/weapi/v1/radio/get",
        {},
        {
            _WEAPI
        }
        );
}

// 推荐电台
QVariantMap Api::personalized_djprogram(QVariantMap query) {
    return request(
        POST,
        "https://music.163.com/weapi/personalized/djprogram",
        {},
        {
            _WEAPI
        }
        );
}

// 推荐MV
QVariantMap Api::personalized_mv(QVariantMap query) {
    return request(
        POST,
        "https://music.163.com/weapi/personalized/mv",
        {},
        {
            _WEAPI
        }
        );
}

// 推荐新歌
QVariantMap Api::personalized_newsong(QVariantMap query) {
    QVariantMap cookie = query["cookie"].toMap();
    cookie["os"] = "pc";
    cookie["appver"] = "2.9.7";
    query["cookie"] = cookie;
    const QVariantMap data{
        { "type", "recommend" },
        { "limit", query.value("limit", 10) },
        { "areaId", query.value("areaId", 0) }
    };
    return request(
        POST,
        "https://music.163.com/api/personalized/newsong",
        data,
        {
            _WEAPI
        }
    );
}

// 独家放送列表
QVariantMap Api::personalized_privatecontent_list(QVariantMap query) {
    const QVariantMap data{
        { "offset", query.value("offset", 10) },
        { "total", "true" },
        { "limit", query.value("limit", 60) }
    };
    return request(
        POST,
        "https://music.163.com/api/v2/privatecontent/list",
        data,
        {
            _WEAPI
        }
    );
}

// 独家放送
QVariantMap Api::personalized_privatecontent(QVariantMap query) {
    return request(
        POST,
        "https://music.163.com/weapi/personalized/privatecontent",
        {},
        {
            _WEAPI
        }
        );
}

// 推荐歌单
QVariantMap Api::personalized(QVariantMap query) {
    const QVariantMap data{
        { "limit", query.value("limit", 30) },
        // { "offset", query.value("offset", 0) },
        { "total", "true" },
        { "n", 1000 },
    };
    return request(
        POST,
        "https://music.163.com/weapi/personalized/playlist",
        data,
        {
            _WEAPI
        }
    );
}

// 私信和通知接口
QVariantMap Api::pl_count(QVariantMap query) {
    return request(
        POST,
        "/api/pl/count",
        {},
        Request::options(query)
        );
}

// 全部歌单分类
QVariantMap Api::playlist_catlist(QVariantMap query) {
    return request(
        POST,
        "https://music.163.com/weapi/playlist/catalogue",
        {},
        {
            _WEAPI
        }
        );
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
    const auto uploadInfo = QCloudMusicApi::Plugins::upload(query);
    const auto res = request(
        POST,
        "https://music.163.com/weapi/playlist/cover/update",
        {
            { "id", query["id"] },
            { "coverImgId", uploadInfo["imgId"] }
        },
        {
            _WEAPI
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

// 创建歌单
QVariantMap Api::playlist_create(QVariantMap query) {
    QVariantMap cookie = query["cookie"].toMap();
    cookie["os"] = "pc";
    cookie["appver"] = "2.9.7";
    query["cookie"] = cookie;
    const QVariantMap data{
        { "name", query["name"] },
        { "privacy", query["privacy"] }, //0 为普通歌单，10 为隐私歌单
        { "type", query.value("type", "NORMAL") }
    };
    return request(
        POST,
        "https://music.163.com/api/playlist/create",
        data,
        {
            _WEAPI
        }
    );
}

// 删除歌单
QVariantMap Api::playlist_delete(QVariantMap query) {
    QVariantMap cookie = query["cookie"].toMap();
    cookie["os"] = "pc";
    cookie["appver"] = "2.9.7";
    query["cookie"] = cookie;
    const QVariantMap data{
        { "ids", "[" + query["id"].toString() + "]" }
    };
    return request(
        POST,
        "https://music.163.com/weapi/playlist/remove",
        data,
        {
            _WEAPI
        }
    );
}

// 更新歌单描述
QVariantMap Api::playlist_desc_update(QVariantMap query) {
    const QVariantMap data{
        { "id", query["id"] },
        { "desc", query["desc"] }
    };
    return request(
        POST,
        "/api/playlist/desc/update",
        data,
        Request::options(query)
    );
}

// 歌单动态信息
QVariantMap Api::playlist_detail_dynamic(QVariantMap query) {
    const QVariantMap data{
        { "id", query["id"] },
        { "n", 100000 },
        { "s", query.value("s", 8) }
    };
    return request(
        POST,
        "/api/playlist/detail/dynamic",
        data,
        Request::options(query)
    );
}

// 歌单详情
QVariantMap Api::playlist_detail(QVariantMap query) {
    const QVariantMap data{
        { "id", query["id"] },
        { "n", 100000 },
        { "s", query.value("s", 8) }
    };
    return request(
        POST,
        "/api/v6/playlist/detail",
        data,
        Request::options(query)
    );
}

// 精品歌单 tags
QVariantMap Api::playlist_highquality_tags(QVariantMap query) {
    const QVariantMap data{};
    return request(
        POST,
        "https://music.163.com/api/playlist/highquality/tags",
        data,
        {
            _WEAPI
        }
    );
}

// 热门歌单分类
QVariantMap Api::playlist_hot(QVariantMap query) {
    return request(
        POST,
        "https://music.163.com/weapi/playlist/hottags",
        {},
        {
            _WEAPI
        }
        );
}

// 获取点赞过的视频
QVariantMap Api::playlist_mylike(QVariantMap query) {
    const QVariantMap data{
        { "time", query.value("time", -1) },
        { "limit", query.value("limit", 12) }
    };
    return request(
        POST,
        "https://music.163.com/api/mlog/playlist/mylike/bytime/get",
        data,
        {
            _WEAPI
        }
    );
}

// 更新歌单名
QVariantMap Api::playlist_name_update(QVariantMap query) {
    const QVariantMap data{
        { "id", query["id"] },
        { "name", query["name"] }
    };
    return request(
        POST,
        "/api/playlist/update/name",
        data,
        Request::options(query)
    );
}

// 编辑歌单顺序
QVariantMap Api::playlist_order_update(QVariantMap query) {
    QVariantMap cookie = query["cookie"].toMap();
    cookie["os"] = "pc";
    cookie["appver"] = "2.9.7";
    query["cookie"] = cookie;
    const QVariantMap data{
        { "ids", query["ids"] }
    };
    return request(
        POST,
        "https://music.163.com/api/playlist/order/update",
        data,
        {
            _WEAPI
        }
    );
}

// 公开隐私歌单
QVariantMap Api::playlist_privacy(QVariantMap query) {
    const QVariantMap data{
        { "id", query["id"] },
        { "privacy", 0 }
    };
    return request(
        POST,
        "/api/playlist/update/privacy",
        data,
        Request::options(query)
    );
}

// 收藏与取消收藏歌单
QVariantMap Api::playlist_subscribe(QVariantMap query) {
    query["t"] = query["t"] == 1 ? "subscribe" : "unsubscribe";
    const QVariantMap data{
        { "id", query["id"] }
    };
    return request(
        POST,
        "https://music.163.com/api/playlist/" + query["t"].toString(),
        data,
        {
            _WEAPI
        }
    );
}

// 歌单收藏者
QVariantMap Api::playlist_subscribers(QVariantMap query) {
    const QVariantMap data{
        { "id", query["id"] },
        { "limit", query.value("limit", 20) },
        { "offset", query.value("offset", 0) }
    };
    return request(
        POST,
        "https://music.163.com/api/playlist/subscribers",
        data,
        {
            _WEAPI
        }
    );
}

// 更新歌单标签
QVariantMap Api::playlist_tags_update(QVariantMap query) {
    const QVariantMap data{
        { "id", query["id"] },
        { "tags", query["tags"] }
    };
    return request(
        POST,
        "/api/playlist/tags/update",
        data,
        Request::options(query)
    );
}

// 收藏视频到视频歌单
QVariantMap Api::playlist_track_add(QVariantMap query) {
    QVariantMap cookie = query["cookie"].toMap();
    cookie["os"] = "pc";
    cookie["appver"] = "2.9.7";
    query["cookie"] = cookie;
    query["ids"] = query.value("ids", "");
    const QVariantMap data{
        { "id", query["pid"] },
        { "tracks", QJsonDocument::fromVariant([&]() {
                       QVariantList items;
                       for (auto& item : query["ids"].toString().split(",")) {
                           items.push_back(QVariantMap
                                           {
                                               { "type", 3 },
                                               { "id", item }
                                           });
                       }
                       return items;
                   }()).toJson() }
    };
    return request(
        POST,
        "https://music.163.com/api/playlist/track/add",
        data,
        {
            _WEAPI
        }
    );
}

// 通过传过来的歌单id拿到所有歌曲数据
// 支持传递参数limit来限制获取歌曲的数据数量 例如: /playlist/track/all?id=7044354223&limit=10
QVariantMap Api::playlist_track_all(QVariantMap query) {
    const QVariantMap data{
        { "id", query["id"] },
        { "n", 100000 },
        { "s", query.value("s", 8) },
    };
    //不放在data里面避免请求带上无用的数据
    int limit = query.value("limit", std::numeric_limits<int>::max()).toInt();
    int offset = query.value("offset", 0).toInt();

    auto res = request(
        POST,
        "/api/v6/playlist/detail",
        data,
        Request::options(query)
    );
    QVariantList trackIds = res["body"].toMap()["playlist"].toMap()["trackIds"].toList();
    QVariantMap idsData{
        { "c", "[" + [&]() -> QString {
             QStringList c;
             for (auto& item : trackIds.mid(offset, limit)) {
                 c.append("{\"id\":" + item.toMap()["id"].toString() + "}");
             }
             return c.join(",");
         }() + "]" }
    };
    return request(
        POST,
        "/api/v3/song/detail",
        idsData,
        Request::options(query)
    );
}

// 收藏单曲到歌单 从歌单删除歌曲
QVariantMap Api::playlist_track_delete(QVariantMap query) {
    QVariantMap cookie = query["cookie"].toMap();
    cookie["os"] = "pc";
    cookie["appver"] = "2.9.7";
    query["cookie"] = cookie;
    query["ids"] = query.value("ids", "");
    const QVariantMap data{
        { "id", query["pid"] },
        { "tracks", QJsonDocument::fromVariant([&]() {
                       QVariantList items;
                       for (auto& item : query["ids"].toString().split(",")) {
                           items.push_back(QVariantMap
                                           {
                                               { "type", 3 },
                                               { "id", item }
                                           });
                       }
                       return items;
                   }()).toJson() }
    };
    return request(
        POST,
        "https://music.163.com/api/playlist/track/delete",
        data,
        {
            _WEAPI
        }
    );
}

// 收藏单曲到歌单 从歌单删除歌曲
QVariantMap Api::playlist_tracks(QVariantMap query) {
    const auto tracks = query["tracks"].toString().split(",");
    const QVariantMap data{
        { "op", query["op"] }, // del,add
        { "pid", query["pid"] }, // 歌单id
        { "trackIds", QJsonDocument::fromVariant(tracks).toJson() }, // 歌曲id
        { "imme", "true" },
    };
    auto res = request(
        POST,
        "https://music.163.com/weapi/playlist/manipulate/tracks",
        data,
        {
            _WEAPI
        }
    );
    auto code = res["body"].toMap()["code"].toInt();
    if (code == 200) {
        return {
            { "status", 200 },
            { "body", res }
        };
    }
    else if (code == 512) {
        return request(
            POST,
            "https://music.163.com/api/playlist/manipulate/tracks",
            {
                { "op", query["op"] }, // del,add
                { "pid", query["pid"] }, // 歌单id
                { "trackIds", QJsonDocument::fromVariant(tracks + tracks).toJson() }, // 歌曲id
                { "imme", "true" },
            },
            {
                _WEAPI
            }
            );
    }
    else {
        return {
            { "status", 200 },
            { "body", res["body"] }
        };
    }
}

// 歌单打卡
QVariantMap Api::playlist_update_playcount(QVariantMap query) {
    const QVariantMap data{
        { "id", query["id"] }
    };
    return request(
        POST,
        "https://music.163.com/api/playlist/update/playcount",
        data,
        {
            _WEAPI
        }
    );
}

// 编辑歌单
QVariantMap Api::playlist_update(QVariantMap query) {
    QVariantMap cookie = query["cookie"].toMap();
    cookie["os"] = "pc";
    cookie["appver"] = "2.9.7";
    query["cookie"] = cookie;
    query["desc"] = query.value("desc", "");
    query["tags"] = query.value("tags", "");
    const QVariantMap data{
        { "/api/playlist/desc/update", "{\"id\":" + query["id"].toString() + ",\"desc\":\"" + query["desc"].toString() + "\"}" },
        { "/api/playlist/tags/update", "{\"id\":" + query["id"].toString() + ",\"tags\":\"" + query["tags"].toString() + "\"}" },
        { "/api/playlist/update/name", "{\"id\":" + query["id"].toString() + ",\"name\":\"" + query["name"].toString() + "\"}" }
    };
    return request(
        POST,
        "https://music.163.com/weapi/batch",
        data,
        {
            _WEAPI
        }
    );
}

// 最近播放的视频
QVariantMap Api::playlist_video_recent(QVariantMap query) {
    const QVariantMap data{};
    return request(
        POST,
        "https://music.163.com/api/playlist/video/recent",
        data,
        {
            _WEAPI
        }
    );
}

// 智能播放
QVariantMap Api::playmode_intelligence_list(QVariantMap query) {
    const QVariantMap data{
        { "songId", query["songId"] },
        { "type", "fromPlayOne" },
        { "playlistId", query["pid"] },
        { "startMusicId", query.value("sid", query["id"]) },
        { "count", query.value("count", 1) },
    };
    return request(
        POST,
        "https://music.163.com/weapi/playmode/intelligence/list",
        data,
        {
            _WEAPI
        }
    );
}

// 云随机播放
QVariantMap Api::playmode_song_vector(QVariantMap query) {
    const QVariantMap data{
        { "ids", query["ids"] },
    };
    return request(
        POST,
        "/api/playmode/song/vector/get",
        data,
        Request::options(query)
    );
}

// 推荐节目
QVariantMap Api::program_recommend(QVariantMap query) {
    QVariantMap data{
        { "cateId", query["type"] },
        { "limit", query.value("limit", 10) },
        { "offset", query.value("offset", 0) }
    };
    return request(
        POST,
        "https://music.163.com/weapi/program/recommend/v1",
        data,
        {
            _WEAPI,
        }
    );
}

// 更换手机
QVariantMap Api::rebind(QVariantMap query) {
    const QVariantMap data{
        { "captcha", query["captcha"] },
        { "phone", query["phone"] },
        { "oldcaptcha", query["oldcaptcha"] },
        { "ctcode", query.value("ctcode", 86) },
    };
    return request(
        POST,
        "https://music.163.com/api/user/replaceCellphone",
        data,
        {
            _WEAPI
        }
    );
}

// 最近听歌列表
QVariantMap Api::recent_listen_list(QVariantMap query) {
    return request(
        POST,
        "/api/pc/recent/listen/list",
        {},
        Request::options(query)
        );
}

// 每日推荐歌单
QVariantMap Api::recommend_resource(QVariantMap query) {
    return request(
        POST,
        "https://music.163.com/weapi/v1/discovery/recommend/resource",
        {},
        {
            _WEAPI
        }
        );
}

// 每日推荐歌曲-不感兴趣
QVariantMap Api::recommend_songs_dislike(QVariantMap query) {
    const QVariantMap data{
        { "resId", query["id"] }, // 日推歌曲id
        { "resType", 4 },
        { "sceneType", 1 },
    };
    return request(
        POST,
        "https://music.163.com/weapi/v2/discovery/recommend/dislike",
        data,
        {
            _WEAPI
        }
    );
}

// 每日推荐歌曲
QVariantMap Api::recommend_songs(QVariantMap query) {
    auto cookie = query["cookie"].toMap();
    cookie["os"] = "ios";
    query["cookie"] = cookie;
    const QVariantMap data{};
    return request(
        POST,
        "https://music.163.com/api/v3/discovery/recommend/songs",
        data,
        {
            _WEAPI
        }
    );
}

// 最近播放-专辑
QVariantMap Api::record_recent_album(QVariantMap query) {
    const QVariantMap data{
        { "limit", query.value("limit", 100) }
    };
    return request(
        POST,
        "https://music.163.com/api/play-record/album/list",
        data,
        {
            _WEAPI
        }
    );
}

// 最近播放-播客
QVariantMap Api::record_recent_dj(QVariantMap query) {
    const QVariantMap data{
        { "limit", query.value("limit", 100) }
    };
    return request(
        POST,
        "https://music.163.com/api/play-record/djradio/list",
        data,
        {
            _WEAPI
        }
    );
}

// 最近播放-歌单
QVariantMap Api::record_recent_playlist(QVariantMap query) {
    const QVariantMap data{
        { "limit", query.value("limit", 100) }
    };
    return request(
        POST,
        "https://music.163.com/api/play-record/playlist/list",
        data,
        {
            _WEAPI
        }
    );
}

// 最近播放-歌曲
QVariantMap Api::record_recent_song(QVariantMap query) {
    const QVariantMap data{
        { "limit", query.value("limit", 100) }
    };
    return request(
        POST,
        "https://music.163.com/api/play-record/song/list",
        data,
        {
            _WEAPI
        }
    );
}

// 最近播放-视频
QVariantMap Api::record_recent_video(QVariantMap query) {
    const QVariantMap data{
        { "limit", query.value("limit", 100) }
    };
    return request(
        POST,
        "https://music.163.com/api/play-record/newvideo/list",
        data,
        {
            _WEAPI
        }
    );
}

// 最近播放-声音
QVariantMap Api::record_recent_voice(QVariantMap query) {
    const QVariantMap data{
        { "limit", query.value("limit", 100) }
    };
    return request(
        POST,
        "https://music.163.com/api/play-record/voice/list",
        data,
        {
            _WEAPI
        }
    );
}

// 游客登录
QVariantMap Api::register_anonimous(QVariantMap query) {
    const QByteArray ID_XOR_KEY_1 = "3go8&$833h0k(2)2";

    auto cloudmusic_dll_encode_id = [ID_XOR_KEY_1](QString some_id) {
        QString xoredString;
        for (int i = 0; i < some_id.size(); i++) {
            auto charCode = QChar(some_id[i].unicode() ^ ID_XOR_KEY_1[i % ID_XOR_KEY_1.size()]);
            xoredString.append(charCode);
        }
        const auto wordArray = xoredString.toUtf8();
        return QCryptographicHash::hash(wordArray, QCryptographicHash::Md5).toBase64();
        };

    auto cookie = query["cookie"].toMap();
    cookie["os"] = "iOS";
    query["cookie"] = cookie;
    const QString deviceId = "NMUSIC";
    const auto encodedId = QString(deviceId + " " + cloudmusic_dll_encode_id(deviceId)).toUtf8().toBase64();
    const QVariantMap data{
        { "username", encodedId },
    };

    QVariantMap result = request(
        POST,
        "https://music.163.com/api/register/anonimous",
        data,
        {
            _WEAPI
        }
    );
    if (result["body"].toMap()["code"] == 200) {
        auto body = result["body"].toMap();
        body["cookie"] = result["cookie"].toStringList().join(';');
        result = QVariantMap{
            { "status", 200 },
            { "body", body },
            { "cookie", result["cookie"] }
        };
    }
    return result;
}

// 注册账号
QVariantMap Api::register_cellphone(QVariantMap query) {
    auto cookie = query["cookie"].toMap();
    cookie["os"] = "pc";
    cookie["appver"] = "2.9.7";
    query["cookie"] = cookie;
    const QVariantMap data{
        { "captcha", query["captcha"] },
        { "phone", query["phone"] },
        { "password", QCryptographicHash::hash(query["password"].toString().toUtf8(), QCryptographicHash::Md5) },
        { "nickname", query["nickname"] },
        { "countrycode", query.value("countrycode", "86") }
    };
    return request(
        POST,
        "https://music.163.com/api/register/cellphone",
        data,
        {
            _WEAPI
        }
    );
}

// 相关视频
QVariantMap Api::related_allvideo(QVariantMap query) {
    const QVariantMap data{
        { "id", query["id"] },
        { "type", QRegularExpression("^\\d+$").match(query["id"].toString()).hasMatch() ? 0 : 1 },
    };
    return request(
        POST,
        "https://music.163.com/weapi/cloudvideo/v1/allvideo/rcmd",
        data,
        {
            _WEAPI
        }
    );
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
                QVariantMap{
                    {
                        "creator", {
                            QVariantMap {
                                { "userId", result.value(4).split("/user/home?id=").value(1) },
                                { "nickname", result.value(5) }
                            }
                        }
                    },
                    {
                        "coverImgUrl", result.value(1).split("?param=50y50").value(0)
                    },
                    {
                        "name", result.value(3)
                    },
                    {
                        "id", result.value(2).split("/playlist?id=").value(1)
                    }
                }
            )
        );
    }
    result["body"] = QVariantMap{
        { "code", 200 },
        { "playlists", QVariant(playlists) }
    };
    return result;
}

// 点赞与取消点赞资源
QVariantMap Api::resource_like(QVariantMap query) {
    QVariantMap cookie = query["cookie"].toMap();
    cookie["os"] = "android";
    query["cookie"] = cookie;
    query["t"] = query["t"] == 1 ? "like" : "unlike";
    query["type"] = resourceTypeMap[query["type"].toString()];
    QVariantMap data{
        { "threadId", query["type"].toString() + query["id"].toString() }
    };
    if (query["type"] == "A_EV_2_") {
        data["threadId"] = query["threadId"];
    }
    return request(
        POST,
        "https://music.163.com/weapi/resource/" + query["t"].toString(),
        data,
        {
            _WEAPI,
        }
    );
}

// 听歌打卡
QVariantMap Api::scrobble(QVariantMap query) {
    const QVariantMap data{
        { "logs", QJsonDocument::fromVariant(QVariantList {
                     QVariantMap {
                     { "action", "play" },
                     { "json", QVariantMap {
                                  { "download", 0 },
                                  { "end", "playend" },
                                  { "id", query["id"] },
                                  { "sourceId", query["sourceid"] },
                                  { "time", query["time"] },
                                  { "type", "song" },
                                  { "wifi", 0 },
                                  { "source", "list" },
                              } },
                     }
                 }).toJson() },
    };
    return request(
        POST,
        "https://music.163.com/weapi/feedback/weblog",
        data,
        {
            _WEAPI
        }
    );
}

// 默认搜索关键词
QVariantMap Api::search_default(QVariantMap query) {
    return request(
        POST,
        "/api/search/defaultkeyword/get",
        {},
        Request::options(query)
        );
}

// 热搜列表
QVariantMap Api::search_hot_detail(QVariantMap query) {
    return request(
        POST,
        "https://music.163.com/weapi/hotsearchlist/get",
        {},
        {
            _WEAPI,
        }
        );
}

// 热搜列表
QVariantMap Api::search_hot(QVariantMap query) {
    const QVariantMap data{
        { "type", "1111" }
    };
    return request(
        POST,
        "https://music.163.com/weapi/search/hot",
        data,
        {
            _WEAPI,
            { "ua", "mobile" },
        }
    );
}

// 本地歌曲匹配音乐信息
QVariantMap Api::search_match(QVariantMap query) {
    QVariantList songs{
        QVariantMap {
            { "title", query.value("title", "") },
            { "album", query.value("album", "") },
            { "artist", query.value("artist", "") },
            { "duration", query.value("duration", 0) },
            { "persistId", query["md5"] },
        }
    };
    const QVariantMap data{
        { "songs", QJsonDocument::fromVariant(songs).toJson() }
    };
    return request(
        POST,
        "/api/search/match/new",
        data,
        Request::options(query)
    );
}

// 多类型搜索
QVariantMap Api::search_multimatch(QVariantMap query) {
    const QVariantMap data{
        { "type", query.value("type", 1) },
        { "s", query.value("keywords", "") }
    };
    return request(
        POST,
        "https://music.163.com/weapi/search/suggest/multimatch",
        data,
        {
            _WEAPI,
        }
    );
}

// 搜索建议
QVariantMap Api::search_suggest(QVariantMap query) {
    const QVariantMap data{
        { "s", query.value("keywords", "") }
    };
    QString type = query["type"] == "mobile" ? "keyword" : "web";
    return request(
        POST,
        "https://music.163.com/weapi/search/suggest/" + type,
        data,
        {
            _WEAPI,
        }
    );
}

// 搜索
QVariantMap Api::search(QVariantMap query) {
    if (query.contains("type") && query["type"] == "2000") {
        const QVariantMap data{
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
                _WEAPI
            }
        );
    }
    QVariantMap cookie = query["cookie"].toMap();
    cookie["os"] = "pc";
    query["cookie"] = cookie;
    const QVariantMap data{
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
            _WEAPI
        }
    );
}

// 私信专辑
QVariantMap Api::send_album(QVariantMap query) {
    QVariantMap data{
        { "id", query["id"] },
        { "msg", query.value("msg", "") },
        { "type", "album" },
        { "userIds", "[" + query["user_ids"].toString() + "]" },
    };
    return request(
        POST,
        "/api/msg/private/send",
        data,
        Request::options(query)
    );
}

// 私信歌单
QVariantMap Api::send_playlist(QVariantMap query) {
    QVariantMap data{
        { "id", query["playlist"] },
        { "type", "playlist" },
        { "msg", query.value("msg", "") },
        { "userIds", "[" + query["user_ids"].toString() + "]" },
    };
    return request(
        POST,
        "https://music.163.com/weapi/msg/private/send",
        data,
        {
            _WEAPI,
        }
    );
}

// 私信歌曲
QVariantMap Api::send_song(QVariantMap query) {
    QVariantMap data{
        { "id", query["id"] },
        { "msg", query.value("msg", "") },
        { "type", "song" },
        { "userIds", "[" + query["user_ids"].toString() + "]" },
    };
    return request(
        POST,
        "/api/msg/private/send",
        data,
        Request::options(query)
    );
}

// 私信
QVariantMap Api::send_text(QVariantMap query) {
    QVariantMap data{
        { "type", "playlist" },
        { "msg", query.value("msg", "") },
        { "userIds", "[" + query["user_ids"].toString() + "]" },
    };
    return request(
        POST,
        "https://music.163.com/weapi/msg/private/send",
        data,
        {
            _WEAPI,
        }
    );
}

// 设置
QVariantMap Api::setting(QVariantMap query) {
    QVariantMap data{};
    return request(
        POST,
        "https://music.163.com/api/user/setting",
        data,
        {
            _WEAPI,
        }
    );
}

// 分享歌曲到动态
QVariantMap Api::share_resource(QVariantMap query) {
    QVariantMap data{
        { "type", query.value("type", "song") }, // song,playlist,mv,djprogram,djradio,noresource
        { "msg", query.value("msg", "") },
        { "id", query.value("id", "") },
    };
    return request(
        POST,
        "https://music.163.com/weapi/share/friends/resource",
        data,
        {
            _WEAPI,
        }
    );
}

// 乐谱列表
QVariantMap Api::sheet_list(QVariantMap query) {
    QVariantMap data{
        { "id", query["id"] },
        { "abTest", query.value("ab", "b") },
    };
    return request(
        POST,
        "/api/music/sheet/list/v1",
        data,
        Request::options(query)
    );
}

// 乐谱预览
QVariantMap Api::sheet_preview(QVariantMap query) {
    QVariantMap data{
        { "id", query["id"] },
    };
    return request(
        POST,
        "/api/music/sheet/preview/info",
        data,
        Request::options(query)
    );
}

// 乐签信息
QVariantMap Api::sign_happy_info(QVariantMap query) {
    QVariantMap data{};
    return request(
        POST,
        "https://music.163.com/api/sign/happy/info",
        data,
        {
            _WEAPI
        }
    );
}

// 签到进度
QVariantMap Api::signin_progress(QVariantMap query) {
    QVariantMap data{
        { "moduleId", query.value("moduleId", "1207signin-1207signin") }
    };
    return request(
        POST,
        "https://music.163.com/weapi/act/modules/signin/v2/progress",
        data,
        {
            _WEAPI
        }
    );
}

// 相似歌手
QVariantMap Api::simi_artist(QVariantMap query) {
    QVariantMap data{
        { "artistid", query["id"] },
    };
    return request(
        POST,
        "https://music.163.com/weapi/discovery/simiArtist",
        data,
        {
            _WEAPI,
        }
    );
}

// 相似MV
QVariantMap Api::simi_mv(QVariantMap query) {
    QVariantMap data{
        { "mvid", query["mvid"] },
    };
    return request(
        POST,
        "https://music.163.com/weapi/discovery/simiMV",
        data,
        {
            _WEAPI,
        }
    );
}

// 相似歌单
QVariantMap Api::simi_playlist(QVariantMap query) {
    QVariantMap data{
        { "songid", query["id"] },
        { "limit", query.value("limit", 50) },
        { "offset", query.value("offset", 0) }
    };
    return request(
        POST,
        "https://music.163.com/weapi/discovery/simiPlaylist",
        data,
        {
            _WEAPI,
        }
    );
}

// 相似歌曲
QVariantMap Api::simi_song(QVariantMap query) {
    QVariantMap data{
        { "songid", query["id"] },
        { "limit", query.value("limit", 50) },
        { "offset", query.value("offset", 0) }
    };
    return request(
        POST,
        "https://music.163.com/weapi/v1/discovery/simiSong",
        data,
        {
            _WEAPI,
        }
    );
}

// 相似用户
QVariantMap Api::simi_user(QVariantMap query) {
    QVariantMap data{
        { "songid", query["id"] },
        { "limit", query.value("limit", 50) },
        { "offset", query.value("offset", 0) }
    };
    return request(
        POST,
        "https://music.163.com/weapi/discovery/simiUser",
        data,
        {
            _WEAPI,
        }
    );
}

// 获取客户端歌曲下载链接
QVariantMap Api::song_download_url(QVariantMap query) {
    QVariantMap data{
        { "id", query["id"] },
        { "br", query.value("br", 999000) }
    };
    return request(
        POST,
        "/api/song/enhance/download/url",
        data,
        Request::options(query)
    );
}

// 歌曲链接 - v1
// 此版本不再采用 br 作为音质区分的标准
// 而是采用 standard, exhigh, lossless, hires, jyeffect(高清环绕声), sky(沉浸环绕声), jymaster(超清母带) 进行音质判断
QVariantMap Api::song_url_v1(QVariantMap query) {
    QVariantMap data{
        { "ids", "[" + query["id"].toString() + "]" },
        { "level", query["level"].toString() },
        { "encodeType", "flac" }
    };
    if (data["level"] == "sky") {
        data["immerseType"] = "c51";
    }
    return request(
        POST,
        "/api/song/enhance/player/url/v1",
        data,
        Request::options(query)
    );
}

// 歌曲链接
QVariantMap Api::song_url(QVariantMap query) {
    const auto ids = query["id"].toString().split(",");
    const QVariantMap data{
        { "ids", QJsonDocument::fromVariant(ids).toJson() },
        { "br", query.value("br", 999000) },
    };
    const auto res = request(
        POST,
        "/api/song/enhance/player/url/v1",
        data,
        Request::options(query)
    );
    auto result = res["body"].toMap()["data"].toList();
    std::sort(result.begin(), result.end(), [&ids](const QVariant& a, const QVariant& b) {
        return ids.indexOf(QString::number(a.toMap()["id"].toInt())) < ids.indexOf(QString::number(b.toMap()["id"].toInt()));
        });

    return QVariantMap{
        { "status", 200 },
        { "body", QVariantMap
            {
                { "code", 200 },
                { "data", result }
            }
        }
    };
}

// 歌曲详情
QVariantMap Api::song_detail(QVariantMap query) {
    query["ids"] = query["ids"].toString().split(QRegularExpression("\\s*,\\s*"));
    const QVariantMap data{
        { "c", "[" + [&]() -> QString {
             QStringList c;
             for (auto& item : query["ids"].toStringList()) {
                 c.append("{\"id\":" + item + "}");
             }
             return c.join(",");
         }() + "]" }
    };
    return request(
        POST,
        "/api/v3/song/detail",
        data,
        Request::options(query)
    );
}

// 音乐百科基础信息
QVariantMap Api::song_wiki_summary(QVariantMap query) {
    QVariantMap data{
        { "songId", query["id"] }
    };
    return request(
        POST,
        "/api/song/play/about/block/page",
        data,
        Request::options(query)
    );
}

// 云村星评馆 - 简要评论列表
QVariantMap Api::starpick_comments_summary(QVariantMap query) {
    QVariantMap data{
        { "cursor", QJsonDocument::fromVariant(
                       QVariantMap {
                           { "offset", 0 },
                           { "blockCodeOrderList", QVariantList { "HOMEPAGE_BLOCK_NEW_HOT_COMMENT" } },
                           { "refresh", true },
                       }).toJson() }
    };
    return request(
        POST,
        "/api/homepage/block/page",
        data,
        Request::options(query)
    );
}

// 歌曲音质详情
QVariantMap Api::song_music_detail(QVariantMap query) {
    QVariantMap data{
        { "songId", query["id"] }
    };
    return request(
        POST,
        "/api/song/music/detail/get",
        data,
        Request::options(query)
    );
}

// 更新歌曲顺序
QVariantMap Api::song_order_update(QVariantMap query) {
    QVariantMap data{
        { "pid", query["pid"] },
        { "trackIds", query["ids"] },
        { "op", "update" },
    };
    return request(
        POST,
        "http://interface.music.163.com/api/playlist/manipulate/tracks",
        data,
        {
            _WEAPI,
            { "url", "/api/playlist/desc/update" }
        }
    );
}

// 已购单曲
QVariantMap Api::song_purchased(QVariantMap query) {
    QVariantMap data{
        { "limit", query.value("limit", 20) },
        { "offset", query.value("offset", 0) },
    };
    return request(
        POST,
        "https://music.163.com/weapi/single/mybought/song/list",
        data,
        {
            _WEAPI,
        }
    );
}

// 歌曲红心数量
QVariantMap Api::song_red_count(QVariantMap query) {
    QVariantMap data{
        { "songId", query["id"] }
    };
    return request(
        POST,
        "/api/song/red/count",
        data,
        Request::options(query)
    );
}

// 曲风-专辑
QVariantMap Api::style_album(QVariantMap query) {
    QVariantMap data{
        { "cursor", query.value("cursor", 0) },
        { "size", query.value("size", 20) },
        { "tagId", query["tagId"] },
        { "sort", query.value("sort", 0) },
    };
    return request(
        POST,
        "https://music.163.com/api/style-tag/home/album",
        data,
        {
            _WEAPI,
        }
    );
}

// 曲风-歌手
QVariantMap Api::style_artist(QVariantMap query) {
    QVariantMap data{
        { "cursor", query.value("cursor", 0) },
        { "size", query.value("size", 20) },
        { "tagId", query["tagId"] },
        { "sort", query.value("sort", 0) },
    };
    return request(
        POST,
        "https://music.163.com/api/style-tag/home/artist",
        data,
        {
            _WEAPI,
        }
    );
}

// 曲风详情
QVariantMap Api::style_detail(QVariantMap query) {
    QVariantMap data{
        { "tagId", query["tagId"] },
    };
    return request(
        POST,
        "https://music.163.com/api/style-tag/home/head",
        data,
        {
            _WEAPI,
        }
    );
}

// 曲风列表
QVariantMap Api::style_list(QVariantMap query) {
    QVariantMap data{};
    return request(
        POST,
        "https://music.163.com/api/tag/list/get",
        data,
        {
            _WEAPI,
        }
    );
}

// 曲风-歌单
QVariantMap Api::style_playlist(QVariantMap query) {
    QVariantMap data{
        { "cursor", query.value("cursor", 0) },
        { "size", query.value("size", 20) },
        { "tagId", query["tagId"] },
        { "sort", query.value("sort", 0) },
    };
    return request(
        POST,
        "https://music.163.com/api/style-tag/home/playlist",
        data,
        {
            _WEAPI,
        }
    );
}

// 曲风偏好
QVariantMap Api::style_preference(QVariantMap query) {
    QVariantMap data{};
    return request(
        POST,
        "https://music.163.com/api/tag/my/preference/get",
        data,
        {
            _WEAPI,
        }
    );
}

// 曲风-歌曲
QVariantMap Api::style_song(QVariantMap query) {
    QVariantMap data{
        { "cursor", query.value("cursor", 0) },
        { "size", query.value("size", 20) },
        { "tagId", query["tagId"] },
        { "sort", query.value("sort", 0) },
    };
    return request(
        POST,
        "https://music.163.com/api/style-tag/home/song",
        data,
        {
            _WEAPI,
        }
    );
}

// 年度听歌报告2017-2023
QVariantMap Api::summary_annual(QVariantMap query) {
    QVariantMap data{ };
    const QString key = QStringList{ "2017", "2018", "2019 " }.indexOf(query["year"].toString()) > -1 ? "userdata" : "data";
    return request(
        POST,
        "/api/activity/summary/annual/" + query["year"].toString() + "/" + key,
        data,
        Request::options(query)
    );
}

// 获取达人达标信息
QVariantMap Api::threshold_detail_get(QVariantMap query) {
    const QVariantMap data{ };
    return request(
        POST,
        "/api/influencer/web/apply/threshold/detail/get",
        data,
        Request::options(query)
    );
}

// 新碟上架
QVariantMap Api::top_album(QVariantMap query) {
    QDateTime date = QDateTime::currentDateTime();
    QVariantMap data{
        { "area", query.value("area", "ALL") }, // //ALL:全部,ZH:华语,EA:欧美,KR:韩国,JP:日本
        { "limit", query.value("limit", 50) },
        { "offset", query.value("offset", 0) },
        { "type", query.value("type", "new") },
        { "year", query.value("year", date.date().year()) },
        { "month", query.value("month", date.date().month()) },
        { "total", false },
        { "rcmd", true }
    };
    return request(
        POST,
        "https://music.163.com/api/discovery/new/albums/area",
        data,
        {
            _WEAPI
        }
    );
}

// 热门歌手
QVariantMap Api::top_artists(QVariantMap query) {
    QVariantMap data{
        { "limit", query.value("limit", 50) },
        { "offset", query.value("offset", 0) },
        { "total", true },
    };
    return request(
        POST,
        "https://music.163.com/weapi/artist/top",
        data,
        {
            _WEAPI
        }
    );
}

// 排行榜
QVariantMap Api::top_list(QVariantMap query) {
    QVariantMap cookie = query["cookie"].toMap();
    cookie["os"] = "pc";
    cookie["appver"] = "2.9.7";
    query["cookie"] = cookie;
    if (query.contains("idx")) {
        return {
            { "status", 500 },
            { "body", QVariantMap{
                { "code", 500 },
                { "msg", "不支持此方式调用,只支持id调用" }
            }}
        };
    }

    QVariantMap data{
        { "id", query["id"] },
        { "n", "500" },
        { "s", "0"},
    };
    return request(
        POST,
        "https://interface3.music.163.com/api/playlist/v4/detail",
        data,
        {
            _WEAPI
        }
    );
}

// MV排行榜
QVariantMap Api::top_mv(QVariantMap query) {
    QVariantMap data{
        { "area", query.value("area", "") },
        { "limit", query.value("limit", 50) },
        { "offset", query.value("offset", 0) },
        { "total", true },
    };
    return request(
        POST,
        "https://music.163.com/weapi/mv/toplist",
        data,
        {
            _WEAPI
        }
    );
}

// 精品歌单
QVariantMap Api::top_playlist_highquality(QVariantMap query) {
    QVariantMap data{
        { "cat", query.value("cat", "全部") }, // 全部,华语,欧美,韩语,日语,粤语,小语种,运动,ACG,影视原声,流行,摇滚,后摇,古风,民谣,轻音乐,电子,器乐,说唱,古典,爵士
        { "limit", query.value("limit", 50) },
        { "lasttime", query.value("lasttime", 0) }, // 歌单updateTime
        { "total", true },
    };
    return request(
        POST,
        "https://music.163.com/api/playlist/highquality/list",
        data,
        {
            _WEAPI
        }
    );
}

// 分类歌单
QVariantMap Api::top_playlist(QVariantMap query) {
    QVariantMap data{
        { "cat", query.value("cat", "全部") }, // 全部,华语,欧美,韩语,日语,粤语,小语种,运动,ACG,影视原声,流行,摇滚,后摇,古风,民谣,轻音乐,电子,器乐,说唱,古典,爵士
        { "order", query.value("order", "hot") }, // hot,new
        { "limit", query.value("limit", 50) },
        { "offset", query.value("offset", 0) },
        { "total", true },
    };
    const auto res = request(
        POST,
        "https://music.163.com/weapi/playlist/list",
        data,
        {
            _WEAPI
        }
    );
    const auto result = QString(QJsonDocument::fromVariant(res).toJson())
        .replace(QRegularExpression("avatarImgId_str"), "avatarImgIdStr");
    return QJsonDocument::fromJson(result.toUtf8()).toVariant().toMap();
}

// 新歌速递
QVariantMap Api::top_song(QVariantMap query) {
    QVariantMap data{
        { "areaId", query.value("type", 0) }, // 全部:0 华语:7 欧美:96 日本:8 韩国:16
        // { "limit", query.value("limit", 100) },
        // { "offset", query.value("offset", 0) },
        { "total", true },
    };
    return request(
        POST,
        "https://music.163.com/weapi/v1/discovery/new/songs",
        data,
        {
            _WEAPI
        }
    );
}

// 获取话题详情热门动态
QVariantMap Api::topic_detail_event_hot(QVariantMap query) {
    QVariantMap data{
        { "actid", query["actid"] },
    };
    return request(
        POST,
        "https://music.163.com/api/act/event/hot",
        data,
        {
            _WEAPI
        }
    );
}

// 获取话题详情
QVariantMap Api::topic_detail(QVariantMap query) {
    QVariantMap data{
        { "actid", query["actid"] },
    };
    return request(
        POST,
        "https://music.163.com/api/act/detail",
        data,
        {
            _WEAPI
        }
    );
}

// 收藏的专栏
QVariantMap Api::topic_sublist(QVariantMap query) {
    QVariantMap data{
        { "limit", query.value("limit", 50) },
        { "offset", query.value("offset", 0) },
        { "total", true },
    };
    return request(
        POST,
        "https://music.163.com/api/topic/sublist",
        data,
        {
            _WEAPI
        }
    );
}

// 歌手榜
QVariantMap Api::toplist_artist(QVariantMap query) {
    QVariantMap data{
        { "type", query.value("type", 1) },
        { "limit", query.value("limit", 100) },
        { "offset", query.value("offset", 0) },
        { "total", true },
    };
    return request(
        POST,
        "https://music.163.com/weapi/toplist/artist",
        data,
        {
            _WEAPI
        }
    );
}

// 所有榜单内容摘要
QVariantMap Api::toplist_detail(QVariantMap query) {
    return request(
        POST,
        "https://music.163.com/weapi/toplist/detail",
        {},
        {
            _WEAPI
        }
        );
}

// 所有榜单介绍
QVariantMap Api::toplist(QVariantMap query) {
    return request(
        POST,
        "/api/toplist",
        {},
        Request::options(query)
        );
}

// 专辑简要百科信息
QVariantMap Api::ugc_album_get(QVariantMap query) {
    const QVariantMap data{
        { "albumId", query["id"] }
    };
    return request(
        POST,
        "/api/rep/ugc/album/get",
        data,
        Request::options(query)
    );
}

// 歌手简要百科信息
QVariantMap Api::ugc_artist_get(QVariantMap query) {
    const QVariantMap data{
        { "artistId", query["id"] }
    };
    return request(
        POST,
        "/api/rep/ugc/artist/get",
        data,
        Request::options(query)
    );
}

// 搜索歌手
// 可传关键字或者歌手id
QVariantMap Api::ugc_artist_search(QVariantMap query) {
    const QVariantMap data{
        { "keyword", query["keyword"] },
        { "limit", query.value("limit", 40) }
    };
    return request(
        POST,
        "https://music.163.com/api/rep/ugc/artist/search",
        data,
        {
            _WEAPI,
            { "url", "/api/rep/ugc/artist/search" },
        }
    );
}

// 用户贡献内容
QVariantMap Api::ugc_detail(QVariantMap query) {
    const QVariantMap data{
        { "auditStatus", query.value("auditStatus", "") },
        //待审核:0 未采纳:-5 审核中:1 部分审核通过:4 审核通过:5
        //WAIT:0 REJECT:-5 AUDITING:1 PARTLY_APPROVED:4 PASS:5
        { "limit", query.value("limit", 10) },
        { "offset", query.value("offset", 0) },
        { "order", query.value("order", "desc") },
        { "sortBy", query.value("sortBy", "createTime") },
        { "type", query.value("type", 1) },
        //曲库纠错 ARTIST:1 ALBUM:2 SONG:3 MV:4 LYRIC:5 TLYRIC:6
        //曲库补充 ALBUM:101 MV:103
    };
    return request(
        POST,
        "https://music.163.com/weapi/rep/ugc/detail",
        data,
        {
            _WEAPI,
        }
    );
}

// mv简要百科信息
QVariantMap Api::ugc_mv_get(QVariantMap query) {
    const QVariantMap data{
        { "mvId", query["id"] }
    };
    return request(
        POST,
        "/api/rep/ugc/mv/get",
        data,
        Request::options(query)
    );
}

// 歌曲简要百科信息
QVariantMap Api::ugc_song_get(QVariantMap query) {
    const QVariantMap data{
        { "songId", query["id"] }
    };
    return request(
        POST,
        "/api/rep/ugc/song/get",
        data,
        Request::options(query)
    );
}

// 用户贡献条目、积分、云贝数量
QVariantMap Api::ugc_user_devote(QVariantMap query) {
    const QVariantMap data{};
    return request(
        POST,
        "/api/rep/ugc/user/devote",
        data,
        Request::options(query)
    );
}

// 获取账号信息
QVariantMap Api::user_account(QVariantMap query) {
    const QVariantMap data{};
    return request(
        POST,
        "https://music.163.com/api/nuser/account/get",
        data,
        {
            _WEAPI
        }
    );
}

// 用户创建的电台
QVariantMap Api::user_audio(QVariantMap query) {
    const QVariantMap data{
        { "userId", query["uid"] }
    };
    return request(
        POST,
        "https://music.163.com/weapi/djradio/get/byuser",
        data,
        {
            _WEAPI
        }
    );
}

// 获取用户绑定信息
QVariantMap Api::user_binding(QVariantMap query) {
    const QVariantMap data{ };
    return request(
        POST,
        "https://music.163.com/api/v1/user/bindings/" + query["uid"].toString(),
        data,
        {
            _WEAPI
        }
    );
}

// 云盘歌曲删除
QVariantMap Api::user_cloud_del(QVariantMap query) {
    const QVariantMap data{
        { "songIds", QVariantList{ query["id"] } }
    };
    return request(
        POST,
        "https://music.163.com/weapi/cloud/del",
        data,
        {
            _WEAPI
        }
    );
}

// 云盘数据详情
QVariantMap Api::user_cloud_detail(QVariantMap query) {
    const auto id = query["id"].toString().replace("\\s", "").split(",");
    const QVariantMap data{
        { "songIds", id }
    };
    return request(
        POST,
        "https://music.163.com/weapi/v1/cloud/get/byids",
        data,
        {
            _WEAPI
        }
    );
}

// 云盘数据
QVariantMap Api::user_cloud(QVariantMap query) {
    const QVariantMap data{
        { "limit", query.value("limit", 30) },
        { "offset", query.value("offset", 0) }
    };
    return request(
        POST,
        "https://music.163.com/api/v1/cloud/get",
        data,
        {
            _WEAPI
        }
    );
}

// 获取用户历史评论
QVariantMap Api::user_comment_history(QVariantMap query) {
    const QVariantMap data{
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
            _WEAPI
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
            _WEAPI
        }
        );
}

// 用户电台节目
QVariantMap Api::user_dj(QVariantMap query) {
    const QVariantMap data{
        { "limit", query.value("limit", 30) },
        { "offset", query.value("offset", 0) }
    };
    return request(
        POST,
        "https://music.163.com/weapi/dj/program/" + query["uid"].toString(),
        data,
        {
            _WEAPI
        }
    );
}

// 用户动态
QVariantMap Api::user_event(QVariantMap query) {
    const QVariantMap data{
        { "getcounts", true },
        { "time", query.value("lasttime", -1) },
        { "limit", query.value("limit", 30) },
        { "total", false }
    };
    return request(
        POST,
        "/api/event/get/" + query["uid"].toString(),
        data,
        Request::options(query)
    );
}

// 关注TA的人(粉丝)
QVariantMap Api::user_followeds(QVariantMap query) {
    const QVariantMap data{
        { "userId", query["uid"] },
        { "time", "0" },
        { "limit", query.value("limit", 30) },
        { "offset", query.value("offset", 0) },
        { "getcounts", "true" }
    };
    return request(
        POST,
        "/api/user/getfolloweds/" + query["uid"].toString(),
        data,
        Request::options(query)
    );
}

// TA关注的人(关注)
QVariantMap Api::user_follows(QVariantMap query) {
    const QVariantMap data{
        { "offset", query.value("offset", 0) },
        { "limit", query.value("limit", 30) },
        { "order", true }
    };
    return request(
        POST,
        "https://music.163.com/weapi/user/getfollows/" + query["uid"].toString(),
        data,
        {
            _WEAPI
        }
    );
}

// 获取用户等级信息
QVariantMap Api::user_level(QVariantMap query) {
    const QVariantMap data{ };
    return request(
        POST,
        "https://music.163.com/weapi/user/level",
        data,
        {
            _WEAPI
        }
    );
}

// 用户歌单
QVariantMap Api::user_playlist(QVariantMap query) {
    const QVariantMap data{
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
            _WEAPI
        }
    );
}

// 听歌排行
QVariantMap Api::user_record(QVariantMap query) {
    const QVariantMap data{
        { "uid", query["uid"] },
        { "type", query.value("type", 0) } // 1: 最近一周, 0: 所有时间
    };
    return request(
        POST,
        "https://music.163.com/weapi/v1/play/record",
        data,
        {
            _WEAPI
        }
    );
}

// 用户绑定手机
QVariantMap Api::user_replacephone(QVariantMap query) {
    const QVariantMap data{
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
            _WEAPI
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
            _WEAPI
        }
        );
}

// 编辑用户信息
QVariantMap Api::user_update(QVariantMap query) {
    const QVariantMap data{
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
            _WEAPI
        }
    );
}

// 验证接口-二维码生成
QVariantMap Api::verify_getQr(QVariantMap query) {
    const QVariantMap data{
        { "verifyConfigId", query["vid"] },
        { "verifyType", query["type"] },
        { "token", query["token"] },
        { "params", QJsonDocument::fromVariant(
                       QVariantMap {
                            { "event_id", query["evid"] },
                            { "sign", query["sign"] },
                   }).toJson()
        },
        { "size", 150 },
    };
    const auto res = request(
        POST,
        "https://music.163.com/weapi/frontrisk/verify/qrcodestatus",
        data,
        {
            _WEAPI
        }
    );
    const QString result = "https://st.music.163.com/encrypt-pages?qrCode="
        + res["body"].toMap()["data"].toMap()["qrCode"].toString()
        + "&verifyToken=" + query["token"].toString() + "&verifyId=" + query["vid"].toString() + "&verifyType="
        + query["type"].toString()
        + "&params=" + QJsonDocument::fromVariant(
            QVariantMap{
                        { "event_id", query["evid"] },
                        { "sign", query["sign"] },
            }).toJson();
    return {
        { "status", 200 },
        { "body", QVariantMap {
                     { "qrCode", res["body"].toMap()["data"].toMap()["qrCode"] },
                     { "qrurl", result },
                 } },
    };
}

// 验证接口-二维码检测
QVariantMap Api::verify_qrcodestatus(QVariantMap query) {
    const QVariantMap data{
        { "qrCode", query["qr"] },
    };
    return request(
        POST,
        "https://music.163.com/weapi/frontrisk/verify/qrcodestatus",
        data,
        {
            _WEAPI
        }
    );
}

// 视频分类列表
QVariantMap Api::video_category_list(QVariantMap query) {
    const QVariantMap data{
        { "offset", query.value("offset", 0) },
        { "total", true },
        { "limit", query.value("limit", 99) },
    };
    return request(
        POST,
        "https://music.163.com/api/cloudvideo/category/list",
        data,
        {
            _WEAPI
        }
    );
}

// 视频点赞转发评论数数据
QVariantMap Api::video_detail_info(QVariantMap query) {
    const QVariantMap data{
        { "threadid", "R_VI_62_" + query["vid"].toString() },
        { "composeliked", true },
    };
    return request(
        POST,
        "https://music.163.com/api/comment/commentthread/info",
        data,
        {
            _WEAPI
        }
    );
}

// 视频详情
QVariantMap Api::video_detail(QVariantMap query) {
    const QVariantMap data{
        { "id", query["id"] },
    };
    return request(
        POST,
        "https://music.163.com/weapi/cloudvideo/v1/video/detail",
        data,
        {
            _WEAPI
        }
    );
}

// 视频标签列表
QVariantMap Api::video_group_list(QVariantMap query) {
    const QVariantMap data{};
    return request(
        POST,
        "https://music.163.com/api/cloudvideo/group/list",
        data,
        {
            _WEAPI
        }
    );
}

// 视频标签/分类下的视频
QVariantMap Api::video_group(QVariantMap query) {
    const QVariantMap data{
        { "groupId", query["id"] },
        { "offset", query.value("offset", 0) },
        { "need_preview_url", "true" },
        { "total", true },
    };
    return request(
        POST,
        "https://music.163.com/api/videotimeline/videogroup/otherclient/get",
        data,
        {
            _WEAPI
        }
    );
}

// 收藏与取消收藏视频
QVariantMap Api::video_sub(QVariantMap query) {
    query["t"] = query["t"] == 1 ? "sub" : "unsub";
    const QVariantMap data{
        { "id", query["id"] }
    };
    return request(
        POST,
        "https://music.163.com/weapi/cloudvideo/video/" + query["t"].toString(),
        data,
        {
            _WEAPI
        }
    );
}

// 全部视频列表
QVariantMap Api::video_timeline_all(QVariantMap query) {
    const QVariantMap data{
        { "groupId", 0 },
        { "offset", query.value("offset", 0) },
        { "need_preview_url", "true" },
        { "total", true },
    };
    return request(
        POST,
        "https://music.163.com/api/videotimeline/otherclient/get",
        data,
        {
            _WEAPI
        }
    );
}

// 推荐视频
QVariantMap Api::video_timeline_recommend(QVariantMap query) {
    const QVariantMap data{
        { "offset", query.value("offset", 0) },
        { "filterLives", "[]" },
        { "withProgramInfo", "true" },
        { "needUrl", "1" },
        { "resolution", "480" },
    };
    return request(
        POST,
        "https://music.163.com/api/videotimeline/get",
        data,
        {
            _WEAPI
        }
    );
}

// 视频链接
QVariantMap Api::video_url(QVariantMap query) {
    const QVariantMap data{
        { "ids", "[\"" + query["id"].toString() + "\"]" },
        { "resolution", query.value("res", 1080) },
    };
    return request(
        POST,
        "https://music.163.com/weapi/cloudvideo/playurl",
        data,
        {
            _WEAPI
        }
    );
}

// 黑胶时光机
QVariantMap Api::vip_timemachine(QVariantMap query) {
    QVariantMap data{};
    if (query.contains("startTime") && query.contains("endTime")) {
        data["startTime"] = query["startTime"];
        data["endTime"] = query["endTime"];
        data["type"] = 1;
        data["limit"] = query.value("limit", 60);
    }
    return request(
        POST,
        "https://music.163.com/weapi/vipmusic/newrecord/weekflow",
        data,
        {
            _WEAPI
        }
    );
}

// 播客删除
QVariantMap Api::voice_delete(QVariantMap query) {
    const QVariantMap data{
        { "ids", query["ids"] }
    };
    return request(
        POST,
        "https://interface.music.163.com/api/content/voice/delete",
        data,
        {
            _WEAPI
        }
    );
}

// 播客声音详情
QVariantMap Api::voice_detail(QVariantMap query) {
    const QVariantMap data{
        { "id", query["id"] }
    };
    return request(
        POST,
        "https://interface.music.163.com/weapi/voice/workbench/voice/detail",
        data,
        {
            _WEAPI
        }
    );
}

// 获取声音歌词
QVariantMap Api::voice_lyric(QVariantMap query) {
    const QVariantMap data{
        { "programId", query["id"] }
    };
    return request(
        POST,
        "/api/voice/lyric/get",
        data,
        Request::options(query)
    );
}

// 声音搜索
QVariantMap Api::voicelist_list_search(QVariantMap query) {
    const QVariantMap data{
        { "limit", query.value("limit", 200) },
        { "offset", query.value("offset", 0) },
        { "name", query.value("name") },
        { "displayStatus", query.value("displayStatus") },
        { "type", query.value("type") },
        { "voiceFeeType", query.value("voiceFeeType") },
        { "radioId", query["voiceListId"] },
    };
    return request(
        POST,
        "https://interface.music.163.com/api/voice/workbench/voice/list",
        data,
        {
            _WEAPI
        }
    );
}

// 播客声音列表
QVariantMap Api::voicelist_list(QVariantMap query) {
    const QVariantMap data{
        { "limit", query.value("limit", 200) },
        { "offset", query.value("offset", 0) },
        { "voiceListId", query["voiceListId"] },
    };
    return request(
        POST,
        "https://interface.music.163.com/weapi/voice/workbench/voices/by/voicelist",
        data,
        {
            _WEAPI
        }
    );
}

// 播客列表详情
QVariantMap Api::voicelist_detail(QVariantMap query) {
    const QVariantMap data{
        { "id", query["id"] }
    };
    return request(
        POST,
        "https://interface.music.163.com/weapi/voice/workbench/voicelist/detail",
        data,
        {
            _WEAPI,
        }
    );
}

// 播客列表
QVariantMap Api::voicelist_search(QVariantMap query) {
    const QVariantMap data{
        { "fee", "-1" },
        { "limit", query.value("limit", 200) },
        { "offset", query.value("offset", 0) },
        { "podcastName", query.value("podcastName", "") },
    };
    return request(
        POST,
        "https://interface.music.163.com/weapi/voice/workbench/voicelist/search",
        data,
        {
            _WEAPI
        }
    );
}

// 播客声音排序
QVariantMap Api::voicelist_trans(QVariantMap query) {
    const QVariantMap data{
        { "limit", query.value("limit", 200) }, // 每页数量
        { "offset", query.value("offset", 0) }, // 偏移量
        { "radioId", query["radioId"] }, // 电台id
        { "programId", query.value("programId", "0") }, // 节目id
        { "position", query.value("position", "1") }, // 排序编号
    };
    return request(
        POST,
        "https://interface.music.163.com/api/voice/workbench/radio/program/trans",
        data,
        {
            _WEAPI
        }
    );
}

// 操作记录
QVariantMap Api::weblog(QVariantMap query) {
    return request(
        POST,
        "https://music.163.com/weapi/feedback/weblog",
        query.value("data", QVariantMap()).toMap(),
        {
            _WEAPI
        }
    );
}

// 云贝 todo 任务
QVariantMap Api::yunbei_tasks_todo(QVariantMap query) {
    const QVariantMap data{};
    // /api/point/today/get
    return request(
        POST,
        "https://music.163.com/api/usertool/task/todo/query",
        data,
        {
            _WEAPI
        }
    );
}

// 云贝所有任务
QVariantMap Api::yunbei_tasks(QVariantMap query) {
    const QVariantMap data{};
    // /api/point/today/get
    return request(
        POST,
        "https://music.163.com/api/usertool/task/list/all",
        data,
        {
            _WEAPI
        }
    );
}

// 云贝今日签到信息
QVariantMap Api::yunbei_today(QVariantMap query) {
    const QVariantMap data{};
    // /api/point/today/get
    return request(
        POST,
        "https://music.163.com/api/point/today/get",
        data,
        {
            _WEAPI
        }
    );
}

// 云贝
QVariantMap Api::yunbei(QVariantMap query) {
    const QVariantMap data{};
    // /api/point/today/get
    return request(
        POST,
        "https://music.163.com/api/point/signed/get",
        data,
        {
            _WEAPI
        }
    );
}

