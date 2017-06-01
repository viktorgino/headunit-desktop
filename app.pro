TEMPLATE = app
TARGET=headunit-app
QT += qml quick multimedia multimediawidgets sql
CONFIG += c++11 link_pkgconfig welleio

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
    QT += network qml quick charts multimedia

    Release: QMAKE_CFLAGS	+=  -flto -ffast-math -O3
    Release: QMAKE_CXXFLAGS	+=  -flto -ffast-math -O3
    Release: QMAKE_LFLAGS	+= -flto -O3

    RESOURCES += \
        $$PWD/dab-gui/dab_gui.qrc

    DEPENDPATH += $$PWD/welle.io \
                  $$PWD/welle.io/src \
                  $$PWD/welle.io/src/ofdm \
                  $$PWD/welle.io/src/backend \
                  $$PWD/welle.io/src/backend/audio \
                  $$PWD/welle.io/src/backend/data \
                  $$PWD/welle.io/src/output \
                  $$PWD/welle.io/src/various \
                  $$PWD/welle.io/src/input \
                  $$PWD/welle.io/src/ofdm \
                  $$PWD/welle.io/src/backend \
                  $$PWD/welle.io/src/backend/audio \
                  $$PWD/welle.io/src/backend/data \
                  $$PWD/welle.io/src/output \
                  $$PWD/welle.io/src/various \
                  $$PWD/welle.io/src/gui

    INCLUDEPATH += $$PWD/welle.io \
                   $$PWD/welle.io/src \
                   $$PWD/welle.io/src/ofdm \
                   $$PWD/welle.io/src/backend \
                   $$PWD/welle.io/src/backend/audio \
                   $$PWD/welle.io/src/backend/data \
                   $$PWD/welle.io/src/backend/data/journaline \
                   $$PWD/welle.io/src/output \
                   $$PWD/welle.io/src/various \
                   $$PWD/welle.io/src/input \
                   $$PWD/welle.io/src/gui

    HEADERS += $$PWD/welle.io/src/DabConstants.h \
               $$PWD/welle.io/src/ofdm/ofdm-processor.h \
               $$PWD/welle.io/src/ofdm/ofdm-decoder.h \
               $$PWD/welle.io/src/ofdm/phasereference.h \
               $$PWD/welle.io/src/ofdm/phasetable.h \
               $$PWD/welle.io/src/ofdm/freq-interleaver.h \
               $$PWD/welle.io/src/backend/viterbi.h \
               $$PWD/welle.io/src/backend/fic-handler.h \
               $$PWD/welle.io/src/backend/msc-handler.h \
               $$PWD/welle.io/src/backend/fib-processor.h  \
               $$PWD/welle.io/src/backend/galois.h \
               $$PWD/welle.io/src/backend/reed-solomon.h \
               $$PWD/welle.io/src/backend/charsets.h \
               $$PWD/welle.io/src/backend/firecode-checker.h \
               $$PWD/welle.io/src/backend/dab-processor.h \
               $$PWD/welle.io/src/backend/dab-virtual.h \
               $$PWD/welle.io/src/backend/audio/dab-audio.h \
               $$PWD/welle.io/src/backend/audio/mp2processor.h \
               $$PWD/welle.io/src/backend/audio/mp4processor.h \
               $$PWD/welle.io/src/backend/audio/faad-decoder.h \
               $$PWD/welle.io/src/backend/audio/neaacdec.h \
               $$PWD/welle.io/src/backend/data/pad_decoder.h \
               $$PWD/welle.io/src/backend/data/mot_manager.h \
               $$PWD/welle.io/src/backend/data/pad_decoder_adapter.h \
               $$PWD/welle.io/src/backend/tools.h \
               $$PWD/welle.io/src/backend/protection.h \
               $$PWD/welle.io/src/backend/eep-protection.h \
               $$PWD/welle.io/src/backend/uep-protection.h \
               $$PWD/welle.io/src/output/CAudio.h \
               $$PWD/welle.io/src/various/fft.h \
               $$PWD/welle.io/src/various/ringbuffer.h \
               $$PWD/welle.io/src/various/Xtan2.h \
               $$PWD/welle.io/src/input/CVirtualInput.h \
               $$PWD/welle.io/src/input/CInputFactory.h \
               $$PWD/welle.io/src/input/CNullDevice.h \
               $$PWD/welle.io/src/input/CRAWFile.h \
               $$PWD/welle.io/src/input/CRTL_TCP_Client.h \
               $$PWD/welle.io/src/MathHelper.h \
               $$PWD/welle.io/src/gui/CMOTImageProvider.h \
               $$PWD/welle.io/src/gui/CStationList.h \
               $$PWD/welle.io/src/gui/CGUI.h \
               $$PWD/welle.io/src/CRadioController.h \
               $$PWD/welle.io/src/CChannels.h \
               $$PWD/dab-gui/dabHelper.h

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
               $$PWD/welle.io/src/backend/data/pad_decoder.cpp \
               $$PWD/welle.io/src/backend/data/mot_manager.cpp \
               $$PWD/welle.io/src/backend/data/pad_decoder_adapter.cpp \
               $$PWD/welle.io/src/backend/tools.cpp \
               $$PWD/welle.io/src/output/CAudio.cpp \
               $$PWD/welle.io/src/various/fft.cpp \
               $$PWD/welle.io/src/various/Xtan2.cpp \
               $$PWD/welle.io/src/input/CInputFactory.cpp \
               $$PWD/welle.io/src/input/CNullDevice.cpp \
               $$PWD/welle.io/src/input/CRAWFile.cpp \
               $$PWD/welle.io/src/input/CRTL_TCP_Client.cpp \
               $$PWD/welle.io/src/DabConstants.cpp \
               $$PWD/welle.io/src/gui/CMOTImageProvider.cpp \
               $$PWD/welle.io/src/gui/CStationList.cpp \
               $$PWD/welle.io/src/gui/CGUI.cpp \
               $$PWD/welle.io/src/CRadioController.cpp \
               $$PWD/welle.io/src/CChannels.cpp \
               $$PWD/dab-gui/dabHelper.cpp

    unix:!macx:!android: {
        INCLUDEPATH	+= /usr/local/include
        LIBS    += -lfftw3f
        LIBS    += -lusb-1.0
        LIBS    += -ldl
        LIBS    += -lfaad
        #CONFIG  += airspy
        CONFIG  += rtl_sdr
        #CONFIG  += soapysdr

        #CONFIG  += kiss_fft_builtin
        #CONFIG  += libfaad_builtin
    }


    win32 {
        INCLUDEPATH += ../welle.io-win-libs/include
        LIBS    += -L../welle.io-win-libs/x86
        LIBS    += -lfftw3f-3
        LIBS    += -lole32
        LIBS    += -lwinpthread
        LIBS    += -lwinmm
        LIBS    += -lstdc++
        LIBS    += -lws2_32
        LIBS    += -llibfaad
        LIBS    += -lusb-1.0
        CONFIG  += rtl_sdr
        CONFIG  += airspy
    }

    macx {
        INCLUDEPATH	+= /opt/local/include
        LIBS    += -L/opt/local/lib
        LIBS    += -lfftw3f
        LIBS    += -lusb-1.0
        LIBS    += -ldl
        LIBS    += -lfaad
        CONFIG  += airspy
        CONFIG  += rtl_sdr
        #CONFIG  += soapysdr        # not tested
    }

    #### Built-in libraries ####
    kiss_fft_builtin {
        DEFINES   += KISSFFT

        INCLUDEPATH += $$PWD/welle.io/src/libs/kiss_fft

        HEADERS    += \
        $$PWD/welle.io/src/libs/kiss_fft/kiss_fft.h \
        $$PWD/welle.io/src/libs/kiss_fft/_kiss_fft_guts.h

        SOURCES    += $$PWD/welle.io/src/libs/kiss_fft/kiss_fft.c
    }

    libfaad_builtin {
        DEFINES += HAVE_CONFIG_H

        # Dangerous but libfaad produces a lot of warnings
        QMAKE_CFLAGS += -Wno-unused-parameter
        QMAKE_CFLAGS += -Wno-unused-function
        QMAKE_CFLAGS += -Wno-unused-variable
        QMAKE_CFLAGS += -Wno-unused-but-set-variable
        QMAKE_CFLAGS += -Wno-old-style-declaration
        QMAKE_CFLAGS += -Wno-missing-braces

        INCLUDEPATH += \
        $$PWD/welle.io/src/libs/faad2 \
        $$PWD/welle.io/src/libs/faad2/libfaad \
        $$PWD/welle.io/src/libs/faad2/libfaad/codebook \
        $$PWD/welle.io/src/libs/faad2/include

        HEADERS += \
        $$PWD/welle.io/src/libs/faad2/config.h \
        $$PWD/welle.io/src/libs/faad2/include/faad.h \
        $$PWD/welle.io/src/libs/faad2/include/neaacdec.h \
        $$PWD/welle.io/src/libs/faad2/libfaad/analysis.h \
        $$PWD/welle.io/src/libs/faad2/libfaad/bits.h \
        $$PWD/welle.io/src/libs/faad2/libfaad/cfft.h \
        $$PWD/welle.io/src/libs/faad2/libfaad/cfft_tab.h \
        $$PWD/welle.io/src/libs/faad2/libfaad/common.h \
        $$PWD/welle.io/src/libs/faad2/libfaad/drc.h \
        $$PWD/welle.io/src/libs/faad2/libfaad/drm_dec.h \
        $$PWD/welle.io/src/libs/faad2/libfaad/error.h \
        $$PWD/welle.io/src/libs/faad2/libfaad/filtbank.h \
        $$PWD/welle.io/src/libs/faad2/libfaad/fixed.h \
        $$PWD/welle.io/src/libs/faad2/libfaad/huffman.h \
        $$PWD/welle.io/src/libs/faad2/libfaad/ic_predict.h \
        $$PWD/welle.io/src/libs/faad2/libfaad/iq_table.h \
        $$PWD/welle.io/src/libs/faad2/libfaad/is.h \
        $$PWD/welle.io/src/libs/faad2/libfaad/kbd_win.h \
        $$PWD/welle.io/src/libs/faad2/libfaad/lt_predict.h \
        $$PWD/welle.io/src/libs/faad2/libfaad/mdct.h \
        $$PWD/welle.io/src/libs/faad2/libfaad/mdct_tab.h \
        $$PWD/welle.io/src/libs/faad2/libfaad/mp4.h \
        $$PWD/welle.io/src/libs/faad2/libfaad/ms.h \
        $$PWD/welle.io/src/libs/faad2/libfaad/output.h \
        $$PWD/welle.io/src/libs/faad2/libfaad/pns.h \
        $$PWD/welle.io/src/libs/faad2/libfaad/ps_dec.h \
        $$PWD/welle.io/src/libs/faad2/libfaad/ps_tables.h \
        $$PWD/welle.io/src/libs/faad2/libfaad/pulse.h \
        $$PWD/welle.io/src/libs/faad2/libfaad/rvlc.h \
        $$PWD/welle.io/src/libs/faad2/libfaad/sbr_dct.h \
        $$PWD/welle.io/src/libs/faad2/libfaad/sbr_dec.h \
        $$PWD/welle.io/src/libs/faad2/libfaad/sbr_e_nf.h \
        $$PWD/welle.io/src/libs/faad2/libfaad/sbr_fbt.h \
        $$PWD/welle.io/src/libs/faad2/libfaad/sbr_hfadj.h \
        $$PWD/welle.io/src/libs/faad2/libfaad/sbr_hfgen.h \
        $$PWD/welle.io/src/libs/faad2/libfaad/sbr_huff.h \
        $$PWD/welle.io/src/libs/faad2/libfaad/sbr_noise.h \
        $$PWD/welle.io/src/libs/faad2/libfaad/sbr_qmf_c.h \
        $$PWD/welle.io/src/libs/faad2/libfaad/sbr_qmf.h \
        $$PWD/welle.io/src/libs/faad2/libfaad/sbr_syntax.h \
        $$PWD/welle.io/src/libs/faad2/libfaad/sbr_tf_grid.h \
        $$PWD/welle.io/src/libs/faad2/libfaad/sine_win.h \
        $$PWD/welle.io/src/libs/faad2/libfaad/specrec.h \
        $$PWD/welle.io/src/libs/faad2/libfaad/ssr_fb.h \
        $$PWD/welle.io/src/libs/faad2/libfaad/ssr.h \
        $$PWD/welle.io/src/libs/faad2/libfaad/ssr_ipqf.h \
        $$PWD/welle.io/src/libs/faad2/libfaad/ssr_win.h \
        $$PWD/welle.io/src/libs/faad2/libfaad/structs.h \
        $$PWD/welle.io/src/libs/faad2/libfaad/syntax.h \
        $$PWD/welle.io/src/libs/faad2/libfaad/tns.h \
        $$PWD/welle.io/src/libs/faad2/libfaad/codebook/hcb_10.h \
        $$PWD/welle.io/src/libs/faad2/libfaad/codebook/hcb_11.h \
        $$PWD/welle.io/src/libs/faad2/libfaad/codebook/hcb_1.h \
        $$PWD/welle.io/src/libs/faad2/libfaad/codebook/hcb_2.h \
        $$PWD/welle.io/src/libs/faad2/libfaad/codebook/hcb_3.h \
        $$PWD/welle.io/src/libs/faad2/libfaad/codebook/hcb_4.h \
        $$PWD/welle.io/src/libs/faad2/libfaad/codebook/hcb_5.h \
        $$PWD/welle.io/src/libs/faad2/libfaad/codebook/hcb_6.h \
        $$PWD/welle.io/src/libs/faad2/libfaad/codebook/hcb_7.h \
        $$PWD/welle.io/src/libs/faad2/libfaad/codebook/hcb_8.h \
        $$PWD/welle.io/src/libs/faad2/libfaad/codebook/hcb_9.h \
        $$PWD/welle.io/src/libs/faad2/libfaad/codebook/hcb.h \
        $$PWD/welle.io/src/libs/faad2/libfaad/codebook/hcb_sf.h

        SOURCES    += \
        $$PWD/welle.io/src/libs/faad2/libfaad/bits.c \
        $$PWD/welle.io/src/libs/faad2/libfaad/cfft.c \
        $$PWD/welle.io/src/libs/faad2/libfaad/common.c \
        $$PWD/welle.io/src/libs/faad2/libfaad/decoder.c \
        $$PWD/welle.io/src/libs/faad2/libfaad/drc.c \
        $$PWD/welle.io/src/libs/faad2/libfaad/drm_dec.c \
        $$PWD/welle.io/src/libs/faad2/libfaad/error.c \
        $$PWD/welle.io/src/libs/faad2/libfaad/filtbank.c \
        $$PWD/welle.io/src/libs/faad2/libfaad/hcr.c \
        $$PWD/welle.io/src/libs/faad2/libfaad/huffman.c \
        $$PWD/welle.io/src/libs/faad2/libfaad/ic_predict.c \
        $$PWD/welle.io/src/libs/faad2/libfaad/is.c \
        $$PWD/welle.io/src/libs/faad2/libfaad/lt_predict.c \
        $$PWD/welle.io/src/libs/faad2/libfaad/mdct.c \
        $$PWD/welle.io/src/libs/faad2/libfaad/mp4.c \
        $$PWD/welle.io/src/libs/faad2/libfaad/ms.c \
        $$PWD/welle.io/src/libs/faad2/libfaad/output.c \
        $$PWD/welle.io/src/libs/faad2/libfaad/pns.c \
        $$PWD/welle.io/src/libs/faad2/libfaad/ps_dec.c \
        $$PWD/welle.io/src/libs/faad2/libfaad/ps_syntax.c \
        $$PWD/welle.io/src/libs/faad2/libfaad/pulse.c \
        $$PWD/welle.io/src/libs/faad2/libfaad/rvlc.c \
        $$PWD/welle.io/src/libs/faad2/libfaad/sbr_dct.c \
        $$PWD/welle.io/src/libs/faad2/libfaad/sbr_dec.c \
        $$PWD/welle.io/src/libs/faad2/libfaad/sbr_e_nf.c \
        $$PWD/welle.io/src/libs/faad2/libfaad/sbr_fbt.c \
        $$PWD/welle.io/src/libs/faad2/libfaad/sbr_hfadj.c \
        $$PWD/welle.io/src/libs/faad2/libfaad/sbr_hfgen.c \
        $$PWD/welle.io/src/libs/faad2/libfaad/sbr_huff.c \
        $$PWD/welle.io/src/libs/faad2/libfaad/sbr_qmf.c \
        $$PWD/welle.io/src/libs/faad2/libfaad/sbr_syntax.c \
        $$PWD/welle.io/src/libs/faad2/libfaad/sbr_tf_grid.c \
        $$PWD/welle.io/src/libs/faad2/libfaad/specrec.c \
        $$PWD/welle.io/src/libs/faad2/libfaad/ssr.c \
        $$PWD/welle.io/src/libs/faad2/libfaad/ssr_fb.c \
        $$PWD/welle.io/src/libs/faad2/libfaad/ssr_ipqf.c \
        $$PWD/welle.io/src/libs/faad2/libfaad/syntax.c \
        $$PWD/welle.io/src/libs/faad2/libfaad/tns.c
    }

    airspy {
        DEFINES    += HAVE_AIRSPY
        HEADERS    += $$PWD/welle.io/src/input/CAirspy.h
        SOURCES    += $$PWD/welle.io/src/input/CAirspy.cpp

        # The same lib for unix and Windows
        LIBS       += -lairspy
    }

    rtl_sdr {
        DEFINES    += HAVE_RTLSDR
        HEADERS    += $$PWD/welle.io/src/input/CRTL_SDR.h
        SOURCES    += $$PWD/welle.io/src/input/CRTL_SDR.cpp

        # The same lib for unix and Windows
        LIBS       += -lrtlsdr
    }

    soapysdr {
        DEFINES    += HAVE_SOAPYSDR
        HEADERS    += s$$PWD/welle.io/rc/input/CSoapySdr.h
        SOURCES    += $$PWD/welle.io/src/input/CSoapySdr.cpp

        # The same lib for unix and Windows
        LIBS       += -lSoapySDR
    }
}
