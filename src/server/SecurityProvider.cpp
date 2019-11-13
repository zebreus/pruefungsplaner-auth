#include "SecurityProvider.hpp"

namespace securityprovider{

SecurityProvider::SecurityProvider(const string& id) : id(id) {
    spdlog::info("{}: SecurityProvider({})", id,id);
}

SecurityProvider::~SecurityProvider(){
    spdlog::info("{}: ~SecurityProvider()", id);
}

void SecurityProvider::authorize(const std::string& username, const std::string& password) {
    //TODO dont log usernames and passwords!!!
    spdlog::info("{}: authorize({}, {})", id, username, password);
}

SecurityProviderThrift::ClaimRequestStatus::type SecurityProvider::requestClaim(const string& claim) {
    spdlog::info("{}: requestClaim({})", id, claim);
    return SecurityProviderThrift::ClaimRequestStatus::GRANTED;
}

string SecurityProvider::getToken() {
    spdlog::info("{}: getToken()", id);
    return "THIS_IS_A_TOKEN";
}

}
