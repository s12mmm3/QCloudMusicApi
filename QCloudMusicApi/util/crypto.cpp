#include "crypto.h"

#include <iostream>
#include <sstream>

#include <QByteArray>
#include <QObject>
#include <QVariantMap>
#include <QJsonDocument>
#include <QRandomGenerator>
#include <QCryptographicHash>
#include <QRegularExpression>
#include <QDebug>

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
#include <cryptopp/aes.h>
#include <cryptopp/modes.h>
#include <cryptopp/filters.h>
#include <cryptopp/hex.h>
#include <cryptopp/base64.h>
#include <cryptopp/rsa.h>
#include <cryptopp/osrng.h>
#include <cryptopp/integer.h> // 用于原始RSA运算

/**
 * @brief AES加密
 * @param plainText 明文数据
 * @param mode 加密模式("cbc"或"ecb")
 * @param key 加密密钥(16字节对于AES-128)
 * @param iv 偏移量(16字节对于CBC模式)
 * @param format 输出格式("base64"或"hex")
 * @return QByteArray 密文数据，如果加密失败则为空字符串
 */
QByteArray Crypto::aesEncrypt(const QByteArray& plainText, const QString mode, const QByteArray& key, const QByteArray& iv, QString format) {
    try {
        std::string ciphertext;
        // 将QByteArray明文转换为std::string
        std::string plainTextStd(plainText.constData(), plainText.size());

        if (mode.toLower() == "cbc") {
            CryptoPP::CBC_Mode<CryptoPP::AES>::Encryption encryptor;
            encryptor.SetKeyWithIV(
                reinterpret_cast<const CryptoPP::byte*>(key.constData()),
                key.size(),
                reinterpret_cast<const CryptoPP::byte*>(iv.constData()),
                iv.size()
                );
            // 使用std::string版本的StringSource，避免转换
            CryptoPP::StringSource(plainTextStd, true,
                                   new CryptoPP::StreamTransformationFilter(
                                       encryptor,
                                       new CryptoPP::StringSink(ciphertext)
                                       )
                                   );
        } else if (mode.toLower() == "ecb") {
            CryptoPP::ECB_Mode<CryptoPP::AES>::Encryption encryptor;
            encryptor.SetKey(
                reinterpret_cast<const CryptoPP::byte*>(key.constData()),
                key.size()
                );
            CryptoPP::StringSource(plainTextStd, true,
                                   new CryptoPP::StreamTransformationFilter(
                                       encryptor,
                                       new CryptoPP::StringSink(ciphertext)
                                       )
                                   );
        } else {
            qWarning() << "Unsupported AES mode:" << mode;
            return QByteArray();
        }

        // 将结果转换回QByteArray
        QByteArray result = QByteArray::fromStdString(ciphertext);

        if (format.toLower() == "base64") {
            return result.toBase64();
        }
        return result.toHex().toUpper();

    } catch (const CryptoPP::Exception& e) {
        qWarning() << "Crypto++ encryption error:" << e.what();
        return QByteArray();
    }
}

/**
 * @brief AES解密
 * @param cipherText 密文数据
 * @param mode 解密模式("cbc"或"ecb")
 * @param key 解密密钥(16字节对于AES-128)
 * @param iv 偏移量(16字节对于CBC模式)
 * @param format 输入格式("base64"或"hex")
 * @return QByteArray 明文数据，如果解密失败则为空字符串
 */
QByteArray Crypto::aesDecrypt(const QByteArray& cipherText, const QString mode, const QByteArray& key, const QByteArray& iv, QString format) {
    try {
        // 根据格式解码输入数据
        QByteArray decodedCipherText;
        if (format.toLower() == "base64") {
            decodedCipherText = QByteArray::fromBase64(cipherText);
        } else {
            decodedCipherText = QByteArray::fromHex(cipherText);
        }

        std::string plaintext;
        // 将解码后的QByteArray转换为std::string
        std::string cipherTextStd(decodedCipherText.constData(), decodedCipherText.size());

        if (mode.toLower() == "cbc") {
            CryptoPP::CBC_Mode<CryptoPP::AES>::Decryption decryptor;
            decryptor.SetKeyWithIV(
                reinterpret_cast<const CryptoPP::byte*>(key.constData()),
                key.size(),
                reinterpret_cast<const CryptoPP::byte*>(iv.constData()),
                iv.size()
                );
            // 使用std::string版本的StringSource
            CryptoPP::StringSource(cipherTextStd, true,
                                   new CryptoPP::StreamTransformationFilter(
                                       decryptor,
                                       new CryptoPP::StringSink(plaintext)
                                       )
                                   );
        } else if (mode.toLower() == "ecb") {
            CryptoPP::ECB_Mode<CryptoPP::AES>::Decryption decryptor;
            decryptor.SetKey(
                reinterpret_cast<const CryptoPP::byte*>(key.constData()),
                key.size()
                );
            CryptoPP::StringSource(cipherTextStd, true,
                                   new CryptoPP::StreamTransformationFilter(
                                       decryptor,
                                       new CryptoPP::StringSink(plaintext)
                                       )
                                   );
        } else {
            qWarning() << "Unsupported AES mode:" << mode;
            return QByteArray();
        }

        return QByteArray::fromStdString(plaintext);

    } catch (const CryptoPP::Exception& e) {
        qWarning() << "Crypto++ decryption error:" << e.what();
        return QByteArray();
    }
}

