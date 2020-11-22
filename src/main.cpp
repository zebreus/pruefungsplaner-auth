#include <QCoreApplication>
#include <server.h>
#include "securityprovider.h"
#include <QFile>
#include <QTextStream>
#include <QFileInfo>
#include "configuration.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QSharedPointer<Configuration> config(new Configuration(a.arguments()));

    //TODO Add support for host address to server
    jsonrpc::Server<SecurityProvider> server(config->getPort());
    server.setConstructorArguments(config);
    server.startListening();

    return a.exec();
}
