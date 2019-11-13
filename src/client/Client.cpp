#include "Client.hpp"

Client::Client() : running(false) {
}

void Client::startCli(){
    running = true;
    while(running){
        cout << "client>" << flush;
        string command;
        cin >> command;
        if(command == "connect"){
            connect();
        }else if(command == "authorize"){
            authorize();
        }else if(command == "request"){
            requestClaim();
        }else if(command == "get"){
            getToken();
        }else if(command == "close"){
            close();
        }
    }
}

void Client::connect(){
    string servername;
    int port;
    cout << "serveraddress:" << flush;
    cin >> servername;
    cout << "port:" << flush;
    cin >> port;
    std::shared_ptr<TTransport> _socket(new TSocket(servername, port));
	std::shared_ptr<TTransport> _transport(new TBufferedTransport(_socket));
    transport = _transport;
	std::shared_ptr<TProtocol> _protocol(new TBinaryProtocol(transport));
	std::shared_ptr<SecurityProviderClient> _client( new SecurityProviderClient(_protocol));
    client = _client;
    
    transport->open();
    cout << "opened connection" << endl;
}
void Client::authorize(){
    string user;
    string pass;
    cout << "username:" << flush;
    cin >> user;
    cout << "password:" << flush;
    cin >> pass;
    
    try{
        client->authorize(user, pass);
        cout << "authorized" << endl;
    }catch(InvalidAuthorization e){
        cout << "invalid authorization, connection closed" << endl;
    }catch(...){
        cout << "error, probably not connected" << endl;
    }
}

void Client::requestClaim(){
    string claim;
    cout << "claim:" << flush;
    cin >> claim;
    
    try{
        client->requestClaim(claim);
        cout << "claim granted" << endl;
    }catch(UnauthorizedClaim e){
        cout << "error, not authorized for this claim" << endl;
    }catch(InvalidClaim e){
        cout << "error, claim does not exist" << endl;
    }catch(...){
        cout << "error, probably not connected" << endl;
    }
}

void Client::getToken(){
    string token;
    
    try{
        client->getToken(token);
        cout << "got token:" << endl;
        cout << token << endl;
    }catch(...){
        cout << "error, probably not connected or authorized" << endl;
    }
}

void Client::close(){
    transport->close();
}

int main(int argc, char** argv){
    Client client;
    client.startCli();
}
