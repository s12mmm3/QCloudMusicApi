#ifndef INDEX_H
#define INDEX_H
#include <QVariantMap>

namespace Index{
QVariantMap cookieToJson(const QString &cookie) {
    if (cookie.isEmpty()) return QVariantMap();
    QVariantMap obj;
    for (const QString &i : cookie.split(";")) {
        QStringList arr = i.trimmed().split("=");
        // 如果子串的个数不等于2，跳过这个元素
        if (arr.size() != 2) continue;
        obj.insert(arr[0], arr[1]);
    }
    return obj;
}
QVariantMap cookieListToJsonMap(const QStringList &cookieList) {
    if (cookieList.isEmpty()) return QVariantMap();
    QVariantMap obj;
    for(QString cookie: cookieList) {
        for (const QString &i : cookie.split(";")) {
            QStringList arr = i.trimmed().split("=");
            // 如果子串的个数不等于2，跳过这个元素
            if (arr.size() != 2) continue;
            obj.insert(arr[0], arr[1]);
        }
    }
    return obj;
}
QVariantMap cookieStrToJsonMap(const QString &cookie) {
    if (cookie.isEmpty()) return QVariantMap();
    QVariantMap obj;
    for (const QString &i : cookie.split(";")) {
        QStringList arr = i.trimmed().split("=");
        // 如果子串的个数不等于2，跳过这个元素
        if (arr.size() != 2) continue;
        obj.insert(arr[0], arr[1]);
    }
    return obj;
}
QStringList jsonMapToCookieList(const QVariantMap &obj) {
    if (obj.isEmpty()) return QStringList();
    QStringList cookieList;
    for (auto it = obj.begin(); it != obj.end(); ++it) {
        QString cookie = it.key() + "=" + it.value().toString();
        cookieList.append(cookie);
    }
    return cookieList;
}
QString jsonMapToCookieStr(const QVariantMap &obj) {
    if (obj.isEmpty()) return QString();
    QStringList cookieList;
    for (auto it = obj.begin(); it != obj.end(); ++it) {
        QString cookie = it.key() + "=" + it.value().toString();
        cookieList.append(cookie);
    }
    return cookieList.join("; ");
}
}
#endif // INDEX_H
