#include <QCoreApplication>
#include <QJsonDocument>
#include <QNetworkCookie>

#include "crypto.hpp"
#include "module.h"

extern "C" {
#include <openssl/rc4.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>

}

QString getLrc(QString source) {
    auto doc = QJsonDocument::fromJson(source.toUtf8());
    return doc["lrc"]["lyric"].toString();
}



int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QString buffer(
        R"(/api/song/enhance/player/url/v1-36cd479b6b5-{"ids":"[33894312]","level":"exhigh","encodeType":"flac","header":{"appver":"8.10.05","versioncode":"140","buildver":"1690971728","resolution":"1920x1080","__csrf":"","os":"android","requestId":"1690971728338_0442","MUSIC_A":"bf8bfeabb1aa84f9c8c3906c04a04fb864322804c83f5d607e91a04eae463c9436bd1a17ec353cf780b396507a3f7464e8a60f4bbc019437993166e004087dd32d1490298caf655c2353e58daa0bc13cc7d5c198250968580b12c1b8817e3f5c807e650dd04abd3fb8130b7ae43fcc5b"}}-36cd479b6b5-1cfd8ee98f8f6c8d3337b1dc7e9be596)"
        );
    QByteArray b = Crypto::aesEncrypt(buffer.toUtf8(), EVP_aes_128_ecb, Crypto::eapiKey, QString());
    qDebug().noquote() << b;
    qDebug().noquote() << (QString::fromUtf8(Crypto::aesDecrypt(b, EVP_aes_128_ecb, Crypto::eapiKey, QString())).compare(buffer) == 0);
    auto getPubKey = []() {
        return "-----BEGIN PUBLIC KEY-----\n"
               "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEA9TgbwZu9LtSRJlTI8eqzU3HVyzTSzM2rIGs4O9IuthVOTtuQDXMGq8V+y9+yxNE/6bBlLzRxDznulMAayzNlMyo/IXE+wfTlKKYfKv0gButyjvlKwd2ZAJyJnXcT7z2vi+z3feuSA9+d3NhELx/T3k/wAt3YD6Zb5ctmKhocp7uAQfI9LqdpXXV+DL99fTYr3+f8MPXXl6XZ6N987p6zY+IIRx8ETVG8AwCFHX0MC3w+Gx0Mzcrrd25/niYICRbRa6uL0Dp4a8KnEzcuAPpGxkclmCbEU65yYU92knhf6lkAN2IgJyxhXtFW/iT7A3wB4YfcqBdYskphW//WmWuIPQIDAQAB"
               "\n-----END PUBLIC KEY-----";
    };
    auto getPriKey = []() {
        return R"(-----BEGIN PRIVATE KEY-----
MIIEvQIBADANBgkqhkiG9w0BAQEFAASCBKcwggSjAgEAAoIBAQD1OBvBm70u1JEm
VMjx6rNTcdXLNNLMzasgazg70i62FU5O25ANcwarxX7L37LE0T/psGUvNHEPOe6U
wBrLM2UzKj8hcT7B9OUoph8q/SAG63KO+UrB3ZkAnImddxPvPa+L7Pd965ID353c
2EQvH9PeT/AC3dgPplvly2YqGhynu4BB8j0up2lddX4Mv319Nivf5/ww9deXpdno
33zunrNj4ghHHwRNUbwDAIUdfQwLfD4bHQzNyut3bn+eJggJFtFrq4vQOnhrwqcT
Ny4A+kbGRyWYJsRTrnJhT3aSeF/qWQA3YiAnLGFe0Vb+JPsDfAHhh9yoF1iySmFb
/9aZa4g9AgMBAAECggEAG9WdmPFjnQA15aL0GDmq10rxmJpLX0HtLcXd4TRvZu2u
MNdNV6l51V+aj+u5cAvMdGs4YQ5T0wWhZjMUF4Ki9xbRiatAE2YH6QBKQ2fuDbsG
CuKTMMRFjMM3/663jY1z8q8YPKvTrCri0WGfA7ZaJz7toHWsA8K/Zasb0qWaIRO3
jcwM5PQAUKBSTf6qHjElPZOdR00H3/IZSmmv6hSKHfis1p1TvEbn9ooogcvZLwIA
S7jkr0r7Lwm9ykebouHlCZMw2F/14XW7Cyq+FF8FwYChK8S3DyR+UgK9N4pp0GJb
ia+3+P1ZbZjfofWinyr9cIN5KYVrQhBQ4bZu+qjg5QKBgQD9rIGUKCiMDev1sqOF
fnVkHQyWjDjTqcmyFwEfODsWH7Bpyt/6WSlCgnUwJtrwrIonoP5ugA+aPMsI2reP
qAh43cD/rgN+ig+A4vzwcP6/q4TyNUA+MTqkp0EHB3/2Au75rQPgPD/RnB6J+q1i
Gmf2Dezp6uHuk0oZftHuucQXqwKBgQD3d8FNErdCME1zEt/7i2zdsSOiqTxfZnqw
bDZz4fDOFMeQZn3L65n+31k5gNMT363ezlJ/4D2FQt2poBwfTQJfdIu5eIehxPGX
g8F52boAJvZHeE83BgINThmct8Ez4dVUJAx+3pfSYuqCCGnYr1ilMGLmdCntmPEb
OqKCJRrXtwKBgQD5ucoylTodblMdJIhQEcb1EJfk2RPUY3B4zqa/Gjcot3ePHjZS
mVXB8PX2ujmM9v5mttni5FT3zOM/yzkrxgrZGc02pxMvjjBm/I5vEX3oKK0PWBbv
Yr1NfI3Korp48DhcONP4+rcE3cE+qjXnvwEbPGg/e4iBuiQXZz652czOUwKBgDiO
lA0LU/q6k1jTXYFd0hmwnFZWm0eqYivj72X4o1JWCW4FHob2Z5GKdhfgS11lZ5Ve
ThiPpT/JkmNsBkFZFku+lRhkZYshOZ3JLPSuQAY84VhgNKp8owdrbAsXK/6xhFHg
k0KDmOsqVDI3vgkZuga20kOdFOAKbkAs3GsmE8ZtAoGACf300XCIcph2qAG9ds1w
s+/FkAWuJSxK6lDiDjwzaQZ1dcE2b1S6LKLrrKlHkvw7UokKZGvGR6h/b+KTcIo/
ljS9jNeqxvJnvKpJZoBrqkI9y7PeNPR/N0rFrcSqAPYs70UnDecbPDGzNe8namyI
TA7YfAENuKJcEaTMMaLF9xM=
-----END PRIVATE KEY-----)";
    };
    QByteArray c = Crypto::rsaEncrypt(buffer.toUtf8(), getPubKey());
//    qDebug().noquote() << c;
//    qDebug().noquote() << (QString::fromUtf8(Crypto::rsaDecrypt(c, getPriKey())).compare(buffer) == 0);
//    qDebug().noquote() << Crypto::weapi(QJsonDocument());
//    qDebug().noquote() << Crypto::linuxapi(QJsonDocument::fromJson(QString(R"({"1": "1"})").toUtf8()));
//    qDebug().noquote() << Crypto::eapi("/api/song/enhance/player/url/v1", QJsonDocument::fromJson(QString(R"({"1": "1"})").toUtf8()));
//    qDebug().noquote() << QString::fromUtf8(Module::song_url_v1({
//        { "id", QVariant::fromValue(QList({2058263032, 2057797340})) },
//        { "level", "exhigh" },
//        { "cookie", QVariantMap({
//                          { "NMTID", "00Otg9F5P1zqloq2kW4hreMZ5JO2BcAAAGJtqKtJg" }
//                      })
//        }
//    }));
//    qDebug().noquote() << QString::fromUtf8(Module::lyric({
//        { "id", 2058263032 }
//    }));
    qDebug().noquote() << QString::fromUtf8(Module::related_playlist({
        { "id", 2057797340 }
    }));
    return a.exec();
}
