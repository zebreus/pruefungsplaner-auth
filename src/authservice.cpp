#include "authservice.h"

AuthService::AuthService(const QSharedPointer<Configuration> &config, QObject *parent):
    QObject(parent), configuration(config)
{

}

QString AuthService::getToken(QString userName, QString password, QJsonValue claimsArray, QJsonArray audiences)
{
    auto creationTime = std::chrono::system_clock::now();
    auto expirationTime = creationTime + std::chrono::seconds{3600};

    QList<QString> claims;
    QJsonArray audience;

    const User& user = configuration->getUser(userName);

    //TODO Maybe do not sign a token at all, if the password does not check
    if(user.checkPassword(password)){
        if(claimsArray.isArray()){
            QJsonArray claimsJsonArray = claimsArray.toArray();
            for(const QJsonValue& claimEntry : claimsJsonArray){
                if(claimEntry.isString()){
                    if(user.checkClaim(claimEntry.toString())){
                        claims.append(claimEntry.toString());
                    }
                }
            }
        }
    }

    //Added registered and public claims
    jwt::builder tokenBuilder = jwt::create<qt_json_traits>()
    .set_type("JWT")
    .set_issuer("pruefungsplaner-auth")
    .set_subject(user.getName().toUtf8().constData())
    //TODO add audiences to config
    .set_audience(audiences)
    .set_issued_at(creationTime)
    .set_expires_at(expirationTime);

    for(QString claim : claims){
        tokenBuilder.set_payload_claim(claim.toUtf8().constData(), jwt::basic_claim<qt_json_traits>(QString("true")));
    }

    QString token =  tokenBuilder.sign(jwt::algorithm::rs256(configuration->getPublicKey().toUtf8().constData(),configuration->getPrivateKey().toUtf8().constData(),"",""));

    return token;
}

QString AuthService::getPublicKey()
{
    return configuration->getPublicKey();
}

QString AuthService::getIssuer()
{
    return "pruefungsplaner-auth";
}
