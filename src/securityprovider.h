#ifndef SECURITYPROVIDER_H
#define SECURITYPROVIDER_H

#include <QObject>
#include <QString>
#include <QJsonValue>

class SecurityProvider : public QObject
{
    Q_OBJECT
private:
    QString privateKey;
    QString publicKey;
public:
    explicit SecurityProvider(const QString& privateKey, const QString& publicKey, QObject *parent = nullptr);

public slots:
    QString getToken(QString userName, QString password, QJsonValue claimsArray);

signals:

};

#endif // SECURITYPROVIDER_H
