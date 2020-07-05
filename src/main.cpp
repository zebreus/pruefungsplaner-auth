#include <QCoreApplication>
#include <server.h>
#include "securityprovider.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QString privateKey = "";
    QString publicKey = "";

    jsonrpc::Server<SecurityProvider> server(9092);
    server.setConstructorArguments(privateKey, publicKey);
    server.startListening();

    return a.exec();
}
