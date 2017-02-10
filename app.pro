TEMPLATE = app

QT += qml quick multimedia multimediawidgets sql
CONFIG += c++11 link_pkgconfig

SOURCES += main.cpp \
    headunit/hu/hu_aad.cpp \
    headunit/hu/hu_aap.cpp \
    headunit/hu/hu_ssl.cpp \
    headunit/hu/hu_tcp.cpp \
    headunit/hu/hu_usb.cpp \
    headunit/hu/hu_uti.cpp \
    headunit/hu/generated.x64/hu.pb.cc \
    headunit.cpp \
    usbconnectionlistener.cpp \
    headunit/common/glib_utils.cpp \
    mediascanner.cpp \
    mediadb.cpp \
    medialibrary.cpp

RESOURCES += \
    qml_gui/qml.qrc \
    main.qrc

PKGCONFIG += libssl libcrypto libusb-1.0 glib-2.0 gobject-2.0 gstreamer-1.0 gstreamer-app-1.0 Qt5GLib-2.0 Qt5GStreamer-1.0 Qt5GStreamerQuick-1.0 Qt5GStreamerUtils-1.0 protobuf libunwind libudev taglib

HEADERS += \
    headunit/hu/hu_aad.h \
    headunit/hu/hu_aap.h \
    headunit/hu/hu_ssl.h \
    headunit/hu/hu_tcp.h \
    headunit/hu/hu_usb.h \
    headunit/hu/hu_uti.h \
    headunit/hu/generated.x64/hu.pb.h \
    headunit.h \
    usbconnectionlistener.h \
    headunit/common/glib_utils.h \
    mediascanner.h \
    mediadb.h \
    medialibrary.h

INCLUDEPATH +=$$PWD/headunit/hu
INCLUDEPATH +=$$PWD/headunit/hu/generated.x64
INCLUDEPATH +=$$PWD/headunit/common

