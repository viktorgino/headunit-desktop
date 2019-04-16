TEMPLATE = lib
CONFIG += c++11 plugin
QT += quick
TARGET = $$qtLibraryTarget(volume-control-plugin)
DEFINES += QT_DEPRECATED_WARNINGS
INCLUDEPATH += $${PWD}/../../../includes
DESTDIR = $${OUT_PWD}/../../../plugins

SOURCES += \
    $$PWD/volumecontrol.cpp

## Default rules for deployment.
#qnx: target.path = /tmp/$${TARGET}/bin
#else: unix:!android: target.path = /opt/$${TARGET}/bin
#!isEmpty(target.path): INSTALLS += target

HEADERS += \
    $$PWD/volumecontrol.h

RESOURCES += \
    $$PWD/AudioSettings.qrc

DISTFILES += \
    config.json
