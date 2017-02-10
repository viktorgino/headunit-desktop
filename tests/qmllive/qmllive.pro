TEMPLATE = app

QT += qml quick multimedia multimediawidgets sql
CONFIG += c++11 link_pkgconfig

SOURCES += \
    main.cpp \
    ../../headunit.cpp \
    ../../mediadb.cpp \
    ../../medialibrary.cpp \
    ../../mediascanner.cpp \
    ../../usbconnectionlistener.cpp \
    ../../headunit/hu/hu_ssl.cpp \
    ../../headunit/hu/hu_aad.cpp \
    ../../headunit/hu/hu_tcp.cpp \
    ../../headunit/hu/hu_aap.cpp \
    ../../headunit/hu/hu_usb.cpp \
    ../../headunit/hu/hu_uti.cpp \
    ../../headunit/hu/generated.x64/hu.pb.cc \
    ../../headunit/common/glib_utils.cpp

RESOURCES += \
    ../../qml_gui/qml.qrc \
    ../../main.qrc

PKGCONFIG += libssl libcrypto libusb-1.0 glib-2.0 gobject-2.0 gstreamer-1.0 gstreamer-app-1.0 Qt5GLib-2.0 Qt5GStreamer-1.0 Qt5GStreamerQuick-1.0 Qt5GStreamerUtils-1.0 protobuf libunwind libudev taglib qmllive

INCLUDEPATH +=$$PWD/../../
DEPENDPATH +=$$PWD/../../
INCLUDEPATH +=$$PWD/../../headunit/hu
INCLUDEPATH +=$$PWD/../../headunit/hu/generated.x64
INCLUDEPATH +=$$PWD/../../headunit/common
INCLUDEPATH +=$$QT_INSTALL_PREFIX
HEADERS += \
    ../../headunit.h \
    ../../mediadb.h \
    ../../medialibrary.h \
    ../../mediascanner.h \
    ../../usbconnectionlistener.h \
    ../../headunit/hu/hu_uti.h \
    ../../headunit/hu/hu_ssl.h \
    ../../headunit/hu/hu_aad.h \
    ../../headunit/hu/hu_tcp.h \
    ../../headunit/hu/hu_aap.h \
    ../../headunit/hu/hu_usb.h \
    ../../headunit/hu/generated.x64/hu.pb.h \
    ../../headunit/common/glib_utils.h

DISTFILES += \
    ../../headunit/hu/hu.proto

