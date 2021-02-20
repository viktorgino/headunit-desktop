TEMPLATE = lib
CONFIG += c++11 plugin link_pkgconfig
QT += quick
TARGET = $$qtLibraryTarget(rpi-plugin)
DEFINES += QT_DEPRECATED_WARNINGS
INCLUDEPATH += $${PWD}/../../includes
DESTDIR = $${OUT_PWD}/../../plugins

include("../../config.pri")

target.path = $${PREFIX}/plugins

INSTALLS += target

SOURCES += \
    rpiplugin.cpp

HEADERS += \
    rpiplugin.h

DISTFILES += \
    config.json

RESOURCES += \
    RPI.qrc