/**
 * @brief 使用无填充模式进行RSA公钥加密
 * @param plainText 明文数据
 * @param strPubKey PEM格式的公钥字符串
 * @return QByteArray 加密后的数据，失败返回空QByteArray
 */
QByteArray Crypto::rsaEncrypt(const QString& plainText, const QString& strPubKey) {
    try {
        // 将QString转换为std::string
        std::string pubKeyStr = strPubKey.toStdString();
        std::string plainTextStr = plainText.toStdString();

        // 从PEM格式中提取Base64编码的密钥数据
        std::string keyBody;
        size_t begin = pubKeyStr.find("-----BEGIN");
        size_t end = pubKeyStr.find("-----END");

        if (begin != std::string::npos && end != std::string::npos) {
            size_t keyStart = pubKeyStr.find("\n", begin) + 1;
            size_t keyEnd = pubKeyStr.find("\n", keyStart);
            while (keyEnd < end) {
                std::string line = pubKeyStr.substr(keyStart, keyEnd - keyStart);
                if (line.find(":") == std::string::npos) { // 跳过头信息行
                    keyBody += line;
                }
                keyStart = keyEnd + 1;
                keyEnd = pubKeyStr.find("\n", keyStart);
                if (keyEnd == std::string::npos) keyEnd = end;
            }
        }

        // Base64解码获取DER格式的公钥
        std::string derKey;
        CryptoPP::StringSource(keyBody, true,
                               new CryptoPP::Base64Decoder(
                                   new CryptoPP::StringSink(derKey)
                                   )
                               );

        // 从DER格式加载公钥
        CryptoPP::RSA::PublicKey publicKey;
        CryptoPP::StringSource derSource(derKey, true);
        publicKey.Load(derSource);

        // 获取模数(n)和公钥指数(e)
        CryptoPP::Integer n = publicKey.GetModulus();
        CryptoPP::Integer e = publicKey.GetPublicExponent();

        int keySizeBytes = n.ByteCount(); // 模数的字节长度

        // 处理明文长度：必须精确等于模数字节长度
        if (plainTextStr.length() < keySizeBytes) {
            // 前面补零
            plainTextStr.insert(0, keySizeBytes - plainTextStr.length(), '\0');
        } else if (plainTextStr.length() > keySizeBytes) {
            // 截断超长部分
            plainTextStr = plainTextStr.substr(0, keySizeBytes);
        }

        // 将明文转换为CryptoPP::Integer
        CryptoPP::Integer plainInteger(
            reinterpret_cast<const CryptoPP::byte*>(plainTextStr.data()),
            plainTextStr.size()
            );

        // 核心：无填充RSA加密 (m^e mod n)
        CryptoPP::Integer cipherInteger = publicKey.ApplyFunction(plainInteger);

        // 将加密结果转换回字节
        std::string cipherText;
        cipherText.resize(keySizeBytes); // 密文长度等于模数长度
        cipherInteger.Encode(
            reinterpret_cast<CryptoPP::byte*>(cipherText.data()),
            cipherText.size(),
            CryptoPP::Integer::UNSIGNED
            );

        // 转换为QByteArray
        QByteArray encryptData = QByteArray::fromStdString(cipherText);
        return encryptData;

    } catch (const CryptoPP::Exception& e) {
        qWarning() << "Raw RSA encryption error:" << e.what();
        return QByteArray();
    } catch (const std::exception& e) {
        qWarning() << "Standard exception in rsaEncryptRaw:" << e.what();
        return QByteArray();
    }
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

QVariantMap Crypto::eapiResDecrypt(const QByteArray& encryptedParams) {
    // 使用aesDecrypt解密参数
    auto decryptedData = aesDecrypt(encryptedParams, "ecb", eapiKey.toUtf8(), "", "hex");
    return QJsonDocument::fromJson(decryptedData).toVariant().toMap();
}

QVariantMap Crypto::eapiReqDecrypt(const QByteArray& encryptedParams) {
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
