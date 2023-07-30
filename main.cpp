#include <QCoreApplication>
#include "Qt-AES/qaesencryption.h"

const QByteArray iv = QByteArray::fromStdString("0102030405060708");
const QByteArray presetKey = QByteArray::fromStdString("0CoJUm6Qyw8W8jud");
const QByteArray linuxapiKey = QByteArray::fromStdString("rFgB&h#%2?^eDg:Q");
const QString base62 = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
const QString publicKey = "-----BEGIN PUBLIC KEY-----\nMIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQDgtQn2JZ34ZC28NWYpAUd98iZ37BUrX/aKzmFbt7clFSs6sXqHauqKWqdtLkF2KexO40H1YTX8z2lSgBBOAxLsvaklV8k4cBFK9snQXE9/DDaFt6Rr7iVZMldczhC0JNgTz+SHXT6CBHuX3e9SdB1Ua44oncaTWz7OBGLbCiK45wIDAQAB\n-----END PUBLIC KEY-----";
const QString eapiKey = "e82ckenh8dichen8";

/**
 * @brief encodeAES 使用AES算法对明文进行加密
 * @param plaintext 明文字符串
 * @param aesKey 密钥字符串
 * @param aesIv 偏移量字符串
 * @return 密文字符串，经过base64编码
 */
QString encodeAES(QString plaintext, QByteArray presetKey, QByteArray iv)
{
    // 创建一个AES加密对象，指定AES_128模式，CBC模式和PKCS7填充方式
    QAESEncryption aes(QAESEncryption::AES_128, QAESEncryption::CBC , QAESEncryption::PKCS7);
    // 将明文、密钥和偏移量从QString转换为QByteArray
    QByteArray input(plaintext.toLocal8Bit());
    // 调用aes.encode方法对明文进行加密，并将结果转换为base64编码
    QByteArray encodedText = aes.encode(input, presetKey, iv).toBase64();
    // 将加密后的字节数组转换为QString并返回
    QString encoded = encodedText;
    QByteArray encoded1 = encoded.toUtf8();
    return encoded1;
}

/**
 * @brief decodeAES 使用AES算法对密文进行解密
 * @param ciphertext 密文字符串，经过base64编码
 * @param aesKey 密钥字符串
 * @param aesIv 偏移量字符串
 * @return 明文字符串
 */
QString decodeAES(QString ciphertext, QByteArray presetKey, QByteArray iv) {
    // 创建一个AES解密对象，指定AES_128模式，CBC模式和PKCS7填充方式
    QAESEncryption aes(QAESEncryption::AES_128, QAESEncryption::CBC , QAESEncryption::PKCS7);
    // 将密文、密钥和偏移量从QString转换为QByteArray
    QByteArray input(ciphertext.toLocal8Bit());
    // 调用aes.decode方法对密文进行解密，并将结果从base64编码转换为原始字节数组
    QByteArray decodedText = aes.decode(QByteArray::fromBase64(input), presetKey, iv);
    // 调用aes.removePadding方法去除填充字符
    decodedText = aes.removePadding(decodedText);
    // 将解密后的字节数组转换为QString并返回
    QString decrypted = decodedText;
    QByteArray decrypted1 = decrypted.toUtf8();
    return decrypted1;
}

int main(int argc, char *argv[])
{
    auto ciphertext = encodeAES("HelloWorld!", presetKey, iv); // 使用AES加密算法加密字符串
    qDebug() << ciphertext << decodeAES(ciphertext, presetKey, iv); // 打印密文和解密后的明文
}
