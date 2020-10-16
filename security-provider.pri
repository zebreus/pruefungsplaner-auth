test{
    RESOURCES += $$PWD/tests/securityprovider-testdata.qrc
}

LIBS *= -L/usr/lib -lssl -lcrypto
