TEMPLATE = lib
CONFIG += c++11 plugin link_pkgconfig
QT += qml quick bluetooth dbus BluezQt
TARGET = $$qtLibraryTarget(phone-bluetooth-plugin)
DEFINES += QT_DEPRECATED_WARNINGS
INCLUDEPATH += $${PWD}/../../includes
DESTDIR = $${OUT_PWD}/../../plugins
QMAKE_LIBS += -lKF5BluezQt

PKGCONFIG += qofono-qt5

SOURCES += \
    telephonymanager.cpp \
    bluezagent.cpp

include("../../config.pri")

target.path = $${PREFIX}/plugins

INSTALLS += target

HEADERS += \
    telephonymanager.h \
    bluezagent.h

DISTFILES += \
    config.json

RESOURCES += \
    phonebluetooth.qrc

DBUS_INTERFACES += org.bluez.obex.PhonebookAccess1.xml
