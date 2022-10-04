TEMPLATE = lib
CONFIG += c++11 plugin link_pkgconfig
QT += quick multimedia
TARGET = $$qtLibraryTarget(reversing-camera)
DEFINES += QT_DEPRECATED_WARNINGS
INCLUDEPATH += $${PWD}/../../includes
DESTDIR = $${OUT_PWD}/../../plugins

include("../../config.pri")

target.path = $${PREFIX}/plugins

INSTALLS += target

RESOURCES += qml.qrc

SOURCES += \
    reversing-camera.cpp

HEADERS += \
    reversing-camera.h

DISTFILES += \
    config.json

