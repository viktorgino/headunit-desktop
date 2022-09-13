TEMPLATE = lib
CONFIG += c++11 plugin link_pkgconfig
QT += quick
TARGET = $$qtLibraryTarget(audio-processor-plugin)
DEFINES += QT_DEPRECATED_WARNINGS
INCLUDEPATH += $${PWD}/../../includes
DESTDIR = $${OUT_PWD}/../../plugins

include("../../config.pri")

target.path = $${PREFIX}/plugins

INSTALLS += target

LIBS += -li2c

SOURCES += \
    audioprocessorplugin.cpp \
    backends/TDA7418/TDA7418.cpp

HEADERS += \
    audioprocessorplugin.h \
    backends/TDA7418/TDA7418.h \
    backends/audioprocessorinterface.h

DISTFILES += \
    config.json

RESOURCES += \
    audio-processor.qrc

