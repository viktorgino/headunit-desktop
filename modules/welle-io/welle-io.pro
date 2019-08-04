TEMPLATE = lib
CONFIG += c++11 plugin
QT += network qml quick charts multimedia
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


DISTFILES += \
    config.json

DEFINES += QT_NO_SYSTEMTRAYICON
include($$PWD/welle.io/src/backend.pri)

INCLUDEPATH += $$PWD/welle.io/src \
               $$PWD/welle.io/src/welle-gui

RESOURCES +=    $$PWD/welle.io/src/welle-gui/resources.qrc

HEADERS += $$PWD/welle.io/src/welle-gui/audio_output.h \
           $$PWD/welle.io/src/welle-gui/debug_output.h \
           $$PWD/welle.io/src/welle-gui/gui_helper.h \
           $$PWD/welle.io/src/welle-gui/mot_image_provider.h \
           $$PWD/welle.io/src/welle-gui/radio_controller.h \
           $$PWD/welle.io/src/welle-gui/waterfallitem.h

SOURCES += $$PWD/welle.io/src/welle-gui/audio_output.cpp \
           $$PWD/welle.io/src/welle-gui/debug_output.cpp \
           $$PWD/welle.io/src/welle-gui/gui_helper.cpp \
           $$PWD/welle.io/src/welle-gui/mot_image_provider.cpp \
           $$PWD/welle.io/src/welle-gui/radio_controller.cpp \
           $$PWD/welle.io/src/welle-gui/waterfallitem.cpp

# Define how to create version.h
welleSed1.commands = find $$PWD/welle.io -type f -name '*.qml' | xargs sed -i \'s/guiHelper/WelleIoPlugin.GUIHelper/g\'

welleSed2.commands = find $$PWD/welle.io -type f -name '*.qml' | xargs sed -i \'s/radioController/WelleIoPlugin.RadioController/g\'

welleSed3.commands = sed -i \'s/<qresource prefix=\"\/\">/<qresource prefix=\"\/WelleIoPluginRes\">/g\' $$PWD/welle.io/src/welle-gui/resources.qrc

#<qresource prefix="/WelleIoPlugin">


QMAKE_EXTRA_TARGETS += welleSed1 welleSed2 welleSed3

PRE_TARGETDEPS += welleSed1 welleSed2 welleSed3
