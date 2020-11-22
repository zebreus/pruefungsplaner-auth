#ifndef SECURITYPROVIDER_H
#define SECURITYPROVIDER_H

#include <QObject>
#include <QString>
#include <QJsonValue>
#include <QJsonArray>
#include <chrono>
#include "QtJsonTraits.h"
#include "jwt-cpp/jwt.h"
#include "configuration.h"
#include <QSharedPointer>

class SecurityProvider : public QObject
{
    Q_OBJECT
private:
    QSharedPointer<Configuration> configuration;
public:
    explicit SecurityProvider(const QSharedPointer<Configuration>& configuration, QObject *parent = nullptr);

public slots:
    QString getToken(QString userName, QString password, QJsonValue claimsArray, QJsonArray audiences);
    QString getPublicKey();
    QString getIssuer();

signals:

};

#endif // SECURITYPROVIDER_H
