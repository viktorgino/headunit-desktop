TEMPLATE = lib
CONFIG += c++11 link_pkgconfig plugin
QT += widgets quick dbus
PKGCONFIG += libcanberra glib-2.0 gconf-2.0 gmodule-2.0 gobject-2.0 gthread-2.0 libpulse-simple libpulse libpulse-mainloop-glib
INCLUDEPATH += ../../../includes
TARGET = $$qtLibraryTarget(volume-control-qml)
DEFINES += QT_DEPRECATED_WARNINGS
DESTDIR = ../../../plugins/qml

SOURCES += \
    $$PWD/plasma-pa/src/card.cpp \
    $$PWD/plasma-pa/src/client.cpp \
    $$PWD/plasma-pa/src/context.cpp \
    $$PWD/plasma-pa/src/debug.cpp \
    $$PWD/plasma-pa/src/device.cpp \
    $$PWD/plasma-pa/src/gconfitem.cpp \
    $$PWD/plasma-pa/src/maps.cpp \
    $$PWD/plasma-pa/src/module.cpp \
    $$PWD/plasma-pa/src/modulemanager.cpp \
    $$PWD/plasma-pa/src/operation.cpp \
    $$PWD/plasma-pa/src/port.cpp \
    $$PWD/plasma-pa/src/profile.cpp \
    $$PWD/plasma-pa/src/pulseaudio.cpp \
    $$PWD/plasma-pa/src/pulseobject.cpp \
    $$PWD/plasma-pa/src/server.cpp \
    $$PWD/plasma-pa/src/sink.cpp \
    $$PWD/plasma-pa/src/sinkinput.cpp \
    $$PWD/plasma-pa/src/source.cpp \
    $$PWD/plasma-pa/src/sourceoutput.cpp \
    $$PWD/plasma-pa/src/stream.cpp \
    $$PWD/plasma-pa/src/streamrestore.cpp \
    $$PWD/plasma-pa/src/volumeobject.cpp \
    $$PWD/globalactioncollection.cpp \
    $$PWD/plugin.cpp \
    $$PWD/volumefeedback.cpp


# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH +=

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    $$PWD/plasma-pa/src/card.h \
    $$PWD/plasma-pa/src/client.h \
    $$PWD/plasma-pa/src/context.h \
    $$PWD/plasma-pa/src/debug.h \
    $$PWD/plasma-pa/src/device.h \
    $$PWD/plasma-pa/src/gconfitem.h \
    $$PWD/plasma-pa/src/maps.h \
    $$PWD/plasma-pa/src/module.h \
    $$PWD/plasma-pa/src/modulemanager.h \
    $$PWD/plasma-pa/src/operation.h \
    $$PWD/plasma-pa/src/port.h \
    $$PWD/plasma-pa/src/profile.h \
    $$PWD/plasma-pa/src/pulseaudio.h \
    $$PWD/plasma-pa/src/pulseobject.h \
    $$PWD/plasma-pa/src/server.h \
    $$PWD/plasma-pa/src/sink.h \
    $$PWD/plasma-pa/src/sinkinput.h \
    $$PWD/plasma-pa/src/source.h \
    $$PWD/plasma-pa/src/sourceoutput.h \
    $$PWD/plasma-pa/src/stream.h \
    $$PWD/plasma-pa/src/streamrestore.h \
    $$PWD/plasma-pa/src/volumeobject.h \
    $$PWD/globalactioncollection.h \
    $$PWD/plugin.h \
    $$PWD/volumefeedback.h

DISTFILES += \
    qmldir \
    config.json
