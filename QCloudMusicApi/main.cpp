#include <QCoreApplication>
#include <QJsonDocument>
#include <QNetworkCookie>
#include <QDebug>

#include "module.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    NeteaseCloudMusicApi api;
    QByteArray ret;
//    qDebug().noquote() << QString::fromUtf8(api.song_url_v1({
//        { "id", QVariant::fromValue(QList({2058263032, 2057797340})) },
//        { "level", "exhigh" },
//        { "cookie", QVariantMap({
//                          { "NMTID", "00Otg9F5P1zqloq2kW4hreMZ5JO2BcAAAGJtqKtJg" }
//                      })
//        }
//    }));
//    qDebug().noquote() << QString::fromUtf8(api.lyric({
//        { "id", 2058263032 }
//    }));
//    qDebug().noquote() << QString::fromUtf8(api.related_playlist({
//        { "id", 2057797340 }
//    }));
//
    QMetaObject::invokeMethod(&api, "register_anonimous", Qt::DirectConnection, Q_RETURN_ARG(QByteArray, ret), Q_ARG(QVariantMap, QVariantMap(/*{
        { "id", 1969230350 }
        }*/)));
//    qDebug().noquote() << QString::fromUtf8(api.album({
//        { "id", 149114512 },
//        { "cookie", QVariantMap({
//                                   { "NMTID", "00O2DrG34TkFlEnMEmCrModcwIfNuIAAAGJwN1pPg" },
//                                   { "__csrf", "a00202686d9f5bdd8d9da2a400e4f76c" },
//                                   })
//        }
//    }));
//    qDebug().noquote() << QString::fromUtf8(api.comment_music({
//        { "id", 1969230350 }
//    }));
//    qDebug().noquote() << QString::fromUtf8(api.artists({
//        { "id", 32238754 },
//        { "cookie", QVariantMap({
//                                   { "NMTID", "00O2DrG34TkFlEnMEmCrModcwIfNuIAAAGJwN1pPg" },
//                                   { "__csrf", "a00202686d9f5bdd8d9da2a400e4f76c" },
//                                   })
//        }
//    }));
//    qDebug().noquote() << QString::fromUtf8(api.toplist({ }));
//    qDebug().noquote() << QString::fromUtf8(api.user_detail({
//        { "uid", 257800231 }
//    }));
//    qDebug().noquote() << QString::fromUtf8(api.artist_detail({
//        { "id", 32238754 },
//        { "cookie", QVariantMap({
//                          { "NMTID", "0e6eb2fc499a8ebd9e8d24b3cc4901e3" },
//                          { "__csrf", "a00202686d9f5bdd8d9da2a400e4f76c" },
//                          { "__remember_me", true },
//                          { "_ntes_nuid", "6ab91b90ba313ce626c8b2705c594407" },
//                          { "cook", "cook" },
//                          { "iris.language", "zh" },
//                          { "MUSIC_A", "bf8bfeabb1aa84f9c8c3906c04a04fb864322804c83f5d607e91a04eae463c9436bd1a17ec353cf780b396507a3f7464e8a60f4bbc019437993166e004087dd32d1490298caf655c2353e58daa0bc13cc7d5c198250968580b12c1b8817e3f5c807e650dd04abd3fb8130b7ae43fcc5b" }
//                      })
//        },
//        {"realIP", "::1"}
//    }));
    qDebug().noquote() << QString::fromUtf8(ret);
    qDebug().noquote() << "End";
    return a.exec();
}
