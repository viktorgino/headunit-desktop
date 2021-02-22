INCLUDEPATH += $${PWD}/QtGstreamer
DISTFILES += \
    QtGstreamer/QGlib/QtGLib-2.0.pc.in \
    QtGstreamer/QGst/CMakeLists.txt \
    QtGstreamer/QGst/QtGStreamer-1.0.pc.in \
    QtGstreamer/QGst/QtGStreamerQuick-1.0.pc.in \
    QtGstreamer/QGst/QtGStreamerUi-1.0.pc.in \
    QtGstreamer/QGst/QtGStreamerUtils-1.0.pc.in \
    QtGstreamer/qml/quick2/QtGStreamerQuick2.json

HEADERS += \
    QtGstreamer/QGst/Quick/VideoItem \
    QtGstreamer/QGst/Quick/VideoSurface \
    QtGstreamer/QGst/Quick/global.h \
    QtGstreamer/QGst/Quick/videoitem.h \
    QtGstreamer/QGst/Quick/videosurface.h \
    QtGstreamer/QGst/Quick/videosurface_p.h \
    QtGstreamer/elements/gstqtvideosink/delegates/basedelegate.h \
    QtGstreamer/elements/gstqtvideosink/delegates/qtquick2videosinkdelegate.h \
    QtGstreamer/elements/gstqtvideosink/gstqtquick2videosink.h \
    QtGstreamer/elements/gstqtvideosink/gstqtvideosinkmarshal.h \
    QtGstreamer/elements/gstqtvideosink/gstqtvideosinkplugin.h \
    QtGstreamer/elements/gstqtvideosink/painters/videomaterial.h \
    QtGstreamer/elements/gstqtvideosink/painters/videonode.h \
    QtGstreamer/elements/gstqtvideosink/utils/bufferformat.h \
    QtGstreamer/elements/gstqtvideosink/utils/utils.h

SOURCES += \
    QtGstreamer/QGst/Quick/videoitem.cpp \
    QtGstreamer/QGst/Quick/videosurface.cpp \
    QtGstreamer/elements/gstqtvideosink/delegates/basedelegate.cpp \
    QtGstreamer/elements/gstqtvideosink/delegates/qtquick2videosinkdelegate.cpp \
    QtGstreamer/elements/gstqtvideosink/gstqtquick2videosink.cpp \
    QtGstreamer/elements/gstqtvideosink/gstqtvideosinkmarshal.c \
    QtGstreamer/elements/gstqtvideosink/painters/videomaterial.cpp \
    QtGstreamer/elements/gstqtvideosink/painters/videonode.cpp \
    QtGstreamer/elements/gstqtvideosink/utils/bufferformat.cpp \
    QtGstreamer/elements/gstqtvideosink/utils/utils.cpp
