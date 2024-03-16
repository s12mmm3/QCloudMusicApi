#ifndef CRYPTO_H
#define CRYPTO_H

#include "../global.h"

#include <QByteArray>
#include <QVariantMap>

namespace QCloudMusicApiProject {
class QCLOUDMUSICAPI_EXPORT Crypto {
public:
    inline const QString static iv = QStringLiteral("0102030405060708");
    inline const QString static presetKey = QStringLiteral("0CoJUm6Qyw8W8jud");
    inline const QString static linuxapiKey = QStringLiteral("rFgB&h#%2?^eDg:Q");
    inline const QString static base62 = QStringLiteral("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
    inline const QString static publicKey = QStringLiteral(
        "-----BEGIN PUBLIC KEY-----\n"
        "MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQDgtQn2JZ34ZC28NWYpAUd98iZ37BUrX/aKzmFbt7clFSs6sXqHauqKWqdtLkF2KexO40H1YTX8z2lSgBBOAxLsvaklV8k4cBFK9snQXE9/DDaFt6Rr7iVZMldczhC0JNgTz+SHXT6CBHuX3e9SdB1Ua44oncaTWz7OBGLbCiK45wIDAQAB"
        "\n-----END PUBLIC KEY-----"
        );
    inline const QString static eapiKey = QStringLiteral("e82ckenh8dichen8");

    static QByteArray aesEncrypt (const QByteArray &plainText, const QString mode, const QByteArray &key, const QByteArray &iv, QString format = "base64");

    static QByteArray aesDecrypt(const QByteArray &cipherText, const QString mode, const QByteArray &key, const QByteArray &iv);

    static QByteArray rsaEncrypt (QString plainText, const QString& strPubKey);

    static QVariantMap weapi(QJsonDocument object);

    static QVariantMap linuxapi(QJsonDocument object);

    static QVariantMap eapi(QString url, QJsonDocument object);

    static QByteArray decrypt(QByteArray cipherBuffer);

};
}
#endif // CRYPTO_H
