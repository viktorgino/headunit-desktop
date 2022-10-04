TEMPLATE = lib
CONFIG += c++11 plugin link_pkgconfig
QT += quick multimedia
TARGET = $$qtLibraryTarget(reversing-camera)
DEFINES += QT_DEPRECATED_WARNINGS
INCLUDEPATH += $${PWD}/../../includes
DESTDIR = $${OUT_PWD}/../../plugins

PKGCONFIG += glib-2.0 gobject-2.0
PKGCONFIG += gstreamer-1.0 gstreamer-app-1.0 gstreamer-video-1.0

include("../../config.pri")

target.path = $${PREFIX}/plugins

INSTALLS += target

RESOURCES += qml.qrc

SOURCES += \
    reversing-camera.cpp \
    qgstvideobuffer.cpp

HEADERS += \
    reversing-camera.h \
    qgstvideobuffer.h

DISTFILES += \
    config.json

