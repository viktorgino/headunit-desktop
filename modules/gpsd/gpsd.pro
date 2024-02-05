TEMPLATE = lib
CONFIG += c++11 plugin link_pkgconfig
QT += quick
LIBS += -lgps
TARGET = $$qtLibraryTarget(gpsd-plugin)
DEFINES += QT_DEPRECATED_WARNINGS
INCLUDEPATH += $${PWD}/../../includes

DESTDIR = $${OUT_PWD}/../../plugins

include("../../config.pri")

target.path = $${PREFIX}/plugins

INSTALLS += target

SOURCES += \
    gpsdplugin.cpp

HEADERS += \
    gpsdplugin.h

DISTFILES += \
    config.json

RESOURCES += \
    qml.qrc
