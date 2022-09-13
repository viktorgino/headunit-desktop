TEMPLATE = lib
CONFIG += c++11 plugin link_pkgconfig
QT += quick serialport
TARGET = $$qtLibraryTarget(hud-serial-plugin)
DEFINES += QT_DEPRECATED_WARNINGS
INCLUDEPATH += $${PWD}/../../includes

DESTDIR = $${OUT_PWD}/../../plugins

include("../../config.pri")

target.path = $${PREFIX}/plugins

INSTALLS += target

SOURCES += \
    CRL/HAL/x86/x86-hal.cpp \
    CRL/HUDSerial/HUDSerial.cpp \
    hudserialplugin.cpp

HEADERS += \
    CRL/HAL/hal.h \
    CRL/HAL/x86/x86-hal.h \
    CRL/HAL/hal_common.h \
    CRL/HUDSerial/HUDSerial.h \
    CRL/Protocols/Protocol.h \
    CRL/common.h \
    hudserialplugin.h \
    ../../includes/hvac-common.h

DISTFILES += \
    config.json

RESOURCES += \
    qml.qrc

cars.path = $$PREFIX/modules/hud-serial/cars/
cars.files += cars/Volvo/*
cars.files += cars/Sample/*

INSTALLS += cars

##recursively copy the theme folder
car_files_copy.commands = $(COPY_DIR) $${PWD}/cars $$OUT_PWD
##attach the copy command to make target
first.depends = $(first) car_files_copy
##export variables to global scope
export(first.depends)
export(car_files_copy.commands)
QMAKE_EXTRA_TARGETS += first car_files_copy
