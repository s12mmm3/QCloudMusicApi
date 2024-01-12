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

//入参与返回值类型为QVariantMap
#define APIH(FUNCNAME) \
Q_INVOKABLE const QVariantMap FUNCNAME(QVariantMap);

class QCLOUDMUSICAPI_EXPORT NeteaseCloudMusicApi: public QObject {
    Q_OBJECT

public:

    // 初始化名字
    APIH(activate_init_profile)

    // 专辑动态信息
    APIH(album_detail_dynamic)

    // 数字专辑详情
    APIH(album_detail)

    // 数字专辑-语种风格馆
    APIH(album_list_style)

    // 数字专辑-新碟上架
    APIH(album_list)

    // 全部新碟
    APIH(album_new)

    // 最新专辑
    APIH(album_newest)

    // 数字专辑&数字单曲-榜单
    APIH(album_songsaleboard)

    // 收藏/取消收藏专辑
    APIH(album_sub)

    // 已收藏专辑列表
    APIH(album_sublist)

    // 专辑内容
    APIH(album)

    // 歌手专辑列表
    APIH(artist_album)

    // 歌手介绍
    APIH(artist_desc)

    // 歌手详情
    APIH(artist_detail)

    // 歌手粉丝
    APIH(artist_fans)

    // 歌手粉丝数量
    APIH(artist_follow_count)

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
    APIH(artist_list)

    // 歌手相关MV
    APIH(artist_mv)

    // 关注歌手新 MV
    APIH(artist_new_mv)

    // 关注歌手新歌
    APIH(artist_new_song)

    // 歌手全部歌曲
    APIH(artist_songs)

    // 收藏与取消收藏歌手
    APIH(artist_sub)

    // 关注歌手列表
    APIH(artist_sublist)

    // 歌手热门 50 首歌曲
    APIH(artist_top_song)

    // 歌手相关视频
    APIH(artist_video)

    // 歌手单曲
    APIH(artists)

    // 批量请求接口
    APIH(batch)

    // 首页轮播图
    APIH(banner)

    // 搜索
    APIH(cloudsearch)

    // 专辑评论
    APIH(comment_album)

    // 电台评论
    APIH(comment_dj)

    // 获取动态评论
    APIH(comment_event)

    // 歌曲评论
    APIH(comment_music)

    // MV评论
    APIH(comment_mv)

    // 歌单评论
    APIH(comment_playlist)

    // 视频评论
    APIH(comment_video)

    // 国家编码列表
    APIH(countries_code_list)

    // 获取达人用户信息
    APIH(creator_authinfo_get)

    // 签到

    /*
    0为安卓端签到 3点经验, 1为网页签到,2点经验
    签到成功 {'android': {'point': 3, 'code': 200}, 'web': {'point': 2, 'code': 200}}
    重复签到 {'android': {'code': -2, 'msg': '重复签到'}, 'web': {'code': -2, 'msg': '重复签到'}}
    未登录 {'android': {'code': 301}, 'web': {'code': 301}}
    */
    APIH(daily_signin)

    // 数字专辑详情
    APIH(digitalAlbum_detail)

    // 购买数字专辑
    APIH(digitalAlbum_ordering)

    // 我的数字专辑
    APIH(digitalAlbum_purchased)

    // 数字专辑销量
    APIH(digitalAlbum_sales)

    // 电台banner
    APIH(dj_banner)

    // 电台非热门类型
    APIH(dj_category_excludehot)

    // 电台推荐类型
    APIH(dj_category_recommend)

    // 电台分类列表
    APIH(dj_catelist)

    // 电台详情
    APIH(dj_detail)

    // 热门电台
    APIH(dj_hot)

    // 付费电台
    APIH(dj_paygift)

    // 电台个性推荐
    APIH(dj_personalize_recommend)

    // 电台节目详情
    APIH(dj_program_detail)

    // 电台24小时节目榜
    APIH(dj_program_toplist_hours)

    // 电台节目榜
    APIH(dj_program_toplist)

    // 电台节目列表
    APIH(dj_program)

    // 类别热门电台
    APIH(dj_radio_hot)

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
    APIH(dj_recommend_type)

    // 精选电台
    APIH(dj_recommend)

    // 订阅与取消电台
    APIH(dj_sub)

    // 订阅电台列表
    APIH(dj_sublist)

    // 电台详情
    APIH(dj_subscriber)

    // 电台今日优选
    APIH(dj_today_perfered)

    // 电台24小时主播榜
    APIH(dj_toplist_hours)

    // 电台新人榜
    APIH(dj_toplist_newcomer)

    // 付费精品
    APIH(dj_toplist_pay)

    // 电台最热主播榜
    APIH(dj_toplist_popular)

    // 新晋电台榜/热门电台榜
    APIH(dj_toplist)

    // 删除动态
    APIH(event_del)

    // 转发动态
    APIH(event_forward)

    // 动态
    APIH(event)

    // 垃圾桶
    APIH(fm_trash)

    // 关注与取消关注用户
    APIH(follow)

    // 粉丝年龄比例
    APIH(fanscenter_basicinfo_age_get)

    // 粉丝性别比例
    APIH(fanscenter_basicinfo_gender_get)

    // 粉丝省份比例
    APIH(fanscenter_basicinfo_province_get)

    // 粉丝数量
    APIH(fanscenter_overview_get)

    // 粉丝来源
    APIH(fanscenter_trend_list)

    // 手机登录
    APIH(login_cellphone)

    // 二维码检测扫码状态接口
    APIH(login_qr_check)

    // 二维码生成接口
    APIH(login_qr_create)

    // 二维码 key 生成接口
    APIH(login_qr_key)

    // 登录刷新
    APIH(login_refresh)

    // 登录状态
    APIH(login_status)

    // 退出登录
    APIH(logout)

    // 新版歌词 - 包含逐字歌词
    APIH(lyric_new)

    // 歌词
    APIH(lyric)

    // 重复昵称检测
    APIH(nickname_check)

    // 游客登录
    APIH(register_anonimous)

    // 相关歌单
    APIH(related_playlist)

    // 搜索
    APIH(search)

    // 获取客户端歌曲下载链接
    APIH(song_download_url)

    // 歌曲链接 - v1
    // 此版本不再采用 br 作为音质区分的标准
    // 而是采用 standard, exhigh, lossless, hires, jyeffect(高清环绕声), sky(沉浸环绕声), jymaster(超清母带) 进行音质判断
    APIH(song_url_v1)

    // 音乐百科基础信息
    APIH(song_wiki_summary)

    // 年度听歌报告2017-2022
    APIH(summary_annual)

    // 获取达人达标信息
    APIH(threshold_detail_get)

    // 所有榜单介绍
    APIH(toplist)

    // 专辑简要百科信息
    APIH(ugc_album_get)

    // 歌手简要百科信息
    APIH(ugc_artist_get)

    // 搜索歌手
    // 可传关键字或者歌手id
    APIH(ugc_artist_search)

    // mv简要百科信息
    APIH(ugc_mv_get)

    // 歌曲简要百科信息
    APIH(ugc_song_get)

    // 获取账号信息
    APIH(user_account)

    // 获取用户历史评论
    APIH(user_comment_history)

    // 用户详情
    APIH(user_detail)

    // 用户歌单
    APIH(user_playlist)

    // 云贝 todo 任务
    APIH(yunbei_tasks_todo)

    // 云贝所有任务
    APIH(yunbei_tasks)

    // 云贝今日签到信息
    APIH(yunbei_today)

    // 云贝
    APIH(yunbei)
};
#endif // MODULE_H
