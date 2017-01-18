TEMPLATE = app

QT += qml quick
CONFIG += c++11 link_pkgconfig

SOURCES += main.cpp \
    headunit/hu/hu_aad.cpp \
    headunit/hu/hu_aap.cpp \
    headunit/hu/hu_ssl.cpp \
    headunit/hu/hu_tcp.cpp \
    headunit/hu/hu_usb.cpp \
    headunit/hu/hu_uti.cpp \
    headunit/hu/generated.x64/hu.pb.cc \
    headunit.cpp

RESOURCES += qml.qrc

PKGCONFIG += libssl libcrypto libusb-1.0 glib-2.0 gobject-2.0 gstreamer-1.0 gstreamer-app-1.0 Qt5GLib-2.0 Qt5GStreamer-1.0 Qt5GStreamerQuick-1.0 Qt5GStreamerUtils-1.0 protobuf libunwind libudev

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    headunit/hu/hu_aad.h \
    headunit/hu/hu_aap.h \
    headunit/hu/hu_ssl.h \
    headunit/hu/hu_tcp.h \
    headunit/hu/hu_usb.h \
    headunit/hu/hu_uti.h \
    headunit/hu/generated.x64/hu.pb.h \
    headunit.h

INCLUDEPATH +=$$PWD/headunit/hu
INCLUDEPATH +=$$PWD/headunit/hu/generated.x64
