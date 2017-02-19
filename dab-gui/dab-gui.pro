TEMPLATE = app
TARGET = dab-gui
QT += network qml quick charts
CONFIG += c++11 link_pkgconfig
RC_ICONS = ../welle.io/gui/QML/images/icon.ico

Release: QMAKE_CFLAGS	+=  -flto -ffast-math -O3
Release: QMAKE_CXXFLAGS	+=  -flto -ffast-math -O3
Release: QMAKE_LFLAGS	+=  -flto -O3

DEFINES	 += GUI_3 # Not necessary after code clean up
DEFINES	 += MOT_BASICS__ # Not necessary after code clean up

RESOURCES += \
    ../welle.io/gui/touch_gui_resource.qrc \
    dab_gui.qrc

DEPENDPATH += ../welle.io \
              ../welle.io/src \
              ../welle.io/includes \
              ../welle.io/src/ofdm \
              ../welle.io/src/backend \
              ../welle.io/src/backend/audio \
              ../welle.io/src/backend/data \
              ../welle.io/src/output \
              ../welle.io/src/various \
              ../welle.io/src/input \
              ../welle.io/includes/ofdm \
              ../welle.io/includes/backend \
              ../welle.io/includes/backend/audio \
              ../welle.io/includes/backend/data \
              ../welle.io/includes/output \
              ../welle.io/includes/various \
              ../welle.io/gui

INCLUDEPATH += ../welle.io \
              ../welle.io/ \
              ../welle.io/src \
              ../welle.io/includes \
              ../welle.io/includes/ofdm \
              ../welle.io/includes/backend \
              ../welle.io/includes/backend/audio \
              ../welle.io/includes/backend/data \
              ../welle.io/includes/backend/data/journaline \
              ../welle.io/includes/output \
              ../welle.io/includes/various \
              ../welle.io/src/input \
              ../welle.io/gui

# Input
HEADERS += ../welle.io/includes/dab-constants.h \
            ../welle.io/includes/ofdm/ofdm-processor.h \
            ../welle.io/includes/ofdm/ofdm-decoder.h \
            ../welle.io/includes/ofdm/phasereference.h \
            ../welle.io/includes/ofdm/phasetable.h \
            ../welle.io/includes/ofdm/freq-interleaver.h \
            ../welle.io/includes/backend/viterbi.h \
            ../welle.io/includes/backend/fic-handler.h \
            ../welle.io/includes/backend/msc-handler.h \
            ../welle.io/includes/backend/fib-processor.h  \
            ../welle.io/includes/backend/galois.h \
            ../welle.io/includes/backend/reed-solomon.h \
            ../welle.io/includes/backend/charsets.h \
            ../welle.io/includes/backend/firecode-checker.h \
            ../welle.io/includes/backend/dab-processor.h \
            ../welle.io/includes/backend/dab-virtual.h \
            ../welle.io/includes/backend/audio/dab-audio.h \
            ../welle.io/includes/backend/audio/mp2processor.h \
            ../welle.io/includes/backend/audio/mp4processor.h \
            ../welle.io/includes/backend/audio/faad-decoder.h \
            ../welle.io/includes/backend/data/dab-data.h \
            ../welle.io/includes/backend/data/data-processor.h \
            ../welle.io/includes/backend/data/pad-handler.h \
            ../welle.io/includes/backend/data/virtual-datahandler.h \
            ../welle.io/includes/backend/data/ip-datahandler.h \
            ../welle.io/includes/backend/data/mot-databuilder.h \
            ../welle.io/includes/backend/data/mot-data.h \
            ../welle.io/includes/backend/protection.h \
            ../welle.io/includes/backend/eep-protection.h \
            ../welle.io/includes/backend/uep-protection.h \
            ../welle.io/includes/output/audio-base.h \
            ../welle.io/includes/output/newconverter.h \
            ../welle.io/includes/output/audiosink.h \
            ../welle.io/includes/output/fir-filters.h \
            ../welle.io/includes/various/fft.h \
            ../welle.io/includes/various/ringbuffer.h \
            ../welle.io/includes/various/Xtan2.h \
            ../welle.io/src/input/virtual-input.h \
            ../welle.io/gui/gui.h \
            ../welle.io/gui/motimageprovider.h \
            ../welle.io/gui/stationlist.h \
            ../welle.io/gui/tools.h \
            ../welle.io/gui/fic_decoder.h \
            ../welle.io/gui/pad_decoder.h \
            ../welle.io/gui/mot_manager.h \
            ../welle.io/gui/pad_decoder_adapter.h

