
#!/usr/local/bin/thrift --gen cpp

namespace cpp SecurityProviderThrift

exception InvalidClaim {
1: string message,
}

exception UnauthorizedClaim {
1: string message,
}

exception InvalidAuthorization {
1: string message,
}

service CertificateGenerator {
  void authorize(1:string username, 2:string password),
  void requestClaim(1:string claim),
  string getToken()
}
