#include "securityprovider.h"

SecurityProvider::SecurityProvider(const QSharedPointer<Configuration> &config, QObject *parent):
    QObject(parent), configuration(config)
{

}

QString SecurityProvider::getToken(QString userName, QString password, QJsonValue claimsArray, QJsonArray audiences)
{
    auto creationTime = std::chrono::system_clock::now();
    auto expirationTime = creationTime + std::chrono::seconds{3600};

    QList<QString> claims;
    QJsonArray audience;

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
    jwt::builder tokenBuilder = jwt::create<qt_json_traits>()
    .set_type("JWT")
    .set_issuer("securityprovider")
    //TODO maybe sanitize username
    .set_subject(userName.toUtf8().constData())
    .set_audience(audiences)
    .set_issued_at(creationTime)
    .set_expires_at(expirationTime);

    for(QString claim : claims){
        tokenBuilder.set_payload_claim(claim.toUtf8().constData(), jwt::basic_claim<qt_json_traits>(QString("true")));
    }

    QString token =  tokenBuilder.sign(jwt::algorithm::rs256(configuration->getPublicKey().toUtf8().constData(),configuration->getPrivateKey().toUtf8().constData(),"",""));

    return token;
}

QString SecurityProvider::getPublicKey()
{
    return configuration->getPublicKey();
}

QString SecurityProvider::getIssuer()
{
    return "securityprovider";
}
