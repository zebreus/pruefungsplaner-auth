#ifndef SECURITYUSERCLIENT_H
#define SECURITYUSERCLIENT_H

#include <QString>
#include <QObject>
#include <QWebSocket>
#include <QUrl>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>

class SecurityProviderClient : public QObject
{
    Q_OBJECT
public:
    enum Error{
        SOCKET_ERROR,
        UNEXPECTED_MESSAGE,
        INVALID_RESPONSE,
        SERVER_ERROR
    };
    explicit SecurityProviderClient(QObject *parent = nullptr);
    //Connection not open -> opens connection
    //Connection open -> does nothing
    void open(const QUrl &url);

    //Will trigger a gotToken or a  error signal
    void getToken(const QString& username, const QString& password, const QList<QString>& claims, const QList<QString>& audiences);
    void getToken(const QString& username, const QString& password, const QList<QString>& claims, const QString& audience);
    void getToken(const QString& username, const QString& password, const QList<QString>& claims);
    void getToken(const QString& username, const QString& password, const QString& claim, const QList<QString>& audiences);
    void getToken(const QString& username, const QString& password, const QString& claim, const QString& audience);
    void getToken(const QString& username, const QString& password, const QString& claim);
    //Will trigger a gotPublicKey or a  error signal
    void getPublicKey();
    //Will trigger a gotIssuer or a  error signal
    void getIssuer();

signals:
    void gotToken(const QString& token);
    void gotPublicKey(const QString& publicKey);
    void gotIssuer(const QString& issuer);
    void error(Error error);
    void socketError(QAbstractSocket::SocketError err);

private:
    enum RequestType{
        TOKEN,
        PUBLIC_KEY,
        ISSUER
    };
    struct Request{
        RequestType type;
        QString name;
        QString password;
        QList<QString> claims;
        QList<QString> audiences;
    };

    QWebSocket webSocket;
    QList<Request> pendingRequests;
    QMap<int, RequestType> activeRequests;
    int currentId = 0;
    void sendRequest(const Request& r);
    QString buildTokenRequest(const Request& r, int id);
    QString buildPublicKeyRequest(int id);
    QString buildIssuerRequest(int id);
private Q_SLOTS:
    void onBinaryMessageReceived(const QByteArray &message);
    void onConnected();
    void onError(QAbstractSocket::SocketError error);
    void onTextMessageReceived(const QString& message);
    void onDisconnected();
};

#endif // SECURITYUSERCLIENT_H
