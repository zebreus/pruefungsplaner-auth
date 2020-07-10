#include "securityprovider.h"

SecurityProvider::SecurityProvider(const QString &privateKey, const QString &publicKey, QObject *parent):
    QObject(parent), privateKey(privateKey), publicKey(publicKey)
{

}

QString SecurityProvider::getToken(QString userName, QString password, QJsonValue claimsArray)
{
    auto creationTime = std::chrono::system_clock::now();
    auto expirationTime = creationTime + std::chrono::seconds{3600};

    QList<QString> claims;

    if(userName == "test" && password == "test"){

    if(claimsArray.isArray()){
        QJsonArray claimsJsonArray = claimsArray.toArray();
        for(const QJsonValue& claimEntry : claimsJsonArray){
            if(claimEntry.isString()){
                claims.append(claimEntry.toString());
            }
        }
    }

    }

    //Added registered and public claims
    jwt::builder tokenBuilder = jwt::create()
    .set_type("JWT")
    .set_issuer("securityprovider")
    //TODO maybe sanitize username
    .set_subject(userName.toUtf8().constData())
    //TODO generate random id
    .set_id("random id")
    .set_audience("someone")
    .set_issued_at(creationTime)
    .set_expires_at(expirationTime);

    for(QString claim : claims){
        tokenBuilder.set_payload_claim(claim.toUtf8().constData(), jwt::claim((std::string)"true"));
    }

    std::string token =  tokenBuilder.sign(jwt::algorithm::rs256(publicKey.toUtf8().constData(),privateKey.toUtf8().constData(),"",""));

    return QString::fromStdString(token);
}
