#ifndef INDEX_H
#define INDEX_H
#include <QNetworkCookie>
#include <QVariantMap>

namespace Index{
QVariantMap stringToMap(const QString &cookie) {
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
QString cookieToString(const QList<QNetworkCookie> &cookie) {
    QString string("");
    for (const QNetworkCookie &i : cookie) {
        string += i.name() + "=" + i.value() + ";";
    }
    return string;
}
QList<QNetworkCookie> stringToCookie(const QString &cookie) {
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
QList<QNetworkCookie> mapToCookie(QVariantMap &cookie) {
    QList<QNetworkCookie> list;
    for(QMap<QString, QVariant>::iterator i = cookie.begin(); i != cookie.end(); ++i) {
        if(!i.value().isValid()) continue;
        list.append(QNetworkCookie(i.key().toUtf8(), i.value().toByteArray()));
    }
    return list;
}
}
#endif // INDEX_H
