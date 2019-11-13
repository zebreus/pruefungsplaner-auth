#ifndef SECURITY_PROVIDER_HPP
#define SECURITY_PROVIDER_HPP

#include <string>
#include <SecurityProvider_types.h>
#include "spdlog/spdlog.h"

using namespace std;

//TODO rename namespace
namespace securityprovider{

class SecurityProvider {
public:
  SecurityProvider(const string& id);
  ~SecurityProvider();
  void authorize(const string& username, const string& password);
  SecurityProviderThrift::ClaimRequestStatus::type requestClaim(const string& claim);
  string getToken();
private:
  string id;
};

}

#endif
