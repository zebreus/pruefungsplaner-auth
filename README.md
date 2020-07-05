# securityProvider
Providing json webtokens via thrift.

## Building
### Server
To build the server executable just run `make server` .
This will create a server executable in the out directory.

### Demo cli client
To build the cli client executable just run `make client` .
This will create the client executable in the out directory.

## Usage
### Generating keys
You can generate a pair of RS256 keys into the res directory by running
```sh
make keys
```
You can also use your own keys, just specify them when starting the server

### Start server
To start the server run
```sh
./out/server -p 9092 --public_key res/public_key.pem --private_key res/private_key.pem
```
To see all options the server is offering run
```sh
./out/server --help
```

### Use the cli client
To start the cli client run
```sh
./out/client
```

You can connect to a server using
```sh
connect <serverip> <serverport>
```

To be able to request claims you need to give the server your login
```sh
authorize <username> <password>
```

Once logged in you can request claims to be added to your token
```sh
request <claim>
```

When you requested all needed claims, you can print your signed token
```sh
get
```