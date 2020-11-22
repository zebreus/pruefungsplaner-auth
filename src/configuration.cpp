#include "configuration.h"

Configuration::Configuration(const QList<QString> &arguments, QObject *parent) : QObject(parent)
{
    QCommandLineParser parser;
    parser.setApplicationDescription("Pruefungsplaner authentication server");
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption configFileOption("config",
            "Load configuration from <config>.",
            "config");
    parser.addOption(configFileOption);

    QCommandLineOption portOption(QStringList() << "p" << "port",
            "The server will listen on port <port>",
            "port");
    parser.addOption(portOption);

    QCommandLineOption addressOption(QStringList() << "a" << "address",
            "The server will listen on address <address>",
            "address");
    parser.addOption(addressOption);

    QCommandLineOption privateKeyOption("private-key",
            "The private RSA256 key file in .pem format",
            "privatekey");
    parser.addOption(privateKeyOption);

    QCommandLineOption publicKeyOption("public-key",
            "The public RSA256 key file in .pem format",
            "privatekey");
    parser.addOption(publicKeyOption);

    parser.process(arguments);

    address = parser.value(addressOption);

    QString portString = parser.value(portOption);
    if(portString != ""){
        bool ok;
        uint portInt = portString.toUInt(&ok);
        if(!ok){
            failConfiguration("Port " + portString + " is not a number.");
        }else{
            if(portInt > 0 && portInt <= 65535){
                port = portInt;
            }else{
                failConfiguration("Port " + QString(portInt) + " is out of range (1-65535).");
            }
        }
    }

    QString privateKeyFileString = parser.value(privateKeyOption);
    QString publicKeyFileString = parser.value(publicKeyOption);
    if(privateKeyFileString == "" && publicKeyFileString != ""){
        failConfiguration("If you specify a private key file, you also have to specify a public key file");
    }
    if(privateKeyFileString != "" && publicKeyFileString == ""){
        failConfiguration("If you specify a public key file, you also have to specify a private key file");
    }
    if(privateKeyFileString != "" && publicKeyFileString != ""){
        QFile privateKeyFile(privateKeyFileString);
        QFile publicKeyFile(publicKeyFileString);
        readKeys(privateKeyFile, publicKeyFile);
    }

    QString parsedConfigurationFile = parser.value(configFileOption);
    if(parsedConfigurationFile == ""){
        bool found = false;
        for(auto configFilePath : defaultConfigurationFiles){
            QFile configFile(configFilePath);
            if(configFile.exists()){
                loadConfiguration(QFile("./config.toml"));
                found = true;
                break;
            }
        }
        if(!found){
            QString defaultConfigFilesMessage;
            if(defaultConfigurationFiles.size() > 0){
                defaultConfigFilesMessage = "create one at ";
                for(uint i = 0; i < defaultConfigurationFiles.size() - 1; i++){
                    defaultConfigFilesMessage += defaultConfigurationFiles[i];
                    defaultConfigFilesMessage += ", ";
                }
                defaultConfigFilesMessage += defaultConfigurationFiles[defaultConfigurationFiles.size() - 1];
                defaultConfigFilesMessage += " or";
            }
            failConfiguration("No valid configuration file found. You can " + defaultConfigFilesMessage + " specify your configuration with the --config option.");
        }
    }else{
        loadConfiguration(parsedConfigurationFile);
    }

    checkConfiguration();
}

QString Configuration::getAddress() const
{
    return address;
}

quint16 Configuration::getPort() const
{
    return port;
}

QString Configuration::getPublicKey() const
{
    return publicKey;
}

QString Configuration::getPrivateKey() const
{
    return privateKey;
}

void Configuration::loadConfiguration(const QFile&)
{
    return;
}

void Configuration::readKeys(QFile &privateKeyFile, QFile &publicKeyFile)
{
    if(!privateKeyFile.exists()){
        generatePrivateKey(privateKeyFile);
    }
    checkPrivateKey(privateKeyFile);

    if(!publicKeyFile.exists()){
        generatePublicKey(privateKeyFile, publicKeyFile);
    }
    checkPublicKey(publicKeyFile);

    checkKeysAreMatching(privateKeyFile, publicKeyFile);

    if(privateKeyFile.open(QFile::ReadOnly | QFile::Text)){
        privateKey = QTextStream(&privateKeyFile).readAll();
        privateKeyFile.close();
    }else{
        failConfiguration("Failed to read private key from " + privateKeyFile.fileName());
    }

    if(publicKeyFile.open(QFile::ReadOnly | QFile::Text)){
        publicKey = QTextStream(&publicKeyFile).readAll();
        publicKeyFile.close();
    }else{
        failConfiguration("Failed to read public key from " + publicKeyFile.fileName());
    }
}

