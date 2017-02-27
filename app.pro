TEMPLATE = app
TARGET=headunit-app
QT += qml quick multimedia multimediawidgets sql
CONFIG += c++11 link_pkgconfig

SOURCES += main.cpp \
    headunit/hu/hu_aad.cpp \
    headunit/hu/hu_aap.cpp \
    headunit/hu/hu_ssl.cpp \
    headunit/hu/hu_tcp.cpp \
    headunit/hu/hu_usb.cpp \
    headunit/hu/hu_uti.cpp \
    headunit.cpp \
    usbconnectionlistener.cpp \
    headunit/common/glib_utils.cpp \
    mediascanner.cpp \
    mediadb.cpp \
    medialibrary.cpp \
    headunit/hu/generated.x64/hu.pb.cc

RESOURCES += \
    qml_gui/qml.qrc \
    main.qrc

PKGCONFIG += libssl libcrypto libusb-1.0 glib-2.0 gobject-2.0
PKGCONFIG += gstreamer-1.0 gstreamer-app-1.0
PKGCONFIG += Qt5GLib-2.0 Qt5GStreamer-1.0 Qt5GStreamerQuick-1.0 Qt5GStreamerUtils-1.0
PKGCONFIG +=  protobuf libunwind libudev taglib

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

include("qml_gui/quickcross/quickcross.pri")

