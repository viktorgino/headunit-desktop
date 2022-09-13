TEMPLATE = lib
CONFIG += c++11 plugin link_pkgconfig
QT += quick
TARGET = $$qtLibraryTarget(hud-serial-test)
DEFINES += QT_DEPRECATED_WARNINGS
INCLUDEPATH += $${PWD}/../../includes
DESTDIR = $${OUT_PWD}/../../plugins

include("../../config.pri")

target.path = $${PREFIX}/plugins

INSTALLS += target

SOURCES += \
    hud-serial-test.cpp

HEADERS += \
    hud-serial-test.h \
    ../../includes/hvac-common.h

RESOURCES += qml.qrc

DISTFILES += \
    config.json

