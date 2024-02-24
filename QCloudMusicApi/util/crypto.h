#ifndef CRYPTO_H
#define CRYPTO_H

#include <QByteArray>
#include <QObject>
#include <QVariantMap>
#include <QJsonDocument>
#include <QRandomGenerator>
#include <QCryptographicHash>
#include <iostream>

extern "C" {
#include "openssl/err.h"
#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
}

namespace Crypto {

const static auto iv = QStringLiteral("0102030405060708");
const static auto presetKey = QStringLiteral("0CoJUm6Qyw8W8jud");
const static auto linuxapiKey = QStringLiteral("rFgB&h#%2?^eDg:Q");
const static auto base62 = QStringLiteral("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
const static auto publicKey = QStringLiteral(
    "-----BEGIN PUBLIC KEY-----\n"
    "MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQDgtQn2JZ34ZC28NWYpAUd98iZ37BUrX/aKzmFbt7clFSs6sXqHauqKWqdtLkF2KexO40H1YTX8z2lSgBBOAxLsvaklV8k4cBFK9snQXE9/DDaFt6Rr7iVZMldczhC0JNgTz+SHXT6CBHuX3e9SdB1Ua44oncaTWz7OBGLbCiK45wIDAQAB"
    "\n-----END PUBLIC KEY-----"
    );
const static auto eapiKey = QStringLiteral("e82ckenh8dichen8");

/**
 * @brief 使用AES算法加密数据的函数
 * @param plainData 明文数据
 * @param mode 加密算法
 * @param key 加密密钥
 * @param iv 偏移量
 * @return QString 密文数据，如果加密失败则为空字符串
 */
QByteArray aesEncrypt (const QByteArray &plainText, const EVP_CIPHER *mode(), const QByteArray &key, const QByteArray &iv, QString format = "base64") {
    EVP_CIPHER_CTX *ctx;
    int len;
    unsigned char *ciphertext = new unsigned char[plainText.size() * 10];
    int ciphertext_len;
    if (! (ctx = EVP_CIPHER_CTX_new ())) ERR_print_errors_fp (stderr);
    if (1 != EVP_EncryptInit_ex (ctx, mode (), NULL,
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
 * @param cipherData 密文数据
 * @param mode 解密算法
 * @param key 解密密钥
 * @param iv 偏移量
 * @return QString 明文数据，如果解密失败则为空字符串
 */
static QByteArray aesDecrypt(const QByteArray &cipherData, const EVP_CIPHER *mode(), const QString &key, const QString &iv)
{
    // 创建解密上下文对象，并使用智能指针接管
    QScopedPointer<EVP_CIPHER_CTX, QScopedPointerPodDeleter> ctx(EVP_CIPHER_CTX_new());
    if (NULL == ctx.data())
    {
        return "";
    }

    // 将QString类型的输入参数转换为char*类型
    const char *cipherDataChar = cipherData.constData();
    const char *keyChar = key.toUtf8();
    const char *ivChar = iv.toUtf8();

    // 计算解密后的数据长度，至少要和密文数据长度一样大
    int outBufLen = cipherData.size();

    // 使用QByteArray代替动态分配的数组
    QByteArray outBuf(outBufLen, '\0');

    // 初始化解密上下文，设置解密算法、密钥、偏移量等参数
    EVP_DecryptInit_ex(ctx.data(), mode(), NULL, (const unsigned char *)keyChar, (const unsigned char *)ivChar);
    // 解密密文数据，将结果存放在outBuf中，更新outBufLen为输出长度
    if (!EVP_DecryptUpdate(ctx.data(), (unsigned char *)outBuf.data(), &outBufLen, (const unsigned char *)cipherDataChar, cipherData.size()))
    {
        return "";
    }

    int tempLen = -1;
    // 结束解密过程，处理剩余的数据，将结果追加到outBuf中，更新tempLen为输出长度
    EVP_DecryptFinal_ex(ctx.data(), (unsigned char *)outBuf.data() + outBufLen, &tempLen);
    outBufLen += tempLen;

    // 将outBuf中的明文数据转换为QByteArray类型，并使用UTF-8编码
    QByteArray plainDataBytes = outBuf.left(outBufLen);

    return plainDataBytes;
}

/**
 * @brief rsaEncrypt 公钥加密
 * @param plainText 明文
 * @param strPubKey 公钥
 * @return 加密后数据(Hex格式)
 */
static QByteArray rsaEncrypt (QString plainText, const QString& strPubKey)
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

QVariantMap weapi(QJsonDocument object) {
    const QString text = object.toJson(QJsonDocument::Compact);

    // 创建一个长度为16的字节数组
    QByteArray secretKey;
    secretKey.resize(16);

    // 生成随机字节并映射到base62字符的ASCII码
    for (int i = 0; i < secretKey.size(); i++) {
        // 生成一个随机的quint32值取其最低8位作为字节值
        quint8 byte = QRandomGenerator::global()->generate() & 0xFF;
        // 对62取余得到base62字符的索引
        int index = byte % base62.length();
        // 获取base62字符并转换为ASCII码存入结果数组
        secretKey[i] = base62.at(index).toLatin1();
    }

    auto params = aesEncrypt(aesEncrypt(text.toUtf8(), EVP_aes_128_cbc, presetKey.toUtf8().data(), iv.toUtf8().data(), "base64"),
                             EVP_aes_128_cbc, secretKey.data(), iv.toUtf8().data(), "base64");
    std::reverse(secretKey.begin(), secretKey.end());
    auto encSecKey = rsaEncrypt(secretKey, publicKey).toHex();

    return {
        { QStringLiteral("params"), params },
        { QStringLiteral("encSecKey"), encSecKey }
    };
}

QVariantMap linuxapi(QJsonDocument object) {
    const QString text = object.toJson(QJsonDocument::Indented);
    return {
        { QStringLiteral("eparams"), aesEncrypt(text.toUtf8(), EVP_aes_128_ecb, linuxapiKey.toUtf8().data(), QStringLiteral("").toUtf8().data(), "hex") }
    };
}

QVariantMap eapi(QString url, QJsonDocument object) {
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
        { "params", aesEncrypt(data.toUtf8(), EVP_aes_128_ecb, eapiKey.toUtf8().data(), QStringLiteral("").toUtf8().data(), "hex") }
    };
}

QByteArray decrypt(QByteArray cipherBuffer) {
    return aesDecrypt(cipherBuffer, EVP_aes_128_ecb, eapiKey, "");
}
}

#endif // CRYPTO_H
