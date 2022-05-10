TEMPLATE = lib
CONFIG += c++11 plugin link_pkgconfig
QT += qml quick
TARGET = $$qtLibraryTarget(hvac-plugin)
DEFINES += QT_DEPRECATED_WARNINGS
INCLUDEPATH += $${PWD}/../../includes
DESTDIR = $${OUT_PWD}/../../plugins

include("../../config.pri")

target.path = $${PREFIX}/plugins

INSTALLS += target

SOURCES += \
    hvacplugin.cpp

HEADERS += \
    hvacplugin.h

DISTFILES += \
    config.json

QML_IMPORT_PATH += $$OUT_PWD

RESOURCES += qml.qrc

INCLUDEPATH += $${PWD}/plugin


