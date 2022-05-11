CONFIG += qtquickcompiler

isEmpty(PREFIX) {
    PREFIX = /opt/headunit-desktop
}

rpi{
DEFINES += RPI
}

HEADERS += \
    $$PWD/includes/hud-common.h \
    $$PWD/includes/mediainterface.h \
    $$PWD/includes/plugininterface.h \
    $$PWD/includes/hvac-common.h

INCLUDEPATH += \
    $$PWD/includes
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000
