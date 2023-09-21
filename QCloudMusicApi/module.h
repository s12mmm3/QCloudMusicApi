#ifndef MODULE_H
#define MODULE_H

#include <QVariantMap>
#include <QObject>
#include <QByteArray>
#include <QtCore/qglobal.h>

#if defined(QCLOUDMUSICAPI_LIBRARY)
#  define QCLOUDMUSICAPI_EXPORT Q_DECL_EXPORT
#else
#  define QCLOUDMUSICAPI_EXPORT Q_DECL_IMPORT
#endif

class QCLOUDMUSICAPI_EXPORT NeteaseCloudMusicApi: public QObject {
    Q_OBJECT

public:
    NeteaseCloudMusicApi();
    ~NeteaseCloudMusicApi();

    // 初始化名字
    Q_INVOKABLE const QVariantMap activate_init_profile(QVariantMap query);

    // 专辑动态信息
    Q_INVOKABLE const QVariantMap album_detail_dynamic(QVariantMap query);

    // 数字专辑详情
    Q_INVOKABLE const QVariantMap album_detail(QVariantMap query);

    // 数字专辑-语种风格馆
    Q_INVOKABLE const QVariantMap album_list_style(QVariantMap query);

    // 数字专辑-新碟上架
    Q_INVOKABLE const QVariantMap album_list(QVariantMap query);

    // 全部新碟
    Q_INVOKABLE const QVariantMap album_new(QVariantMap query);

    // 最新专辑
    Q_INVOKABLE const QVariantMap album_newest(QVariantMap query);

    // 数字专辑&数字单曲-榜单
    Q_INVOKABLE const QVariantMap album_songsaleboard(QVariantMap query);

    // 收藏/取消收藏专辑
    Q_INVOKABLE const QVariantMap album_sub(QVariantMap query);

    // 已收藏专辑列表
    Q_INVOKABLE const QVariantMap album_sublist(QVariantMap query);

    // 专辑内容
    Q_INVOKABLE const QVariantMap album(QVariantMap query);

    // 歌手专辑列表
    Q_INVOKABLE const QVariantMap artist_album(QVariantMap query);

    // 歌手介绍
    Q_INVOKABLE const QVariantMap artist_desc(QVariantMap query);

    // 歌手详情
    Q_INVOKABLE const QVariantMap artist_detail(QVariantMap query);

    // 歌手粉丝
    Q_INVOKABLE const QVariantMap artist_fans(QVariantMap query);

    // 歌手粉丝数量
    Q_INVOKABLE const QVariantMap artist_follow_count(QVariantMap query);

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
    Q_INVOKABLE const QVariantMap artist_list(QVariantMap query);

    // 歌手相关MV
    Q_INVOKABLE const QVariantMap artist_mv(QVariantMap query);

    // 关注歌手新 MV
    Q_INVOKABLE const QVariantMap artist_new_mv(QVariantMap query);

    // 关注歌手新歌
    Q_INVOKABLE const QVariantMap artist_new_song(QVariantMap query);

    // 歌手全部歌曲
    Q_INVOKABLE const QVariantMap artist_songs(QVariantMap query);

    // 收藏与取消收藏歌手
    Q_INVOKABLE const QVariantMap artist_sub(QVariantMap query);

    // 关注歌手列表
    Q_INVOKABLE const QVariantMap artist_sublist(QVariantMap query);

    // 歌手热门 50 首歌曲
    Q_INVOKABLE const QVariantMap artist_top_song(QVariantMap query);

    // 歌手相关视频
    Q_INVOKABLE const QVariantMap artist_video(QVariantMap query);

    // 歌手单曲
    Q_INVOKABLE const QVariantMap artists(QVariantMap query);

    // 首页轮播图
    Q_INVOKABLE const QVariantMap banner(QVariantMap query);

    // 搜索
    Q_INVOKABLE const QVariantMap cloudsearch(QVariantMap query);

    // 专辑评论
    Q_INVOKABLE const QVariantMap comment_album(QVariantMap query);

    // 电台评论
    Q_INVOKABLE const QVariantMap comment_dj(QVariantMap query);

    // 获取动态评论
    Q_INVOKABLE const QVariantMap comment_event(QVariantMap query);

    // 歌曲评论
    Q_INVOKABLE const QVariantMap comment_music(QVariantMap query);

    // 国家编码列表
    Q_INVOKABLE const QVariantMap countries_code_list(QVariantMap query);

    // 手机登录
    Q_INVOKABLE const QVariantMap login_cellphone(QVariantMap query);

    // 退出登录
    Q_INVOKABLE const QVariantMap logout(QVariantMap query);

    // 新版歌词 - 包含逐字歌词
    Q_INVOKABLE const QVariantMap lyric_new(QVariantMap query);

    // 歌词
    Q_INVOKABLE const QVariantMap lyric(QVariantMap query);

    // 重复昵称检测
    Q_INVOKABLE const QVariantMap nickname_check(QVariantMap query);

    // 游客登录
    Q_INVOKABLE const QVariantMap register_anonimous(QVariantMap query);

    // 相关歌单
    Q_INVOKABLE const QVariantMap related_playlist(QVariantMap query);

    // 搜索
    Q_INVOKABLE const QVariantMap search(QVariantMap query);

    // 获取客户端歌曲下载链接
    Q_INVOKABLE const QVariantMap song_download_url(QVariantMap query);

    // 歌曲链接 - v1
    // 此版本不再采用 br 作为音质区分的标准
    // 而是采用 standard, exhigh, lossless, hires, jyeffect(高清环绕声), sky(沉浸环绕声), jymaster(超清母带) 进行音质判断
    Q_INVOKABLE const QVariantMap song_url_v1(QVariantMap query);

    // 音乐百科基础信息
    Q_INVOKABLE const QVariantMap song_wiki_summary(QVariantMap query);

    // 所有榜单介绍
    Q_INVOKABLE const QVariantMap toplist(QVariantMap query);

    // 获取账号信息
    Q_INVOKABLE const QVariantMap user_account(QVariantMap query);

    // 获取用户历史评论
    Q_INVOKABLE const QVariantMap user_comment_history(QVariantMap query);

    // 用户详情
    Q_INVOKABLE const QVariantMap user_detail(QVariantMap query);

    // 用户歌单
    Q_INVOKABLE const QVariantMap user_playlist(QVariantMap query);
};
#endif // MODULE_H
