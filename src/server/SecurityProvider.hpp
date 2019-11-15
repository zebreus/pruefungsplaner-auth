#ifndef SECURITY_PROVIDER_HPP
#define SECURITY_PROVIDER_HPP

#include <string>
#include <map>
#include <SecurityProvider_types.h>
#include "spdlog/spdlog.h"
#include <jwt.h>

using namespace std;
using namespace jwt;

extern string publicKey;
extern string privateKey;

//TODO rename namespace
namespace securityprovider{

class SecurityProvider {
public:
//TODO better comments
//Constructor
  SecurityProvider(const string& id);
//Destructor
  ~SecurityProvider();
//Authorizes this connection
  void authorize(const string& username, const string& password);
//Request a claim to be added to the jwt
  SecurityProviderThrift::ClaimRequestStatus::type requestClaim(const string& claim);
//Get the jwt
  string getToken();
private:
  string id;
  map<string, string> grantedClaims;
  long creationTimestamp;
  string username;
  string password;
};

}

#endif
