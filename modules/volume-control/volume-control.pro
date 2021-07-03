TEMPLATE = lib
CONFIG += c++11 link_pkgconfig plugin
QT += quick dbus

PKGCONFIG += libpulse-simple libpulse libpulse-mainloop-glib

TARGET = $$qtLibraryTarget(volume-control-plugin)
DEFINES += QT_DEPRECATED_WARNINGS
INCLUDEPATH += $${PWD}/../../includes
DESTDIR = $${OUT_PWD}/../../plugins

include("../../config.pri")

target.path = $${PREFIX}/plugins

INSTALLS += target

SOURCES += \
    $$PWD/volumecontrol.cpp \
    debug.cpp \
    pulseaudio-qt/src/card.cpp \
    pulseaudio-qt/src/cardport.cpp \
    pulseaudio-qt/src/client.cpp \
    pulseaudio-qt/src/context.cpp \
    pulseaudio-qt/src/device.cpp \
    pulseaudio-qt/src/indexedpulseobject.cpp \
    pulseaudio-qt/src/maps.cpp \
    pulseaudio-qt/src/models.cpp \
    pulseaudio-qt/src/module.cpp \
    pulseaudio-qt/src/operation.cpp \
    pulseaudio-qt/src/port.cpp \
    pulseaudio-qt/src/profile.cpp \
    pulseaudio-qt/src/pulseobject.cpp \
    pulseaudio-qt/src/server.cpp \
    pulseaudio-qt/src/sink.cpp \
    pulseaudio-qt/src/sinkinput.cpp \
    pulseaudio-qt/src/source.cpp \
    pulseaudio-qt/src/sourceoutput.cpp \
    pulseaudio-qt/src/stream.cpp \
    pulseaudio-qt/src/streamrestore.cpp \
    pulseaudio-qt/src/volumeobject.cpp

HEADERS += \
    $$PWD/volumecontrol.h \
    debug.h \
    pulseaudio-qt/src/card.h \
    pulseaudio-qt/src/card_p.h \
    pulseaudio-qt/src/cardport.h \
    pulseaudio-qt/src/client.h \
    pulseaudio-qt/src/client_p.h \
    pulseaudio-qt/src/context.h \
    pulseaudio-qt/src/context_p.h \
    pulseaudio-qt/src/device.h \
    pulseaudio-qt/src/device_p.h \
    pulseaudio-qt/src/indexedpulseobject.h \
    pulseaudio-qt/src/indexedpulseobject_p.h \
    pulseaudio-qt/src/maps.h \
    pulseaudio-qt/src/models.h \
    pulseaudio-qt/src/models_p.h \
    pulseaudio-qt/src/module.h \
    pulseaudio-qt/src/module_p.h \
    pulseaudio-qt/src/operation.h \
    pulseaudio-qt/src/port.h \
    pulseaudio-qt/src/port_p.h \
    pulseaudio-qt/src/profile.h \
    pulseaudio-qt/src/profile_p.h \
    pulseaudio-qt/src/pulseobject.h \
    pulseaudio-qt/src/pulseobject_p.h \
    pulseaudio-qt/src/server.h \
    pulseaudio-qt/src/server_p.h \
    pulseaudio-qt/src/sink.h \
    pulseaudio-qt/src/sink_p.h \
    pulseaudio-qt/src/sinkinput.h \
    pulseaudio-qt/src/sinkinput_p.h \
    pulseaudio-qt/src/source.h \
    pulseaudio-qt/src/source_p.h \
    pulseaudio-qt/src/sourceoutput.h \
    pulseaudio-qt/src/sourceoutput_p.h \
    pulseaudio-qt/src/stream.h \
    pulseaudio-qt/src/stream_p.h \
    pulseaudio-qt/src/streamrestore.h \
    pulseaudio-qt/src/streamrestore_p.h \
    pulseaudio-qt/src/volumeobject.h \
    pulseaudio-qt/src/volumeobject_p.h \
    pulseaudioqt_export.h

QML_IMPORT_PATH += $$OUT_PWD

RESOURCES += \
    $$PWD/AudioSettings.qrc

DISTFILES += \
    config.json \
    pulseaudio-qt/src/CMakeLists.txt \
    pulseaudio-qt/src/Messages.sh
