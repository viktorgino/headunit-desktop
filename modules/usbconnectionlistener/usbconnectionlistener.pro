TEMPLATE = lib
CONFIG += c++11 plugin link_pkgconfig
QT += quick
TARGET = $$qtLibraryTarget(usbconnectionlistener-plugin)
DEFINES += QT_DEPRECATED_WARNINGS
INCLUDEPATH += ../../includes
DESTDIR = ../../plugins

PKGCONFIG += libusb-1.0

SOURCES += \
    usbconnectionlistener.cpp \
    usbconnectionlistenerplugin.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    usbconnectionlistener.h \
    usbconnectionlistenerplugin.h

DISTFILES += \
    config.json
