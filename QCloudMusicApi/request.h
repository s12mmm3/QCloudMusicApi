#include <QObject>
#include <QNetworkAccessManager>

namespace Request {

QString chooseUserAgent(QString ua = "");

QVariantMap createRequest(QNetworkAccessManager::Operation method,
                          QString urlStr,
                          QVariantMap data,
                          QVariantMap options);

}
