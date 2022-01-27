CONFIG += qtquickcompiler

isEmpty(PREFIX) {
    PREFIX = /opt/headunit-desktop
}

rpi{
DEFINES += RPI
}

HEADERS += \
    $$PWD/includes/mediainterface.h

#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000