SOURCES += ../welle.io/src/ofdm/ofdm-processor.cpp \
            ../welle.io/src/ofdm/ofdm-decoder.cpp \
            ../welle.io/src/ofdm/phasereference.cpp \
            ../welle.io/src/ofdm/phasetable.cpp \
            ../welle.io/src/ofdm/freq-interleaver.cpp \
            ../welle.io/src/backend/viterbi.cpp \
            ../welle.io/src/backend/fic-handler.cpp \
            ../welle.io/src/backend/msc-handler.cpp \
            ../welle.io/src/backend/protection.cpp \
            ../welle.io/src/backend/eep-protection.cpp \
            ../welle.io/src/backend/uep-protection.cpp \
            ../welle.io/src/backend/fib-processor.cpp  \
            ../welle.io/src/backend/galois.cpp \
            ../welle.io/src/backend/reed-solomon.cpp \
            ../welle.io/src/backend/charsets.cpp \
            ../welle.io/src/backend/firecode-checker.cpp \
            ../welle.io/src/backend/dab-virtual.cpp \
            ../welle.io/src/backend/dab-processor.cpp \
            ../welle.io/src/backend/protTables.cpp \
            ../welle.io/src/backend/audio/dab-audio.cpp \
            ../welle.io/src/backend/audio/mp2processor.cpp \
            ../welle.io/src/backend/audio/mp4processor.cpp \
            ../welle.io/src/backend/data/pad-handler.cpp \
            ../welle.io/src/backend/data/dab-data.cpp \
            ../welle.io/src/backend/data/data-processor.cpp \
            ../welle.io/src/backend/data/virtual-datahandler.cpp \
            ../welle.io/src/backend/data/ip-datahandler.cpp \
            ../welle.io/src/backend/data/mot-databuilder.cpp \
            ../welle.io/src/backend/data/mot-data.cpp \
            ../welle.io/src/output/audio-base.cpp \
            ../welle.io/src/output/newconverter.cpp \
            ../welle.io/src/output/audiosink.cpp \
            ../welle.io/src/output/fir-filters.cpp \
            ../welle.io/src/various/fft.cpp \
            ../welle.io/src/various/Xtan2.cpp \
            ../welle.io/src/input/virtual-input.cpp \
            ../welle.io/gui/gui.cpp \
            ../welle.io/gui/motimageprovider.cpp \
            ../welle.io/gui/stationlist.cpp \
            ../welle.io/gui/tools.cpp \
            ../welle.io/gui/fic_decoder.cpp \
            ../welle.io/gui/pad_decoder.cpp \
            ../welle.io/gui/mot_manager.cpp \
            ../welle.io/gui/pad_decoder_adapter.cpp \
    main.cpp

unix {
LIBS		+= -ldl
LIBS		+= -lfaad
PKGCONFIG += fftw3f libusb-1.0 portaudio-2.0 zlib sndfile samplerate

CONFIG		+= dabstick
#CONFIG		+= rtl_tcp
#CONFIG          += rawfiles
}


win32 {
INCLUDEPATH += .../welle.io/welle.io-win-libs/include
LIBS		+= -L.../welle.io/welle.io-win-libs/x86
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
#CONFIG          += rawfiles
}


#### Devices ####
dabstick {
        DEFINES		+= HAVE_DABSTICK
        INCLUDEPATH	+= ../welle.io/src/input/dabstick
        HEADERS		+= ../welle.io/src/input/dabstick/dabstick.h \
                           ../welle.io/src/input/dabstick/dongleselect.h
        SOURCES		+= ../welle.io/src/input/dabstick/dabstick.cpp \
                           ../welle.io/src/input/dabstick/dongleselect.cpp
        FORMS		+= ../welle.io/src/input/dabstick/dabstick-widget.ui
}

rtl_tcp {
        DEFINES		+= HAVE_RTL_TCP
        QT		+= network
        INCLUDEPATH	+= ../welle.io/src/input/rtl_tcp
        HEADERS		+= ../welle.io/src/input/rtl_tcp/rtl_tcp_client.h
        SOURCES		+= ../welle.io/src/input/rtl_tcp/rtl_tcp_client.cpp
}

rawfiles {
        DEFINES		+= HAVE_RAWFILES
        INCLUDEPATH	+= ../welle.io/src/input/rawfiles
        HEADERS		+= ../welle.io/src/input/rawfiles/rawfiles.h \
        SOURCES		+= ../welle.io/src/input/rawfiles/rawfiles.cpp
}
