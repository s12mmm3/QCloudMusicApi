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

    QVariantMap paramInject(QVariantMap params);
    // 专辑动态信息
    Q_INVOKABLE const QByteArray album_detail_dynamic(QVariantMap query);

    // 数字专辑详情
    Q_INVOKABLE const QByteArray album_detail(QVariantMap query);

    // 数字专辑-语种风格馆
    Q_INVOKABLE const QByteArray album_list_style(QVariantMap query);

    // 数字专辑-新碟上架
    Q_INVOKABLE const QByteArray album_list(QVariantMap query);

    // 全部新碟
    Q_INVOKABLE const QByteArray album_new(QVariantMap query);

    // 最新专辑
    Q_INVOKABLE const QByteArray album_newest(QVariantMap query);

    // 专辑内容
    Q_INVOKABLE const QByteArray album(QVariantMap query);

    // 歌手专辑列表
    Q_INVOKABLE const QByteArray artist_album(QVariantMap query);

    // 歌手热门 50 首歌曲
    Q_INVOKABLE const QByteArray artist_top_song(QVariantMap query);

    // 歌手单曲
    Q_INVOKABLE const QByteArray artists(QVariantMap query);

    Q_INVOKABLE const QByteArray artist_detail(QVariantMap query);

    // 国家编码列表
    Q_INVOKABLE const QByteArray countries_code_list(QVariantMap query);

    // 获取客户端歌曲下载链接
    Q_INVOKABLE const QByteArray song_download_url(QVariantMap query);

    // 歌曲链接 - v1
    // 此版本不再采用 br 作为音质区分的标准
    // 而是采用 standard, exhigh, lossless, hires, jyeffect(高清环绕声), sky(沉浸环绕声), jymaster(超清母带) 进行音质判断
    Q_INVOKABLE const QByteArray song_url_v1(QVariantMap query);

    // 音乐百科基础信息
    Q_INVOKABLE const QByteArray song_wiki_summary(QVariantMap query);

    // 手机登录
    Q_INVOKABLE const QByteArray login_cellphone(QVariantMap query);

    // 歌词
    Q_INVOKABLE const QByteArray lyric(QVariantMap query);

    // 新版歌词 - 包含逐字歌词
    Q_INVOKABLE const QByteArray lyric_new(QVariantMap query);

    // 相关歌单
    Q_INVOKABLE const QByteArray related_playlist(QVariantMap query);

    // 搜索
    Q_INVOKABLE const QByteArray cloudsearch(QVariantMap query);

    // 歌曲评论
    Q_INVOKABLE const QByteArray comment_music(QVariantMap query);

    // 所有榜单介绍
    Q_INVOKABLE const QByteArray toplist(QVariantMap query);

    // 用户详情
    Q_INVOKABLE const QByteArray user_detail(QVariantMap query);

    // 用户歌单
    Q_INVOKABLE const QByteArray user_playlist(QVariantMap query);

    // 游客登录
    Q_INVOKABLE const QByteArray register_anonimous(QVariantMap query);

    // 搜索
    Q_INVOKABLE const QByteArray search(QVariantMap query);
};
#endif // MODULE_H
