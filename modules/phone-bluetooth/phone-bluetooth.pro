TEMPLATE = lib
CONFIG += c++11 plugin link_pkgconfig
QT += qml quick bluetooth dbus BluezQt
TARGET = $$qtLibraryTarget(phone-bluetooth-plugin)
DEFINES += QT_DEPRECATED_WARNINGS
INCLUDEPATH += ../../includes
DESTDIR = ../../plugins
QMAKE_LIBS += -lKF5BluezQt

PKGCONFIG += qofono-qt5

SOURCES += \
    $$PWD/bluetooth.cpp \
    telephonymanager.cpp \
    bluezagent.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    $$PWD/bluetooth.h \
    telephonymanager.h \
    bluezagent.h

DISTFILES += \
    config.json

RESOURCES += \
    phonebluetooth.qrc

DBUS_INTERFACES += org.bluez.obex.PhonebookAccess1.xml
