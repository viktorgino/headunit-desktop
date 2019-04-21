TEMPLATE = lib
CONFIG += c++11 plugin
QT += quick
TARGET = $$qtLibraryTarget(odb-car-plugin)
DEFINES += QT_DEPRECATED_WARNINGS
INCLUDEPATH += $${PWD}/../../includes
DESTDIR = $${OUT_PWD}/../../plugins

SOURCES += \
    odbcar.cpp \
    odbcontroller.cpp

RESOURCES += qml.qrc

include("../../config.pri")

target.path = $${PREFIX}/plugins

INSTALLS += target

HEADERS += \
    odbcar.h \
    odbcontroller.h

DISTFILES += \
    config.json
