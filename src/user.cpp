#include "user.h"

User::User(const QString &name, const QString &password, const QList<QString> &claims) :
    name(name), password(password), claims(claims)
{
}

QString User::getName() const
{
    return name;
}

bool User::checkPassword(const QString &password) const
{
    // An empty password will always fail
    if(this->password == ""){
        return false;
    }

    if(this->password == password){
        return true;
    }else{
        return false;
    }
}

bool User::checkClaim(const QString &claim) const
{
    if(claims.indexOf(claim) != -1){
        return true;
    }else{
        return false;
    }
}
