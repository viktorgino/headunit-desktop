TEMPLATE = lib
CONFIG += c++11 plugin link_pkgconfig
QT += quick
TARGET = $$qtLibraryTarget(navit-plugin)
DEFINES += QT_DEPRECATED_WARNINGS
INCLUDEPATH += $${PWD}/../../includes
DESTDIR = $${OUT_PWD}/../../plugins
INCLUDEPATH += $$PWD/navit/navit
DEPENDPATH += $$PWD/navit/navit

include("../../config.pri")

target.path = $${PREFIX}/plugins

INSTALLS += target

SOURCES += \
    navitplugin.cpp

HEADERS += \
    navitplugin.h

DISTFILES += \
    config.json

DISTFILES += \
    navit.xml \
    navit_layout_bike.xml \
    navit_layout_car.xml \
    navit_layout_car_android.xml \
    navit_layout_car_dark.xml \
    navit_layout_car_simple.xml
