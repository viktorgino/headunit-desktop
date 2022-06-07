TEMPLATE = lib
CONFIG += c++11 plugin link_pkgconfig
QT += quick
TARGET = $$qtLibraryTarget(usbconnectionlistener-plugin)
DEFINES += QT_DEPRECATED_WARNINGS
INCLUDEPATH += $${PWD}/../../includes
DESTDIR = $${OUT_PWD}/../../plugins

PKGCONFIG += libusb-1.0

SOURCES += \
    udeveventlistener.cpp \
    usbconnectionlistener.cpp \
    usbconnectionlistenerplugin.cpp

include("../../config.pri")

target.path = $${PREFIX}/plugins

INSTALLS += target

HEADERS += \
    udeveventlistener.h \
    usbconnectionlistener.h \
    usbconnectionlistenerplugin.h

DISTFILES += \
    config.json
