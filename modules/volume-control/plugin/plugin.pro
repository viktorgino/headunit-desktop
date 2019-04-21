TEMPLATE = lib
CONFIG += c++11 plugin
QT += quick
TARGET = $$qtLibraryTarget(volume-control-plugin)
DEFINES += QT_DEPRECATED_WARNINGS
INCLUDEPATH += $${PWD}/../../../includes
DESTDIR = $${OUT_PWD}/../../../plugins

include("../../../config.pri")

target.path = $${PREFIX}/plugins

INSTALLS += target

SOURCES += \
    $$PWD/volumecontrol.cpp

HEADERS += \
    $$PWD/volumecontrol.h

RESOURCES += \
    $$PWD/AudioSettings.qrc

DISTFILES += \
    config.json
