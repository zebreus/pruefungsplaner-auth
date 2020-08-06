#include "security-provider/client.h"

namespace securityprovider {

Client::Client(QObject *parent):
    QObject(parent), webSocket()
{
    connect(&webSocket, &QWebSocket::connected, this, &Client::onConnected);
    connect(&webSocket, &QWebSocket::disconnected, this, &Client::onDisconnected);
    connect(&webSocket, &QWebSocket::textMessageReceived, this, &Client::onTextMessageReceived);
    connect(&webSocket, &QWebSocket::binaryMessageReceived, this, &Client::onBinaryMessageReceived);
    connect(&webSocket, QOverload<QAbstractSocket::SocketError>::of(&QWebSocket::error), this, &Client::onError);
}

void Client::getToken(const QString& username, const QString& password, const QList<QString>& claims, const QList<QString>& audiences)
{
    Request request;
    request.type = TOKEN;
    request.name = username;
    request.password = password;
    request.claims = claims;
    request.audiences = audiences;
    if(webSocket.isValid()){
        sendRequest(request);
    }else{
        pendingRequests.append(request);
    }
}

void Client::getToken(const QString &username, const QString &password, const QList<QString> &claims, const QString &audience)
{
    getToken(username, password, claims, QList<QString>{audience});
}

void Client::getToken(const QString &username, const QString &password, const QList<QString> &claims)
{
    getToken(username, password, claims, QList<QString>{});
}

void Client::getToken(const QString &username, const QString &password, const QString &claim, const QList<QString> &audiences)
{
    getToken(username, password, QList<QString>{claim}, audiences);
}

void Client::getToken(const QString &username, const QString &password, const QString &claim, const QString &audience)
{
    getToken(username, password, QList<QString>{claim}, QList<QString>{audience});
}

void Client::getToken(const QString &username, const QString &password, const QString &claim)
{
    getToken(username, password, QList<QString>{claim}, QList<QString>{});
}

void Client::getPublicKey()
{
    Request request;
    request.type = PUBLIC_KEY;
    if(webSocket.isValid()){
        sendRequest(request);
    }else{
        pendingRequests.append(request);
    }
}

void Client::getIssuer()
{
    Request request;
    request.type = ISSUER;
    if(webSocket.isValid()){
        sendRequest(request);
    }else{
        pendingRequests.append(request);
    }

}

void Client::open(const QUrl &url)
{
    if(!webSocket.isValid()){
        webSocket.open(url);
    }
}

QString Client::buildTokenRequest(const Client::Request &request, int id)
{
    QJsonObject jsonRequest;
    jsonRequest["jsonrpc"] = "2.0";
    jsonRequest["id"] = id;
    jsonRequest["method"] = "getToken";
    QJsonArray params;
    params.append(request.name);
    params.append(request.password);
    params.append(QJsonArray::fromStringList(request.claims));
    params.append(QJsonArray::fromStringList(request.audiences));
    jsonRequest["params"] = params;
    QJsonDocument doc;
    doc.setObject(jsonRequest);
    return doc.toJson(QJsonDocument::Compact);
}

QString Client::buildPublicKeyRequest(int id)
{
    QJsonObject jsonRequest;
    jsonRequest["jsonrpc"] = "2.0";
    jsonRequest["id"] = id;
    jsonRequest["method"] = "getPublicKey";
    QJsonDocument doc;
    doc.setObject(jsonRequest);
    return doc.toJson(QJsonDocument::Compact);
}

QString Client::buildIssuerRequest(int id)
{
    QJsonObject jsonRequest;
    jsonRequest["jsonrpc"] = "2.0";
    jsonRequest["id"] = id;
    jsonRequest["method"] = "getIssuer";
    QJsonDocument doc;
    doc.setObject(jsonRequest);
    return doc.toJson(QJsonDocument::Compact);
}

void Client::sendRequest(const Client::Request &request)
{
    int id = currentId++;
    QString message;
    switch(request.type){
        case TOKEN:
            message = buildTokenRequest(request, id);
            break;
        case PUBLIC_KEY:
            message = buildPublicKeyRequest(id);
            break;
        case ISSUER:
            message = buildIssuerRequest(id);
    }
    activeRequests.insert(id, request.type);
    webSocket.sendTextMessage(message);
}

void Client::onConnected()
{
    qDebug("Connected");
    for(const Request& request : pendingRequests){
        sendRequest(request);
    }
    pendingRequests.clear();
}

void Client::onError(QAbstractSocket::SocketError e)
{
    emit socketError(e);
}

void Client::onBinaryMessageReceived(const QByteArray &message)
{
    onTextMessageReceived(QString::fromLatin1(message));
}

void Client::onTextMessageReceived(const QString& message)
{
    qDebug() << message;
    QJsonDocument document = QJsonDocument::fromJson(message.toLatin1());
    QJsonObject response = document.object();
    QJsonValue idValue = response.value("id");
    if(!idValue.isDouble()){
        emit error(INVALID_RESPONSE);
        return;
    }
    int id = idValue.toInt();

    QJsonValue resultValue = response.value("result");
    QString result;
    if(resultValue.isString()){
        result = resultValue.toString();
    }else if(resultValue.isArray() && resultValue.toArray().first().isString()){
        result = resultValue.toArray().first().toString();
    }else{
        emit error(INVALID_RESPONSE);
        return;
    }

    auto type = activeRequests.find(id);
    if(type == activeRequests.end()){
        emit error(UNEXPECTED_MESSAGE);
        return;
    }
    switch(type.value()){
        case TOKEN:
            emit gotToken(result);
            break;
        case PUBLIC_KEY:
            emit gotPublicKey(result);
            break;
        case ISSUER:
            emit gotIssuer(result);
            break;
    }
}

void Client::onDisconnected()
{
    //TODO do something
}

}
