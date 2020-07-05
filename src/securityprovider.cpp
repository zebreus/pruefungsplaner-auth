#include "securityprovider.h"

SecurityProvider::SecurityProvider(const QString &privateKey, const QString &publicKey, QObject *parent):
    QObject(parent), privateKey(privateKey), publicKey(publicKey)
{

}

QString SecurityProvider::getToken(QString userName, QString password, QJsonValue claimsArray)
{
    return "";
}
