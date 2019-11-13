#!/usr/local/bin/thrift --gen cpp

namespace cpp SecurityProviderThrift

enum ClaimRequestStatus {
    GRANTED,
    UNAUTHORIZED,
    NONEXISTENT
} 

exception InvalidAuthorization {
1: string message,
}

exception InternalServerError {
1: string message,
}

service SecurityProvider {
  void authorize(1:string username, 2:string password),
  ClaimRequestStatus requestClaim(1:string claim),
  string getToken()
}