void Configuration::generatePrivateKey(QFile& privateKeyFile)
{
    if(privateKeyFile.exists()){
        failConfiguration("Cannot generate new private key, because the file " + privateKeyFile.fileName() + " already exists.");
    }

    if(privateKeyFile.open(QIODevice::WriteOnly)){
        privateKeyFile.close();
    }else{
        failConfiguration("Cannot generate new private key, because the file " + privateKeyFile.fileName() + " cannot be wrote.");
    }

    QString generatePrivateKeyCommand("openssl genpkey -algorithm RSA -out %1 -pkeyopt rsa_keygen_bits:2048");

    generatePrivateKeyCommand = generatePrivateKeyCommand.arg(privateKeyFile.fileName());

    int result = system(generatePrivateKeyCommand.toUtf8());
    if(result != 0){
        failConfiguration("Failed to generate private key (Code " + QString(result) + ").");
    }
}

void Configuration::generatePublicKey(QFile &privateKeyFile, QFile &publicKeyFile)
{
    if(!privateKeyFile.exists()){
        failConfiguration("Cannot generate new public key, because there is no private key specified.");
    }

    if(privateKeyFile.open(QIODevice::ReadOnly)){
        privateKeyFile.close();
    }else{
        failConfiguration("Cannot generate new public key, because the private key cannot be read.");
    }

    if(publicKeyFile.open(QIODevice::WriteOnly)){
        publicKeyFile.close();
    }else{
        failConfiguration("Cannot generate new public key, because the file " + publicKeyFile.fileName() + " cannot be wrote.");
    }

    QString generatePublicKeyCommand("openssl rsa -pubout -in %1 -out %2");

    generatePublicKeyCommand = generatePublicKeyCommand.arg(privateKeyFile.fileName()).arg(publicKeyFile.fileName());

    int result = system(generatePublicKeyCommand.toUtf8());
    if(result != 0){
        failConfiguration("Failed to generate public key (Code " + QString(result) + ").");
    }
}

void Configuration::checkPrivateKey(QFile &privateKeyFile)
{
    if(privateKeyFile.exists()){
        failConfiguration("Private key file does not exist " + privateKeyFile.fileName() + ".");
    }

    if(privateKeyFile.open(QIODevice::ReadOnly)){
        privateKeyFile.close();
    }else{
        failConfiguration("Private key file " + privateKeyFile.fileName() + " cannot be read.");
    }

    QString checkPrivateKeyCommand("openssl rsa -in %1 -check");

    checkPrivateKeyCommand = checkPrivateKeyCommand.arg(privateKeyFile.fileName());

    int result = system(checkPrivateKeyCommand.toUtf8());
    if(result != 0){
        failConfiguration("Private key file is invalid (Code " + QString(result) + ").");
    }
}

void Configuration::checkPublicKey(QFile &publicKeyFile)
{
    if(publicKeyFile.exists()){
        failConfiguration("Public key file does not exist " + publicKeyFile.fileName() + ".");
    }

    if(publicKeyFile.open(QIODevice::ReadOnly)){
        publicKeyFile.close();
    }else{
        failConfiguration("Private key file " + publicKeyFile.fileName() + " cannot be read.");
    }

    QString checkPublicKeyCommand("openssl pkey -inform PEM -pubin -in %1 -noout");

    checkPublicKeyCommand = checkPublicKeyCommand.arg(publicKeyFile.fileName());

    int result = system(checkPublicKeyCommand.toUtf8());
    if(result != 0){
        failConfiguration("Public key file is invalid (Code " + QString(result) + ").");
    }
}

void Configuration::checkKeysAreMatching(QFile &privateKeyFile, QFile &publicKeyFile)
{
    QString checkKeysCommand("test \"$(openssl rsa -in '%1' -outform PEM -pubout)\" = \"$(openssl pkey -inform PEM -pubin -in '%2')\"");

    checkKeysCommand = checkKeysCommand.arg(privateKeyFile.fileName()).arg(publicKeyFile.fileName());

    int result = system(checkKeysCommand.toUtf8());
    if(result != 0){
        failConfiguration("The public key seems not to belong to the private key.");
    }
}

void Configuration::failConfiguration(const QString &message) const
{
    QTextStream(stderr) << message << Qt::endl;
    exit(1);
}

void Configuration::warnConfiguration(const QString &message) const
{
    qDebug() << message;
}

void Configuration::checkConfiguration()
{
    if(port == 0){
        failConfiguration("You specified the only invalid port, which is 0.");
    }

    if(!QHostAddress().setAddress(address)){
        failConfiguration("The address " + address + " seems to be invalid.");
    }

    if(!( privateKey.trimmed().startsWith("-----BEGIN PRIVATE KEY-----") && privateKey.trimmed().startsWith("-----END PRIVATE KEY-----") )){
        failConfiguration("Your private key file could be invalid, or this check could be too basic. Anyway you should look into it.");
    }

    if(!( publicKey.trimmed().startsWith("-----BEGIN PUBLIC KEY-----") && publicKey.trimmed().startsWith("-----END PUBLIC KEY-----") )){
        failConfiguration("Your public key file could be invalid, or this check could be too basic. Anyway you should look into it.");
    }

    if(users.size() == 0){
        warnConfiguration("There are no users in your configuration.");
    }

}
