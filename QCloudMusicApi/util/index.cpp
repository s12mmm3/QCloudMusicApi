#include "index.h"

using namespace QCloudMusicApi;

bool Index::toBoolean(const QVariant &val) {
    if (val.userType() == QMetaType::Bool) return val.toBool();
    if (val == "") return val.toBool();
    return val == "true" || val == "1";
}

QVariantMap Index::cookieToJson(const QString &cookie) {
    if (cookie.isEmpty()) return QVariantMap();
    auto cookieArr = cookie.split(";");
    QVariantMap obj;
    for (const QString &i : cookieArr) {
        QStringList arr = i.trimmed().split("=");
        // 如果子串的个数不等于2，跳过这个元素
        if (arr.size() != 2) continue;
        obj.insert(arr[0], arr[1]);
    }
    return obj;
}

QString Index::cookieObjToString(const QVariantMap &cookie) {
    QString string;
    for(auto i = cookie.begin(); i != cookie.end(); ++i) {
        string.append(i.key() + "=" + i.value().toString() + "; ");
    }
    return string;
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
