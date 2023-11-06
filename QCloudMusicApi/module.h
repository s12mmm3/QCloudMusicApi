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

    // 初始化名字
    Q_INVOKABLE const QVariantMap activate_init_profile(QVariantMap);

    // 专辑动态信息
    Q_INVOKABLE const QVariantMap album_detail_dynamic(QVariantMap);

    // 数字专辑详情
    Q_INVOKABLE const QVariantMap album_detail(QVariantMap);

    // 数字专辑-语种风格馆
    Q_INVOKABLE const QVariantMap album_list_style(QVariantMap);

    // 数字专辑-新碟上架
    Q_INVOKABLE const QVariantMap album_list(QVariantMap);

    // 全部新碟
    Q_INVOKABLE const QVariantMap album_new(QVariantMap);

    // 最新专辑
    Q_INVOKABLE const QVariantMap album_newest(QVariantMap);

    // 数字专辑&数字单曲-榜单
    Q_INVOKABLE const QVariantMap album_songsaleboard(QVariantMap);

    // 收藏/取消收藏专辑
    Q_INVOKABLE const QVariantMap album_sub(QVariantMap);

    // 已收藏专辑列表
    Q_INVOKABLE const QVariantMap album_sublist(QVariantMap);

    // 专辑内容
    Q_INVOKABLE const QVariantMap album(QVariantMap);

    // 歌手专辑列表
    Q_INVOKABLE const QVariantMap artist_album(QVariantMap);

    // 歌手介绍
    Q_INVOKABLE const QVariantMap artist_desc(QVariantMap);

    // 歌手详情
    Q_INVOKABLE const QVariantMap artist_detail(QVariantMap);

    // 歌手粉丝
    Q_INVOKABLE const QVariantMap artist_fans(QVariantMap);

    // 歌手粉丝数量
    Q_INVOKABLE const QVariantMap artist_follow_count(QVariantMap);

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
    Q_INVOKABLE const QVariantMap artist_list(QVariantMap);

    // 歌手相关MV
    Q_INVOKABLE const QVariantMap artist_mv(QVariantMap);

    // 关注歌手新 MV
    Q_INVOKABLE const QVariantMap artist_new_mv(QVariantMap);

    // 关注歌手新歌
    Q_INVOKABLE const QVariantMap artist_new_song(QVariantMap);

    // 歌手全部歌曲
    Q_INVOKABLE const QVariantMap artist_songs(QVariantMap);

    // 收藏与取消收藏歌手
    Q_INVOKABLE const QVariantMap artist_sub(QVariantMap);

    // 关注歌手列表
    Q_INVOKABLE const QVariantMap artist_sublist(QVariantMap);

    // 歌手热门 50 首歌曲
    Q_INVOKABLE const QVariantMap artist_top_song(QVariantMap);

    // 歌手相关视频
    Q_INVOKABLE const QVariantMap artist_video(QVariantMap);

    // 歌手单曲
    Q_INVOKABLE const QVariantMap artists(QVariantMap);

    // 首页轮播图
    Q_INVOKABLE const QVariantMap banner(QVariantMap);

    // 搜索
    Q_INVOKABLE const QVariantMap cloudsearch(QVariantMap);

    // 专辑评论
    Q_INVOKABLE const QVariantMap comment_album(QVariantMap);

    // 电台评论
    Q_INVOKABLE const QVariantMap comment_dj(QVariantMap);

    // 获取动态评论
    Q_INVOKABLE const QVariantMap comment_event(QVariantMap);

    // 歌曲评论
    Q_INVOKABLE const QVariantMap comment_music(QVariantMap);

    // MV评论
    Q_INVOKABLE const QVariantMap comment_mv(QVariantMap);

    // 歌单评论
    Q_INVOKABLE const QVariantMap comment_playlist(QVariantMap);

    // 视频评论
    Q_INVOKABLE const QVariantMap comment_video(QVariantMap);

    // 国家编码列表
    Q_INVOKABLE const QVariantMap countries_code_list(QVariantMap);

    // 获取达人用户信息
    Q_INVOKABLE const QVariantMap creator_authinfo_get(QVariantMap);

    // 签到

    /*
    0为安卓端签到 3点经验, 1为网页签到,2点经验
    签到成功 {'android': {'point': 3, 'code': 200}, 'web': {'point': 2, 'code': 200}}
    重复签到 {'android': {'code': -2, 'msg': '重复签到'}, 'web': {'code': -2, 'msg': '重复签到'}}
    未登录 {'android': {'code': 301}, 'web': {'code': 301}}
    */
    Q_INVOKABLE const QVariantMap daily_signin(QVariantMap);

    // 数字专辑详情
    Q_INVOKABLE const QVariantMap digitalAlbum_detail(QVariantMap);

    // 购买数字专辑
    Q_INVOKABLE const QVariantMap digitalAlbum_ordering(QVariantMap);

    // 我的数字专辑
    Q_INVOKABLE const QVariantMap digitalAlbum_purchased(QVariantMap);

    // 数字专辑销量
    Q_INVOKABLE const QVariantMap digitalAlbum_sales(QVariantMap);

    // 电台banner
    Q_INVOKABLE const QVariantMap dj_banner(QVariantMap);

    // 电台非热门类型
    Q_INVOKABLE const QVariantMap dj_category_excludehot(QVariantMap);

    // 电台推荐类型
    Q_INVOKABLE const QVariantMap dj_category_recommend(QVariantMap);

    // 电台分类列表
    Q_INVOKABLE const QVariantMap dj_catelist(QVariantMap);

    // 电台详情
    Q_INVOKABLE const QVariantMap dj_detail(QVariantMap);

    // 热门电台
    Q_INVOKABLE const QVariantMap dj_hot(QVariantMap);

    // 付费电台
    Q_INVOKABLE const QVariantMap dj_paygift(QVariantMap);

    // 电台个性推荐
    Q_INVOKABLE const QVariantMap dj_personalize_recommend(QVariantMap);

    // 电台节目详情
    Q_INVOKABLE const QVariantMap dj_program_detail(QVariantMap);

    // 电台24小时节目榜
    Q_INVOKABLE const QVariantMap dj_program_toplist_hours(QVariantMap);

    // 电台节目榜
    Q_INVOKABLE const QVariantMap dj_program_toplist(QVariantMap);

    // 电台节目列表
    Q_INVOKABLE const QVariantMap dj_program(QVariantMap);

    // 类别热门电台
    Q_INVOKABLE const QVariantMap dj_radio_hot(QVariantMap);

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
    Q_INVOKABLE const QVariantMap dj_recommend_type(QVariantMap);

    // 精选电台
    Q_INVOKABLE const QVariantMap dj_recommend(QVariantMap);

    // 订阅与取消电台
    Q_INVOKABLE const QVariantMap dj_sub(QVariantMap);

    // 订阅电台列表
    Q_INVOKABLE const QVariantMap dj_sublist(QVariantMap);

    // 电台详情
    Q_INVOKABLE const QVariantMap dj_subscriber(QVariantMap);

    // 电台今日优选
    Q_INVOKABLE const QVariantMap dj_today_perfered(QVariantMap);

    // 电台24小时主播榜
    Q_INVOKABLE const QVariantMap dj_toplist_hours(QVariantMap);

    // 电台新人榜
    Q_INVOKABLE const QVariantMap dj_toplist_newcomer(QVariantMap);

    // 付费精品
    Q_INVOKABLE const QVariantMap dj_toplist_pay(QVariantMap);

    // 电台最热主播榜
    Q_INVOKABLE const QVariantMap dj_toplist_popular(QVariantMap);

    // 新晋电台榜/热门电台榜
    Q_INVOKABLE const QVariantMap dj_toplist(QVariantMap);

    // 粉丝年龄比例
    Q_INVOKABLE const QVariantMap fanscenter_basicinfo_age_get(QVariantMap);

    // 粉丝性别比例
    Q_INVOKABLE const QVariantMap fanscenter_basicinfo_gender_get(QVariantMap);

    // 粉丝省份比例
    Q_INVOKABLE const QVariantMap fanscenter_basicinfo_province_get(QVariantMap);

    // 粉丝数量
    Q_INVOKABLE const QVariantMap fanscenter_overview_get(QVariantMap);

    // 粉丝来源
    Q_INVOKABLE const QVariantMap fanscenter_trend_list(QVariantMap);

    // 手机登录
    Q_INVOKABLE const QVariantMap login_cellphone(QVariantMap);

    // 退出登录
    Q_INVOKABLE const QVariantMap logout(QVariantMap);

    // 新版歌词 - 包含逐字歌词
    Q_INVOKABLE const QVariantMap lyric_new(QVariantMap);

    // 歌词
    Q_INVOKABLE const QVariantMap lyric(QVariantMap);

    // 重复昵称检测
    Q_INVOKABLE const QVariantMap nickname_check(QVariantMap);

    // 游客登录
    Q_INVOKABLE const QVariantMap register_anonimous(QVariantMap);

    // 相关歌单
    Q_INVOKABLE const QVariantMap related_playlist(QVariantMap);

    // 搜索
    Q_INVOKABLE const QVariantMap search(QVariantMap);

    // 获取客户端歌曲下载链接
    Q_INVOKABLE const QVariantMap song_download_url(QVariantMap);

    // 歌曲链接 - v1
    // 此版本不再采用 br 作为音质区分的标准
    // 而是采用 standard, exhigh, lossless, hires, jyeffect(高清环绕声), sky(沉浸环绕声), jymaster(超清母带) 进行音质判断
    Q_INVOKABLE const QVariantMap song_url_v1(QVariantMap);

    // 音乐百科基础信息
    Q_INVOKABLE const QVariantMap song_wiki_summary(QVariantMap);

    // 年度听歌报告2017-2022
    Q_INVOKABLE const QVariantMap summary_annual(QVariantMap);

    // 获取达人达标信息
    Q_INVOKABLE const QVariantMap threshold_detail_get(QVariantMap);

    // 所有榜单介绍
    Q_INVOKABLE const QVariantMap toplist(QVariantMap);

    // 专辑简要百科信息
    Q_INVOKABLE const QVariantMap ugc_album_get(QVariantMap);

    // 歌手简要百科信息
    Q_INVOKABLE const QVariantMap ugc_artist_get(QVariantMap);

    // 搜索歌手
    // 可传关键字或者歌手id
    Q_INVOKABLE const QVariantMap ugc_artist_search(QVariantMap);

    // mv简要百科信息
    Q_INVOKABLE const QVariantMap ugc_mv_get(QVariantMap);

    // 歌曲简要百科信息
    Q_INVOKABLE const QVariantMap ugc_song_get(QVariantMap);

    // 获取账号信息
    Q_INVOKABLE const QVariantMap user_account(QVariantMap);

    // 获取用户历史评论
    Q_INVOKABLE const QVariantMap user_comment_history(QVariantMap);

    // 用户详情
    Q_INVOKABLE const QVariantMap user_detail(QVariantMap);

    // 用户歌单
    Q_INVOKABLE const QVariantMap user_playlist(QVariantMap);
};
#endif // MODULE_H