welleio {
    DEFINES += HAVE_WELLEIO
    DEFINES	+= MOT_BASICS__ # Not necessary after code clean up
    QT += network charts widgets
    CONFIG += dabstick

    Release: QMAKE_CFLAGS	+=  -flto -ffast-math -O3
    Release: QMAKE_CXXFLAGS	+=  -flto -ffast-math -O3
    Release: QMAKE_LFLAGS	+=  -flto -O3

    RESOURCES += \
        $$PWD/welle.io/gui/touch_gui_resource.qrc \
        $$PWD/dab-gui/dab_gui.qrc

    DEPENDPATH += $$PWD/welle.io \
                  $$PWD/welle.io/src \
                  $$PWD/welle.io/includes \
                  $$PWD/welle.io/src/ofdm \
                  $$PWD/welle.io/src/backend \
                  $$PWD/welle.io/src/backend/audio \
                  $$PWD/welle.io/src/backend/data \
                  $$PWD/welle.io/src/output \
                  $$PWD/welle.io/src/various \
                  $$PWD/welle.io/src/input \
                  $$PWD/welle.io/includes/ofdm \
                  $$PWD/welle.io/includes/backend \
                  $$PWD/welle.io/includes/backend/audio \
                  $$PWD/welle.io/includes/backend/data \
                  $$PWD/welle.io/includes/output \
                  $$PWD/welle.io/includes/various \
                  $$PWD/welle.io/gui

    INCLUDEPATH += $$PWD/welle.io \
                   $$PWD/welle.io/ \
                   $$PWD/welle.io/src \
                   $$PWD/welle.io/includes \
                   $$PWD/welle.io/includes/ofdm \
                   $$PWD/welle.io/includes/backend \
                   $$PWD/welle.io/includes/backend/audio \
                   $$PWD/welle.io/includes/backend/data \
                   $$PWD/welle.io/includes/backend/data/journaline \
                   $$PWD/welle.io/includes/output \
                   $$PWD/welle.io/includes/various \
                   $$PWD/welle.io/src/input \
                   $$PWD/welle.io/gui

    HEADERS += $$PWD/welle.io/includes/dab-constants.h \
               $$PWD/welle.io/includes/ofdm/ofdm-processor.h \
               $$PWD/welle.io/includes/ofdm/ofdm-decoder.h \
               $$PWD/welle.io/includes/ofdm/phasereference.h \
               $$PWD/welle.io/includes/ofdm/phasetable.h \
               $$PWD/welle.io/includes/ofdm/freq-interleaver.h \
               $$PWD/welle.io/includes/backend/viterbi.h \
               $$PWD/welle.io/includes/backend/fic-handler.h \
               $$PWD/welle.io/includes/backend/msc-handler.h \
               $$PWD/welle.io/includes/backend/fib-processor.h  \
               $$PWD/welle.io/includes/backend/galois.h \
               $$PWD/welle.io/includes/backend/reed-solomon.h \
               $$PWD/welle.io/includes/backend/charsets.h \
               $$PWD/welle.io/includes/backend/firecode-checker.h \
               $$PWD/welle.io/includes/backend/dab-processor.h \
               $$PWD/welle.io/includes/backend/dab-virtual.h \
               $$PWD/welle.io/includes/backend/audio/dab-audio.h \
               $$PWD/welle.io/includes/backend/audio/mp2processor.h \
               $$PWD/welle.io/includes/backend/audio/mp4processor.h \
               $$PWD/welle.io/includes/backend/audio/faad-decoder.h \
               $$PWD/welle.io/includes/backend/data/dab-data.h \
               $$PWD/welle.io/includes/backend/data/data-processor.h \
               $$PWD/welle.io/includes/backend/data/pad-handler.h \
               $$PWD/welle.io/includes/backend/data/virtual-datahandler.h \
               $$PWD/welle.io/includes/backend/data/ip-datahandler.h \
               $$PWD/welle.io/includes/backend/data/mot-databuilder.h \
               $$PWD/welle.io/includes/backend/data/mot-data.h \
               $$PWD/welle.io/includes/backend/protection.h \
               $$PWD/welle.io/includes/backend/eep-protection.h \
               $$PWD/welle.io/includes/backend/uep-protection.h \
               $$PWD/welle.io/includes/output/audio-base.h \
               $$PWD/welle.io/includes/output/newconverter.h \
               $$PWD/welle.io/includes/output/audiosink.h \
               $$PWD/welle.io/includes/output/fir-filters.h \
               $$PWD/welle.io/includes/various/fft.h \
               $$PWD/welle.io/includes/various/ringbuffer.h \
               $$PWD/welle.io/includes/various/Xtan2.h \
               $$PWD/welle.io/src/input/virtual-input.h \
               $$PWD/welle.io/gui/gui.h \
               $$PWD/welle.io/gui/motimageprovider.h \
               $$PWD/welle.io/gui/stationlist.h \
               $$PWD/welle.io/gui/tools.h \
               $$PWD/welle.io/gui/fic_decoder.h \
               $$PWD/welle.io/gui/pad_decoder.h \
               $$PWD/welle.io/gui/mot_manager.h \
               $$PWD/welle.io/gui/pad_decoder_adapter.h

    SOURCES += $$PWD/welle.io/src/ofdm/ofdm-processor.cpp \
               $$PWD/welle.io/src/ofdm/ofdm-decoder.cpp \
               $$PWD/welle.io/src/ofdm/phasereference.cpp \
               $$PWD/welle.io/src/ofdm/phasetable.cpp \
               $$PWD/welle.io/src/ofdm/freq-interleaver.cpp \
               $$PWD/welle.io/src/backend/viterbi.cpp \
               $$PWD/welle.io/src/backend/fic-handler.cpp \
               $$PWD/welle.io/src/backend/msc-handler.cpp \
               $$PWD/welle.io/src/backend/protection.cpp \
               $$PWD/welle.io/src/backend/eep-protection.cpp \
               $$PWD/welle.io/src/backend/uep-protection.cpp \
               $$PWD/welle.io/src/backend/fib-processor.cpp  \
               $$PWD/welle.io/src/backend/galois.cpp \
               $$PWD/welle.io/src/backend/reed-solomon.cpp \
               $$PWD/welle.io/src/backend/charsets.cpp \
               $$PWD/welle.io/src/backend/firecode-checker.cpp \
               $$PWD/welle.io/src/backend/dab-virtual.cpp \
               $$PWD/welle.io/src/backend/dab-processor.cpp \
               $$PWD/welle.io/src/backend/protTables.cpp \
               $$PWD/welle.io/src/backend/audio/dab-audio.cpp \
               $$PWD/welle.io/src/backend/audio/mp2processor.cpp \
               $$PWD/welle.io/src/backend/audio/mp4processor.cpp \
               $$PWD/welle.io/src/backend/data/pad-handler.cpp \
               $$PWD/welle.io/src/backend/data/dab-data.cpp \
               $$PWD/welle.io/src/backend/data/data-processor.cpp \
               $$PWD/welle.io/src/backend/data/virtual-datahandler.cpp \
               $$PWD/welle.io/src/backend/data/ip-datahandler.cpp \
               $$PWD/welle.io/src/backend/data/mot-databuilder.cpp \
               $$PWD/welle.io/src/backend/data/mot-data.cpp \
               $$PWD/welle.io/src/output/audio-base.cpp \
               $$PWD/welle.io/src/output/newconverter.cpp \
               $$PWD/welle.io/src/output/audiosink.cpp \
               $$PWD/welle.io/src/output/fir-filters.cpp \
               $$PWD/welle.io/src/various/fft.cpp \
               $$PWD/welle.io/src/various/Xtan2.cpp \
               $$PWD/welle.io/src/input/virtual-input.cpp \
               $$PWD/welle.io/gui/gui.cpp \
               $$PWD/welle.io/gui/motimageprovider.cpp \
               $$PWD/welle.io/gui/stationlist.cpp \
               $$PWD/welle.io/gui/tools.cpp \
               $$PWD/welle.io/gui/fic_decoder.cpp \
               $$PWD/welle.io/gui/pad_decoder.cpp \
               $$PWD/welle.io/gui/mot_manager.cpp \
               $$PWD/welle.io/gui/pad_decoder_adapter.cpp

    unix {
        LIBS		+= -ldl
        LIBS		+= -lfaad
        PKGCONFIG += fftw3f libusb-1.0 portaudio-2.0 zlib sndfile samplerate

        CONFIG		+= dabstick
        #CONFIG		+= rtl_tcp
        #CONFIG          += rawfiles
    }


    win32 {
        INCLUDEPATH += .$$PWD/welle.io/welle.io-win-libs/include
        LIBS		+= -L.$$PWD/welle.io/welle.io-win-libs/x86
        LIBS		+= -lfftw3f-3
        LIBS		+= -lportaudio_x86
        LIBS		+= -lole32
        LIBS		+= -lwinpthread
        LIBS		+= -lwinmm
        LIBS 		+= -lstdc++
        LIBS		+= -lws2_32
        LIBS		+= -llibfaad
        LIBS		+= -lusb-1.0
        LIBS		+= -llibsamplerate
        CONFIG		+= rtl_tcp
        #CONFIG		+= dabstick
        #CONFIG         += rawfiles
    }

    dabstick {
        DEFINES		+= HAVE_DABSTICK
        INCLUDEPATH	+= $$PWD/welle.io/src/input/dabstick
        HEADERS		+= $$PWD/welle.io/src/input/dabstick/dabstick.h \
                           $$PWD/welle.io/src/input/dabstick/dongleselect.h
        SOURCES		+= $$PWD/welle.io/src/input/dabstick/dabstick.cpp \
                           $$PWD/welle.io/src/input/dabstick/dongleselect.cpp
        FORMS		+= $$PWD/welle.io/src/input/dabstick/dabstick-widget.ui
    }
}
