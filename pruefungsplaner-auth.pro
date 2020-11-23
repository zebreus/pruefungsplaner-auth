INCLUDEPATH += $$PWD/src
INCLUDEPATH += $$PWD/libs
INCLUDEPATH += $$PWD/libs/jwt-cpp/include
INCLUDEPATH += $$PWD/libs/cpptoml/include
DEPENDPATH += $$PWD
DEFINES += "DISABLE_PICOJSON"
QT -= gui

CONFIG += c++2a console
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
        src/authservice.cpp \
        src/configuration.cpp \
        src/main.cpp \
        src/user.cpp

unix{
    # Install executable
    target.path = /usr/bin

    # Install default config file
    config.path = /etc/$${TARGET}/
    config.files = res/config.toml

    # Create data directory
    datadir.path = /usr/share/$${TARGET}
    datadir.extra = " "
    datadir.uninstall = " "

    # Create directory for keys
    keys.path = $${datadir.path}/keys
    keys.extra = " "
    keys.uninstall = " "
}

!isEmpty(target.path): INSTALLS += target
!isEmpty(config.path): INSTALLS += config
!isEmpty(keys.path): INSTALLS += keys
!isEmpty(datadir.path): INSTALLS += datadir


LIBS += -L/usr/lib -lssl -lcrypto

HEADERS += \
    src/QtJsonTraits.h \
    src/authservice.h \
    src/configuration.h \
    src/user.h
