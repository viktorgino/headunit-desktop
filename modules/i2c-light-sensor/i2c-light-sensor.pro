TEMPLATE = lib
CONFIG += c++11 plugin link_pkgconfig
QT += quick
TARGET = $$qtLibraryTarget(i2c-light-sensor-plugin)
DEFINES += QT_DEPRECATED_WARNINGS
LIBS += -li2c
QMAKE_CXXFLAGS += -Wno-unused-parameter
INCLUDEPATH += $${PWD}/../../includes
DESTDIR = $${OUT_PWD}/../../plugins

include("../../config.pri")

target.path = $${PREFIX}/plugins

INSTALLS += target

SOURCES += \
    i2c-light-sensor-plugin.cpp

HEADERS += \
    i2c-light-sensor-plugin.h

DISTFILES += \
    config.json

RESOURCES += \
    qml.qrc

sensors.files = $${PWD}/sensors/*
sensors.path = $$PREFIX/modules/i2c-light-sensor/sensors

INSTALLS += sensors
