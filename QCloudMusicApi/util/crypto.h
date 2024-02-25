#ifndef CRYPTO_H
#define CRYPTO_H

#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) \
|| defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#define Q_DECL_EXPORT __declspec(dllexport)
#define Q_DECL_IMPORT __declspec(dllimport)
#else
#define Q_DECL_EXPORT __attribute__((visibility("default")))
#define Q_DECL_IMPORT __attribute__((visibility("default")))
#endif

#if defined(BUILD_SHARED_LIBS)
#if defined(QCLOUDMUSICAPI_LIBRARY)
#  define QCLOUDMUSICAPI_EXPORT Q_DECL_EXPORT
#else
#  define QCLOUDMUSICAPI_EXPORT Q_DECL_IMPORT
#endif
#else
#  define QCLOUDMUSICAPI_EXPORT
#endif

#include <QByteArray>
#include <QVariantMap>

extern "C" {
#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
}

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

    static QByteArray aesEncrypt (const QByteArray &plainText, const EVP_CIPHER *mode(), const QByteArray &key, const QByteArray &iv, QString format = "base64");

    static QByteArray aesDecrypt(const QByteArray &cipherText, const EVP_CIPHER *mode(), const QByteArray &key, const QByteArray &iv);

    static QByteArray rsaEncrypt (QString plainText, const QString& strPubKey);

    static QVariantMap weapi(QJsonDocument object);

    static QVariantMap linuxapi(QJsonDocument object);

    static QVariantMap eapi(QString url, QJsonDocument object);

    static QByteArray decrypt(QByteArray cipherBuffer);

};
#endif // CRYPTO_H
