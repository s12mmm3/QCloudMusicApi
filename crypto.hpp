#ifndef CRYPTO_H
#define CRYPTO_H

#include <QByteArray>
#include <QObject>

extern "C" {
#include <openssl/rc4.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
}

namespace Crypto {

const QByteArray iv = QByteArray::fromStdString("0102030405060708");
const QByteArray presetKey = QByteArray::fromStdString("0CoJUm6Qyw8W8jud");
const QByteArray linuxapiKey = QByteArray::fromStdString("rFgB&h#%2?^eDg:Q");
const QString base62 = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
const QString publicKey = "-----BEGIN PUBLIC KEY-----\nMIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQDgtQn2JZ34ZC28NWYpAUd98iZ37BUrX/aKzmFbt7clFSs6sXqHauqKWqdtLkF2KexO40H1YTX8z2lSgBBOAxLsvaklV8k4cBFK9snQXE9/DDaFt6Rr7iVZMldczhC0JNgTz+SHXT6CBHuX3e9SdB1Ua44oncaTWz7OBGLbCiK45wIDAQAB\n-----END PUBLIC KEY-----";
const QString eapiKey = "e82ckenh8dichen8";

/**
 * @brief 使用AES-128-CBC算法加密数据的函数
 * @param plainData 明文数据
 * @param mode 加密算法
 * @param key 加密密钥
 * @param iv 偏移量
 * @return QString 密文数据，如果加密失败则为空字符串
 */
QString aesEncrypt(const QString &plainData, const EVP_CIPHER *mode(), const QString &key, const QString &iv)
{
    // 创建加密上下文对象，并使用智能指针接管
    QScopedPointer<EVP_CIPHER_CTX, QScopedPointerPodDeleter> ctx(EVP_CIPHER_CTX_new());
    if (NULL == ctx.data())
    {
        return "";
    }

    // 将QString类型的输入参数转换为char*类型
    QByteArray plainDataBytes = plainData.toUtf8();
    QByteArray keyBytes = key.toUtf8();
    QByteArray ivBytes = iv.toUtf8();
    const char *plainDataChar = plainDataBytes.constData();
    const char *keyChar = keyBytes.constData();
    const char *ivChar = ivBytes.constData();

    // 计算加密后的数据长度，至少要比明文数据长度大一个块大小
    int outBufLen = plainDataBytes.size() + EVP_CIPHER_block_size(mode());
    // 使用QByteArray代替动态分配的数组
    QByteArray outBuf(outBufLen, '\0');

    // 初始化加密上下文，设置加密算法、密钥、偏移量等参数
    EVP_EncryptInit_ex(ctx.data(), mode(), NULL, (const unsigned char *)keyChar, (const unsigned char *)ivChar);
    // 加密明文数据，将结果存放在outBuf中，更新outBufLen为输出长度
    if (!EVP_EncryptUpdate(ctx.data(), (unsigned char *)outBuf.data(), &outBufLen, (const unsigned char *)plainDataChar, plainDataBytes.size()))
    {
        return "";
    }

    int tempLen = -1;
    // 结束加密过程，处理剩余的数据，将结果追加到outBuf中，更新tempLen为输出长度
    if (!EVP_EncryptFinal_ex(ctx.data(), (unsigned char *)outBuf.data() + outBufLen, &tempLen))
    {
        return "";
    }

    outBufLen += tempLen;

    // 将outBuf中的密文数据转换为QByteArray类型，并使用Hex编码
    QByteArray cipherDataBytes = outBuf.left(outBufLen).toHex();

    // 将QByteArray类型的密文数据转换为QString类型
    QString cipherData = QString::fromUtf8(cipherDataBytes);

    return cipherData;
}


/**
 * @brief 使用AES-128-CBC算法解密数据的函数
 * @param cipherData 密文数据
 * @param mode 解密算法
 * @param key 解密密钥
 * @param iv 偏移量
 * @return QString 明文数据，如果解密失败则为空字符串
 */
QString aesDecrypt(const QString &cipherData, const EVP_CIPHER *mode(), const QString &key, const QString &iv)
{
    // 创建解密上下文对象，并使用智能指针接管
    QScopedPointer<EVP_CIPHER_CTX, QScopedPointerPodDeleter> ctx(EVP_CIPHER_CTX_new());
    if (NULL == ctx.data())
    {
        return "";
    }

    // 将QString类型的输入参数转换为char*类型，并使用Hex解码
    QByteArray cipherDataBytes = QByteArray::fromHex(cipherData.toUtf8());
    QByteArray keyBytes = key.toUtf8();
    QByteArray ivBytes = iv.toUtf8();
    const char *cipherDataChar = cipherDataBytes.constData();
    const char *keyChar = keyBytes.constData();
    const char *ivChar = ivBytes.constData();

    // 计算解密后的数据长度，至少要和密文数据长度一样大
    int outBufLen = cipherDataBytes.size();

    // 使用QByteArray代替动态分配的数组
    QByteArray outBuf(outBufLen, '\0');

    // 初始化解密上下文，设置解密算法、密钥、偏移量等参数
    EVP_DecryptInit_ex(ctx.data(), mode(), NULL, (const unsigned char *)keyChar, (const unsigned char *)ivChar);
    // 解密密文数据，将结果存放在outBuf中，更新outBufLen为输出长度
    if (!EVP_DecryptUpdate(ctx.data(), (unsigned char *)outBuf.data(), &outBufLen, (const unsigned char *)cipherDataChar, cipherDataBytes.size()))
    {
        return "";
    }

    int tempLen = -1;
    // 结束解密过程，处理剩余的数据，将结果追加到outBuf中，更新tempLen为输出长度
    EVP_DecryptFinal_ex(ctx.data(), (unsigned char *)outBuf.data() + outBufLen, &tempLen);
    outBufLen += tempLen;

    // 将outBuf中的明文数据转换为QByteArray类型，并使用UTF-8编码
    QByteArray plainDataBytes = outBuf.left(outBufLen);

    // 将QByteArray类型的明文数据转换为QString类型
    QString plainData = QString::fromUtf8(plainDataBytes);

    return plainData;
}

}



#endif // CRYPTO_H
