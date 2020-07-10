#include <QCoreApplication>
#include <server.h>
#include "securityprovider.h"
#include <QFile>
#include <QTextStream>
#include <QFileInfo>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QFileInfo privateKeyPath("../securityProvider/res/private_key.pem");
    QFileInfo publicKeyPath("../securityProvider/res/public_key.pem");

    QFile privateKeyFile(privateKeyPath.absoluteFilePath());
    QFile publicKeyFile(publicKeyPath.absoluteFilePath());

    bool error = false;
    if (!privateKeyFile.open(QFile::ReadOnly | QFile::Text)){
        qDebug() << "Error opening privatekey file: " << privateKeyPath.absoluteFilePath();
        error = true;
    }
    if (!publicKeyFile.open(QFile::ReadOnly | QFile::Text)){
        qDebug() << "Error opening publickey file: " << publicKeyPath.absoluteFilePath();
        error = true;
    }
    if(error){
        exit(1);
    }

    QString privateKey = QTextStream(&privateKeyFile).readAll();
    QString publicKey = QTextStream(&publicKeyFile).readAll();

    jsonrpc::Server<SecurityProvider> server(9092);
    server.setConstructorArguments(privateKey, publicKey);
    server.startListening();

    return a.exec();
}
