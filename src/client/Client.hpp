
#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <sstream>
#include <string>
#include <iostream>
#include <iomanip>
#include <filesystem>
#include <SecurityProvider.h>

//thrift
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/transport/TSocket.h>

//json
#include <nlohmann/json.hpp>

//cxxopts
#include <cxxopts.hpp>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;

using namespace  ::SecurityProviderThrift;

using namespace std;

class Client{
public:
    Client();
    void startCli();
private:
    void connect();
    void authorize();
    void requestClaim();
    void getToken();
    void close();
    
    bool running;
    std::shared_ptr<TTransport> transport;
    std::shared_ptr<SecurityProviderClient> client;
};

int main(int argc, char** argv);

#endif
