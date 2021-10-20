TEMPLATE = lib
CONFIG += c++11 plugin
QT += network qml quick charts multimedia quickcontrols2 dbus
TARGET = $$qtLibraryTarget(welle-io-plugin)
DEFINES += QT_DEPRECATED_WARNINGS
INCLUDEPATH += $${PWD}/../../includes
DESTDIR = $${OUT_PWD}/../../plugins

SOURCES += \
    welleio.cpp

HEADERS += \
    welleio.h

RESOURCES += \
    $$PWD/qml/dab_gui.qrc

include("../../config.pri")

target.path = $${PREFIX}/plugins

INSTALLS += target

DEFINES += QT_NO_SYSTEMTRAYICON
DEFINES += QUIET_MPG123

DISTFILES += \
    config.json

include($$PWD/welle.io/src/backend.pri)

INCLUDEPATH += $$PWD/welle.io/src \
               $$PWD/welle.io/src/welle-gui

HEADERS += $$PWD/welle.io/src/welle-gui/audio_output.h \
           $$PWD/welle.io/src/welle-gui/debug_output.h \
           $$PWD/welle.io/src/welle-gui/gui_helper.h \
           $$PWD/welle.io/src/welle-gui/mot_image_provider.h \
           $$PWD/welle.io/src/welle-gui/radio_controller.h \
           $$PWD/welle.io/src/welle-gui/waterfallitem.h \
           $$PWD/welle.io/src/welle-gui/mpris/mpris.h \
           $$PWD/welle.io/src/welle-gui/mpris/mpris_mp2.h \
           $$PWD/welle.io/src/welle-gui/mpris/mpris_mp2_player.h

SOURCES += $$PWD/welle.io/src/welle-gui/audio_output.cpp \
           $$PWD/welle.io/src/welle-gui/debug_output.cpp \
           $$PWD/welle.io/src/welle-gui/gui_helper.cpp \
           $$PWD/welle.io/src/welle-gui/mot_image_provider.cpp \
           $$PWD/welle.io/src/welle-gui/radio_controller.cpp \
           $$PWD/welle.io/src/welle-gui/waterfallitem.cpp \
           $$PWD/welle.io/src/welle-gui/mpris/mpris.cpp \
           $$PWD/welle.io/src/welle-gui/mpris/mpris_mp2.cpp \
           $$PWD/welle.io/src/welle-gui/mpris/mpris_mp2_player.cpp
