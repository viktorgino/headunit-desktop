TEMPLATE = lib
CONFIG += c++11 plugin link_pkgconfig
QT += quick serialport
TARGET = $$qtLibraryTarget(hud-serial-plugin)
DEFINES += QT_DEPRECATED_WARNINGS
INCLUDEPATH += $${PWD}/../../includes

DESTDIR = $${OUT_PWD}/../../plugins

include("../../config.pri")

target.path = $${PREFIX}/plugins

INSTALLS += target

SOURCES += \
    CRL/HAL/x86/x86-hal.cpp \
    CRL/HUDSerial/HUDSerial.cpp \
    hudserialplugin.cpp

HEADERS += \
    CRL/HAL/hal.h \
    CRL/HAL/x86/x86-hal.h \
    CRL/HAL/hal_common.h \
    CRL/HUDSerial/HUDSerial.h \
    CRL/Protocols/Protocol.h \
    CRL/common.h \
    hudserialplugin.h \
    ../../includes/hvac-common.h

DISTFILES += \
    config.json

RESOURCES += \
    qml.qrc

