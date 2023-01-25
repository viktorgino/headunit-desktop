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

sensors.files = $$PREFIX/modules/i2c-light-sensor/sensors/*

INSTALLS += sensors

##recursively copy the sensors folder
sensor_files_copy.commands = $(COPY_DIR) $${PWD}/sensors $$OUT_PWD
##attach the copy command to make target
first.depends = $(first) sensor_files_copy
##export variables to global scope
export(first.depends)
export(sensor_files_copy.commands)
QMAKE_EXTRA_TARGETS += first sensor_files_copy