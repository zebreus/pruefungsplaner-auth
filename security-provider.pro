INCLUDEPATH += $$PWD/src
INCLUDEPATH += $$PWD/libs
INCLUDEPATH += $$PWD/libs/jwt-cpp/include
DEPENDPATH += $$PWD
DEFINES += "DISABLE_PICOJSON"
QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

include($$PWD/libs/qt-jsonrpc-server/qt-jsonrpc-server.pri)

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        src/main.cpp \
        src/securityprovider.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

LIBS += -L/usr/lib -lssl -lcrypto

HEADERS += \
    src/QtJsonTraits.h \
    src/securityprovider.h

# Extra target to generate rsa256 keys
keys.target = keys
keys.commands = \
    mkdir -p res;\
    openssl genpkey -algorithm RSA -out res/private_key.pem -pkeyopt rsa_keygen_bits:2048;\
    openssl rsa -pubout -in res/private_key.pem -out res/public_key.pem
keys.depends =

QMAKE_EXTRA_TARGETS += keys
