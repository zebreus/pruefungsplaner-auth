#ifndef USER_H
#define USER_H

#include <QString>
#include <QList>

class User
{
public:
    /**
     *  @brief Creates a user
     *  @param [in] name of the user
     *  @param [in] password of the created user. If it is empty checkPassword will always fail
     *  @param [in] claims is a list of strings representing all the claims the user can get
     */
    User(const QString& name, QString& password, const QList<QString>& claims);
    QString getName() const;
    bool checkPassword(const QString& password) const;
    bool checkClaim(const QString& claim) const;
private:
    const QString name;
    const QString password;
    const QList<QString> claims;
};

#endif // USER_H
