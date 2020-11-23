# pruefungsplaner authenticatin service
Providing json webtokens via jsonrpc.

## Usage
### Generate the Makefile
You can generate the Makefile with `qmake`
### Generating keys
You can generate a RSA256 key pair by running `make keys`
### Generate the server executable
You can generate the server executable by running `make`
### Start the server
To start the server you need a private and a public key in the correct place (`res/private_key.pem` and `res/public_key.pem`). After starting the server it will listen for websocket connection on port 9092.
### Use the authentication service
The security provider has a [jsonrpc 2.0](https://www.jsonrpc.org/specification) interface providing the functions getToken , getPublicKey and getIssuer.
The function definitions can be found in src/securityprovider.hpp.
### Use the client
For easier usage there is a Client implementation. To use it with qmake include the `client/client.pri` in your qmake project. For examples on how to use the client you can look at the demo [server](https://github.com/Zebreus/security-provider-server) and [client](https://github.com/Zebreus/security-provider-user) implementations
