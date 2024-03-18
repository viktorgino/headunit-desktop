TEMPLATE = lib
CONFIG += plugin link_pkgconfig
QT += quick bluetooth multimedia
TARGET = $$qtLibraryTarget(android-auto-plugin)
DEFINES += QT_DEPRECATED_WARNINGS
INCLUDEPATH += $${PWD}/../../includes
DESTDIR = $${OUT_PWD}/../../plugins

PKGCONFIG += libssl libcrypto libusb-1.0 glib-2.0 gobject-2.0
PKGCONFIG += gstreamer-1.0 gstreamer-app-1.0 gstreamer-video-1.0
PKGCONFIG += protobuf

SOURCES += \
    androidauto.cpp \
    headuniteventhandler.cpp \
    headunitmediapipeline.cpp \
    headunitvideosource.cpp \
    qgstvideobuffer.cpp

RESOURCES += qml.qrc


INCLUDEPATH +=$$PWD/headunit/hu
INCLUDEPATH +=$$PWD/headunit/hu/generated.x64
INCLUDEPATH +=$$PWD/headunit/common

include("../../config.pri")

target.path = $${PREFIX}/plugins

INSTALLS += target

HEADERS += \
    androidauto.h \
    headunit_common.h \
    headuniteventhandler.h \
    headunitmediapipeline.h \
    headunitvideosource.h \
    qgstvideobuffer.h

DISTFILES += \
    config.json \
    aa_settings.ini

SUBDIRS = \
          headunit