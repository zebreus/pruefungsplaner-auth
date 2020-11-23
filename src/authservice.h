#ifndef AUTHSERVICE_H
#define AUTHSERVICE_H

#include <QObject>
#include <QString>
#include <QJsonValue>
#include <QJsonArray>
#include <chrono>
#include "QtJsonTraits.h"
#include "jwt-cpp/jwt.h"
#include "configuration.h"
#include <QSharedPointer>

class AuthService : public QObject
{
    Q_OBJECT
private:
    QSharedPointer<Configuration> configuration;
public:
    explicit AuthService(const QSharedPointer<Configuration>& configuration, QObject *parent = nullptr);

public slots:
    QString getToken(QString userName, QString password, QJsonValue claimsArray, QJsonArray audiences);
    QString getPublicKey();
    QString getIssuer();

signals:

};

#endif // AUTHSERVICE_H
