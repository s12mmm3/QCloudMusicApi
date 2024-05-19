#include "crypto.h"

#include <QByteArray>
#include <QObject>
#include <QVariantMap>
#include <QJsonDocument>
#include <QRandomGenerator>
#include <QCryptographicHash>
#include <QRegularExpression>

extern "C" {
#include "openssl/err.h"
#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
}

using namespace QCloudMusicApi;

const QString Crypto::iv = QStringLiteral("0102030405060708");
const QString Crypto::presetKey = QStringLiteral("0CoJUm6Qyw8W8jud");
const QString Crypto::linuxapiKey = QStringLiteral("rFgB&h#%2?^eDg:Q");
const QString Crypto::base62 = QStringLiteral("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
const QString Crypto::publicKey = QStringLiteral(
    "-----BEGIN PUBLIC KEY-----\n"
    "MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQDgtQn2JZ34ZC28NWYpAUd98iZ37BUrX/aKzmFbt7clFSs6sXqHauqKWqdtLkF2KexO40H1YTX8z2lSgBBOAxLsvaklV8k4cBFK9snQXE9/DDaFt6Rr7iVZMldczhC0JNgTz+SHXT6CBHuX3e9SdB1Ua44oncaTWz7OBGLbCiK45wIDAQAB"
    "\n-----END PUBLIC KEY-----"
    );

const QString Crypto::eapiKey = QStringLiteral("e82ckenh8dichen8");
/**
 * @brief 使用AES算法加密数据的函数
 * @param plainText 明文数据
 * @param mode 加密算法
 * @param key 加密密钥
 * @param iv 偏移量
 * @param format 输出格式
 * @return QString 密文数据，如果加密失败则为空字符串
 */
QByteArray Crypto::aesEncrypt (const QByteArray &plainText, const QString mode, const QByteArray &key, const QByteArray &iv, QString format) {
    auto cipher = (mode == "cbc") ? EVP_aes_128_cbc : /*ecb*/ EVP_aes_128_ecb;
    EVP_CIPHER_CTX *ctx;
    int len;
    unsigned char *ciphertext = new unsigned char[plainText.size() * 10];
    int ciphertext_len;
    if (! (ctx = EVP_CIPHER_CTX_new ())) ERR_print_errors_fp (stderr);
    if (1 != EVP_EncryptInit_ex (ctx, cipher (), NULL,
                                (unsigned char *)key.constData(),
                                (unsigned char *)iv.constData()))
        ERR_print_errors_fp (stderr);
    if (1 != EVP_EncryptUpdate (ctx, ciphertext, &len, (unsigned char *)plainText.constData(), plainText.size()))
        ERR_print_errors_fp (stderr);
    ciphertext_len = len;
    if (1 != EVP_EncryptFinal_ex (ctx, ciphertext + len, &len)) ERR_print_errors_fp (stderr);
    ciphertext_len += len;

    EVP_CIPHER_CTX_free (ctx);

    auto result = QByteArray((char *)ciphertext, ciphertext_len);
    delete[] ciphertext;
    if (format == "base64") {
        return result.toBase64();
    }
    return result.toHex().toUpper();
}

/**
 * @brief 使用AES算法解密数据的函数
 * @param cipherText 密文数据
 * @param mode 解密算法
 * @param key 解密密钥
 * @param iv 偏移量
 * @return QString 明文数据，如果解密失败则为空字符串
 */
QByteArray Crypto::aesDecrypt(const QByteArray &cipherText, const QString mode, const QByteArray &key, const QByteArray &iv, QString format)
{
    auto cipher = (mode == "cbc") ? EVP_aes_128_cbc : /*ecb*/ EVP_aes_128_ecb;
    EVP_CIPHER_CTX *ctx;

    int len;
    auto cipherText_p = format == "base64" ? QByteArray::fromBase64(cipherText) : QByteArray::fromHex(cipherText);
    unsigned char *plainText = new unsigned char[cipherText_p.size()];
    int plaintext_len;

    if (! (ctx = EVP_CIPHER_CTX_new ())) ERR_print_errors_fp (stderr);

    if (1 != EVP_DecryptInit_ex (ctx, cipher (), NULL,
                                (unsigned char *)key.constData(),
                                (unsigned char *)iv.constData()))
        ERR_print_errors_fp (stderr);

    if (1 != EVP_DecryptUpdate (ctx, plainText, &len, (unsigned char *)cipherText_p.constData(), cipherText_p.size()))
        ERR_print_errors_fp (stderr);
    plaintext_len = len;

    if (1 != EVP_DecryptFinal_ex (ctx, plainText + len, &len)) ERR_print_errors_fp (stderr);
    plaintext_len += len;

    EVP_CIPHER_CTX_free (ctx);

    auto result = QByteArray((char *)plainText, plaintext_len);
    delete[] plainText;
    return result;
}

/**
 * @brief rsaEncrypt 公钥加密
 * @param plainText 明文
 * @param strPubKey 公钥
 * @return 加密后数据(Hex格式)
 */
QByteArray Crypto::rsaEncrypt (QString plainText, const QString& strPubKey)
{
    QByteArray encryptData;
    QByteArray pubKeyArry = strPubKey.toUtf8();
    uchar* pPubKey = (uchar*)pubKeyArry.data();
    BIO* pKeyBio = BIO_new_mem_buf(pPubKey, pubKeyArry.length());
    if (pKeyBio == NULL){
        return "";
    }
    RSA* pRsa = RSA_new();
    if ( strPubKey.contains("BEGIN RSA PUBLIC KEY") ){
        pRsa = PEM_read_bio_RSAPublicKey(pKeyBio, &pRsa, NULL, NULL);
    }else{
        pRsa = PEM_read_bio_RSA_PUBKEY(pKeyBio, &pRsa, NULL, NULL);
    }
    if ( pRsa == NULL ){
        BIO_free_all(pKeyBio);
        return "";
    }

    int nLen = RSA_size(pRsa);
    char* pEncryptBuf = new char[nLen];


    if (plainText.length() < 128) {
        // 如果小于128，就用0填充空位，直到长度为128
        plainText.prepend(QString().fill(QChar(), 128 - plainText.length()));
    }
    QByteArray plainData = plainText.toUtf8();
    int nClearDataLen = plainData.length();
    uchar* pClearData = (uchar*)plainData.data();

    int nSize = RSA_public_encrypt(nClearDataLen,
                                   pClearData,
                                   (unsigned char*)pEncryptBuf,
                                   pRsa,
                                   RSA_NO_PADDING);

    if (nSize >= 0) {
        QByteArray arry((char*)pEncryptBuf, nSize);
        encryptData.append(arry);
    }

    // 释放内存
    delete[] pEncryptBuf;
    BIO_free_all(pKeyBio);
    RSA_free(pRsa);
    return encryptData;
}

QVariantMap Crypto::weapi(QJsonDocument object) {
    const QString text = object.toJson(QJsonDocument::Compact);

    // 创建一个长度为16的字节数组
    QByteArray secretKey;
    secretKey.resize(16);

    for (int i = 0; i < secretKey.size(); i++) {
        quint8 byte = QRandomGenerator::global()->generate() & 0xFF;
        int index = byte % base62.length();
        secretKey[i] = base62.at(index).toLatin1();
    }

    auto params = aesEncrypt(aesEncrypt(text.toUtf8(), "cbc", presetKey.toUtf8().data(), iv.toUtf8().data(), "base64"),
                             "cbc", secretKey.data(), iv.toUtf8().data(), "base64");
    std::reverse(secretKey.begin(), secretKey.end());
    auto encSecKey = rsaEncrypt(secretKey, publicKey).toHex();

    return {
        { QStringLiteral("params"), params },
        { QStringLiteral("encSecKey"), encSecKey }
    };
}

QVariantMap Crypto::linuxapi(QJsonDocument object) {
    const QString text = object.toJson(QJsonDocument::Indented);
    return {
        { QStringLiteral("eparams"), aesEncrypt(text.toUtf8(), "ecb", linuxapiKey.toUtf8().data(), QStringLiteral("").toUtf8().data(), "hex") }
    };
}

QVariantMap Crypto::eapi(QString url, QJsonDocument object) {
    const QString text = object.toJson(QJsonDocument::Indented);
    const QString message = QStringLiteral("nobody")
                            + url
                            + QStringLiteral("use")
                            + text
                            + QStringLiteral("md5forencrypt");
    const QByteArray digest = QCryptographicHash::hash(message.toUtf8(), QCryptographicHash::Md5).toHex();
    const QString data = url
                         + QStringLiteral("-36cd479b6b5-")
                         + text
                         + QStringLiteral("-36cd479b6b5-")
                         + digest;
    return {
        { "params", aesEncrypt(data.toUtf8(), "ecb", eapiKey.toUtf8().data(), QStringLiteral("").toUtf8().data(), "hex") }
    };
}

QVariantMap Crypto::eapiResDecrypt(const QByteArray &encryptedParams) {
    // 使用aesDecrypt解密参数
    auto decryptedData = aesDecrypt(encryptedParams, "ecb", eapiKey.toUtf8(), "", "hex");
    return QJsonDocument::fromJson(decryptedData).toVariant().toMap();
}

QVariantMap Crypto::eapiReqDecrypt(const QByteArray &encryptedParams) {
    // 使用aesDecrypt解密参数
    auto decryptedData = aesDecrypt(encryptedParams, "ecb", eapiKey.toUtf8(), "", "hex");
    // 使用正则表达式解析出URL和数据
    QRegularExpressionMatch match = QRegularExpression("(.*?)-36cd479b6b5-(.*?)-36cd479b6b5-(.*)").match(decryptedData);
    if (match.hasMatch()) {
        const auto url = match.captured(1);
        const auto data = QJsonDocument::fromJson(match.captured(2).toUtf8()).toVariant().toMap();
        return {
            { "url", url },
            { "data", data }
        };
    }

    // 如果没有匹配到，返回null
    return {};
}

QByteArray Crypto::decrypt(QByteArray cipherBuffer) {
    return aesDecrypt(cipherBuffer, "ecb", eapiKey.toUtf8().data(), "");
}
