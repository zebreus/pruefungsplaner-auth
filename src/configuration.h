#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <QObject>
#include <QString>
#include <QFile>
#include <QHostAddress>
#include <QTextStream>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include "user.h"
#include <string>
#include <array>
#include <cpptoml.h>

class Configuration : public QObject
{
    Q_OBJECT
private:
    static constexpr std::array defaultConfigurationFiles{"./config.toml",  "/etc/pruefungsplaner-config/config.toml"};
    static constexpr auto defaultAddress = "0.0.0.0";
    static constexpr uint32_t defaultPort = 9092;
    static constexpr auto defaultPrivateKey = "./private_key.pem";
    static constexpr auto defaultPublicKey = "./public_key.pem";
    QString address;
    quint16 port;
    QString privateKey;
    QString publicKey;
    QList<User> users;

public:
    explicit Configuration(const QList<QString>& args, QObject *parent = nullptr);
    QString getAddress() const;
    quint16 getPort() const;
    QString getPublicKey() const;
    QString getPrivateKey() const;
    /**
     *  @brief Gets a user with the specified name.
     *  @param [in] name of the user
     *  @return A User object representing the user
     *
     *  If there is no user registered with the specified name, a User object with no claims and a checkPassword function that will always fail is returned
     */
    const User getUser(const QString& name) const;

private:
    void loadConfiguration(const QFile& );
    void readKeys(QFile& privateKeyFile,  QFile& publicKeyFile);
    void generatePrivateKey(QFile& privateKeyFile);
    void generatePublicKey(QFile &privateKeyFile, QFile &publicKeyFile);
    void checkPrivateKey(QFile& privateKeyFile);
    void checkPublicKey(QFile &publicKeyFile);
    void checkKeysAreMatching(QFile &privateKeyFile, QFile &publicKeyFile);
    void checkConfiguration();
    void failConfiguration(const QString& message) const;
    void warnConfiguration(const QString& message) const;

signals:

};

#endif // CONFIGURATION_H
