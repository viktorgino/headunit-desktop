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
    headunit/hu/headunitbluetoothserver.cpp \
    headunit/hu/headunitbluetoothservice.cpp \
    headunit/hu/hu_aad.cpp \
    headunit/hu/hu_aap.cpp \
    headunit/hu/hu_ssl.cpp \
    headunit/hu/hu_tcp.cpp \
    headunit/hu/hu_usb.cpp \
    headunit/hu/hu_uti.cpp \
    headunit/hu/generated.x64/hu.pb.cc \
    headunit/hu/generated.x64/bt.pb.cc \
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
    headunit/hu/headunitbluetoothserver.h \
    headunit/hu/headunitbluetoothservice.h \
    headunit/hu/hu_aad.h \
    headunit/hu/hu_aap.h \
    headunit/hu/hu_ssl.h \
    headunit/hu/hu_tcp.h \
    headunit/hu/hu_usb.h \
    headunit/hu/hu_uti.h \
    headunit/hu/generated.x64/hu.pb.h \
    headunit/hu/generated.x64/bt.pb.h \
    headunit_common.h \
    headuniteventhandler.h \
    headunitmediapipeline.h \
    headunitvideosource.h \
    qgstvideobuffer.h

DISTFILES += \
    config.json \
    aa_settings.ini
