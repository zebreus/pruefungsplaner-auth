#include "client.h"

SecurityProviderClient::SecurityProviderClient(QObject *parent):
    QObject(parent), webSocket()
{
    connect(&webSocket, &QWebSocket::connected, this, &SecurityProviderClient::onConnected);
    connect(&webSocket, &QWebSocket::disconnected, this, &SecurityProviderClient::onDisconnected);
    connect(&webSocket, &QWebSocket::textMessageReceived, this, &SecurityProviderClient::onTextMessageReceived);
    connect(&webSocket, &QWebSocket::binaryMessageReceived, this, &SecurityProviderClient::onBinaryMessageReceived);
    connect(&webSocket, QOverload<QAbstractSocket::SocketError>::of(&QWebSocket::error), this, &SecurityProviderClient::onError);
}

void SecurityProviderClient::getToken(const QString& username, const QString& password, const QList<QString>& claims, const QList<QString>& audiences)
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

void SecurityProviderClient::getToken(const QString &username, const QString &password, const QList<QString> &claims, const QString &audience)
{
    getToken(username, password, claims, QList<QString>{audience});
}

void SecurityProviderClient::getToken(const QString &username, const QString &password, const QList<QString> &claims)
{
    getToken(username, password, claims, QList<QString>{});
}

void SecurityProviderClient::getToken(const QString &username, const QString &password, const QString &claim, const QList<QString> &audiences)
{
    getToken(username, password, QList<QString>{claim}, audiences);
}

void SecurityProviderClient::getToken(const QString &username, const QString &password, const QString &claim, const QString &audience)
{
    getToken(username, password, QList<QString>{claim}, QList<QString>{audience});
}

void SecurityProviderClient::getToken(const QString &username, const QString &password, const QString &claim)
{
    getToken(username, password, QList<QString>{claim}, QList<QString>{});
}

void SecurityProviderClient::getPublicKey()
{
    Request request;
    request.type = PUBLIC_KEY;
    if(webSocket.isValid()){
        sendRequest(request);
    }else{
        pendingRequests.append(request);
    }
}

void SecurityProviderClient::getIssuer()
{
    Request request;
    request.type = ISSUER;
    if(webSocket.isValid()){
        sendRequest(request);
    }else{
        pendingRequests.append(request);
    }

}

void SecurityProviderClient::open(const QUrl &url)
{
    if(!webSocket.isValid()){
        webSocket.open(url);
    }
}

QString SecurityProviderClient::buildTokenRequest(const SecurityProviderClient::Request &request, int id)
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

QString SecurityProviderClient::buildPublicKeyRequest(int id)
{
    QJsonObject jsonRequest;
    jsonRequest["jsonrpc"] = "2.0";
    jsonRequest["id"] = id;
    jsonRequest["method"] = "getPublicKey";
    QJsonDocument doc;
    doc.setObject(jsonRequest);
    return doc.toJson(QJsonDocument::Compact);
}

QString SecurityProviderClient::buildIssuerRequest(int id)
{
    QJsonObject jsonRequest;
    jsonRequest["jsonrpc"] = "2.0";
    jsonRequest["id"] = id;
    jsonRequest["method"] = "getIssuer";
    QJsonDocument doc;
    doc.setObject(jsonRequest);
    return doc.toJson(QJsonDocument::Compact);
}

void SecurityProviderClient::sendRequest(const SecurityProviderClient::Request &request)
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

void SecurityProviderClient::onConnected()
{
    qDebug("Connected");
    for(const Request& request : pendingRequests){
        sendRequest(request);
    }
    pendingRequests.clear();
}

void SecurityProviderClient::onError(QAbstractSocket::SocketError e)
{
    emit socketError(e);
}

void SecurityProviderClient::onBinaryMessageReceived(const QByteArray &message)
{
    onTextMessageReceived(QString::fromLatin1(message));
}

void SecurityProviderClient::onTextMessageReceived(const QString& message)
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

void SecurityProviderClient::onDisconnected()
{
    //TODO do something
}
