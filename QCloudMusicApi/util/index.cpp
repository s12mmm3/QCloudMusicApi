#include "index.h"

using namespace QCloudMusicApiNS;
QVariantMap Index::stringToMap(const QString &cookie) {
    if (cookie.isEmpty()) return QVariantMap();
    QVariantMap map;
    for (const QString &i : cookie.split(";")) {
        QStringList arr = i.trimmed().split("=");
        // 如果子串的个数不等于2，跳过这个元素
        if (arr.size() != 2) continue;
        map.insert(arr[0], arr[1]);
    }
    return map;
}
QString Index::mapToString(const QVariantMap &cookie) {
    QString string;
    for(auto i = cookie.begin(); i != cookie.end(); ++i) {
        string.append(i.key() + "=" + i.value().toString() + ";");
    }
    return string;
}
QString Index::cookieToString(const QList<QNetworkCookie> &cookie) {
    QString string;
    for (const QNetworkCookie &i : cookie) {
        string.append(i.toRawForm() + ";");
    }
    return string;
}
QList<QNetworkCookie> Index::stringToCookie(const QString &cookie) {
    if (cookie.isEmpty()) return QList<QNetworkCookie>();
    QList<QNetworkCookie> list;
    for (const QString &i : cookie.split(";")) {
        QStringList arr = i.trimmed().split("=");
        // 如果子串的个数不等于2，跳过这个元素
        if (arr.size() != 2) continue;
        list.append(QNetworkCookie(arr[0].toUtf8(), arr[1].toUtf8()));
    }
    return list;
}
QList<QNetworkCookie> Index::mapToCookie(QVariantMap &cookie) {
    QList<QNetworkCookie> list;
    for(QMap<QString, QVariant>::iterator i = cookie.begin(); i != cookie.end(); ++i) {
        if(!i.value().isValid()) continue;
        list.append(QNetworkCookie(i.key().toUtf8(), i.value().toByteArray()));
    }
    return list;
}
QVariantMap Index::mergeMap(const QVariantMap &map0, const QVariantMap &map1)
{
    QVariantMap result = map0; //复制第一个map
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
    for (auto i = map1.constBegin(); i != map1.constEnd(); ++i) {
        result.insert(i.key(), i.value()); //插入或覆盖第二个map的键值对
    }
#else
    result.insert(map1);
#endif
    return result;
}
