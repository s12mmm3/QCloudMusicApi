#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QVariantMap>
#include <QHttpServer>

class Server: public QObject
{
    Q_OBJECT
public:
    Server();

    /**
     * Serve the NCM API.
     * @param QVariantMap options
     * @returns void
     */
    void serveNcmApi(QVariantMap options);

private:
    /**
     * Construct the server of NCM API.
     */
    void consturctServer(QVariantMap options);

    /**
     * Get the module definitions dynamically.
     * @param QVariantMap [specificRoute] The specific route of specific modules.
     * @returns QVariantList The module definitions.
     */
    QVariantList getModulesDefinitions(QVariantMap specificRoute);

private:
    QHttpServer server;
};

#endif // SERVER_H
