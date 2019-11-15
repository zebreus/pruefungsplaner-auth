#include "SecurityProvider.hpp"

namespace securityprovider{

SecurityProvider::SecurityProvider(const string& id) : id(id) {
    spdlog::info("{}: SecurityProvider({})", id, id);
    creationTimestamp = std::chrono::system_clock::now();

}

SecurityProvider::~SecurityProvider(){
    spdlog::info("{}: ~SecurityProvider()", id);
}

void SecurityProvider::authorize(const std::string& username, const std::string& password) {
    //TODO dont log usernames and passwords!!!
    spdlog::info("{}: authorize({}, {})", id, username, password);
    if(username != "" && this->username == ""){
        //TODO maybe check if valid or accept everything and check only when granting
        this->username = username;
        this->password = password;
        spdlog::info("{}: authorized with username {}", id, username);
    }else{
        spdlog::info("{}: tried to authorize again; ignored", id);
    }
}

SecurityProviderThrift::ClaimRequestStatus::type SecurityProvider::requestClaim(const string& claim) {
    spdlog::info("{}: requestClaim({})", id, claim);
    
    auto status = SecurityProviderThrift::ClaimRequestStatus::NONEXISTENT;
    
    if(claim == "claim_A"){
        if(username == "max" && password == "12345"){
            status = SecurityProviderThrift::ClaimRequestStatus::GRANTED;
        }else if(username == "tom" && password == "67890"){
            status = SecurityProviderThrift::ClaimRequestStatus::GRANTED;
        }else{
            status = SecurityProviderThrift::ClaimRequestStatus::UNAUTHORIZED;
        }
    }else if(claim == "claim_B"){
        if(username == "max" && password == "12345"){
            status = SecurityProviderThrift::ClaimRequestStatus::GRANTED;
        }else{
            status = SecurityProviderThrift::ClaimRequestStatus::UNAUTHORIZED;
        }
    }else if(claim == "claim_C"){
        if(username == "tom" && password == "67890"){
            status = SecurityProviderThrift::ClaimRequestStatus::GRANTED;
        }else{
            status = SecurityProviderThrift::ClaimRequestStatus::UNAUTHORIZED;
        }
    }
    
    if(status == SecurityProviderThrift::ClaimRequestStatus::GRANTED){
        spdlog::info("{}: got claim {}", id, claim);
        grantedClaims.insert(std::pair<string,string>(claim,"true"));
    }else if(status == SecurityProviderThrift::ClaimRequestStatus::UNAUTHORIZED){
        spdlog::info("{}: not authorized to get claim {}", id, claim);
    }else{
        spdlog::info("{}: tried to get nonexistent claim {}", id, claim);
    }
    
    return status;
}

string SecurityProvider::getToken() {
    spdlog::info("{}: getToken()", id);
    
    //Added registered and public claims
    jwt::builder tokenBuilder = jwt::create()
    .set_type("JWT")
	.set_issuer("securityprovider")
    //TODO maybe sanitize username
    .set_subject(username)
    //TODO generate random id
    .set_id("random id")
    .set_audience("someone")
	.set_issued_at(creationTimestamp)
	.set_expires_at(creationTimestamp + std::chrono::seconds{3600});
    
    //Add payload claims
    for(std::pair<string,string> grantedClaim: grantedClaims){
        tokenBuilder.set_payload_claim(grantedClaim.first, jwt::claim(grantedClaim.second));
    }
    
    //Generated signed token
    string token = tokenBuilder.sign(jwt::algorithm::rs256(publicKey,privateKey,"",""));
    
    spdlog::info("{} got token: {}", id, token);
    
    return token;
}

}
