TEMPLATE = lib
CONFIG += c++11 plugin link_pkgconfig
QT += qml quick
TARGET = $$qtLibraryTarget(hvac-plugin)
DEFINES += QT_DEPRECATED_WARNINGS
INCLUDEPATH += $${PWD}/../../includes
DESTDIR = $${OUT_PWD}/../../plugins

include("../../config.pri")

target.path = $${PREFIX}/plugins

INSTALLS += target

SOURCES += \
    hvacplugin.cpp

HEADERS += \
    hvacplugin.h \
    ../../includes/hvac-common.h

DISTFILES += \
    config.json \
    cars/* \
    config.json

RESOURCES += qml.qrc

cars.path = $${OUT_PWD}/cars
cars.files += cars/

INSTALLS += cars

INCLUDEPATH += $${PWD}/plugin

##recursively copy the theme folder
car_files_copy.commands = $(COPY_DIR) $${PWD}/cars $$OUT_PWD
##attach the copy command to make target
first.depends = $(first) car_files_copy
##export variables to global scope
export(first.depends)
export(car_files_copy.commands)
QMAKE_EXTRA_TARGETS += first car_files_copy

