win32:!defined(packagesExist, test) {
    # Change this to your gstreamer SDK installation path
    GSTREAMER_PATH=C:/gstreamer/1.0/x86

    # pkgconfig dependencies: gstreamer-1.0, gstreamer-video-1.0, gstreamer-gl-1.0

    GSTREAMER_1_0_INCLUDEPATH = $${GSTREAMER_PATH}/include/gstreamer-1.0
    win32-g++:GSTREAMER_1_0_LIBS = -L$${GSTREAMER_PATH}/lib -lgstreamer-1.0
    win32-msvc:GSTREAMER_1_0_LIBS = /LIBPATH:$${GSTREAMER_PATH}/lib gstreamer-1.0.lib

    GSTREAMER_VIDEO_1_0_INCLUDEPATH = $${GSTREAMER_PATH}/include/gstreamer-1.0
    win32-g++:GSTREAMER_VIDEO_1_0_LIBS = -L$${GSTREAMER_PATH}/lib -lgstvideo-1.0
    win32-msvc:GSTREAMER_VIDEO_1_0_LIBS = /LIBPATH:$${GSTREAMER_PATH}/lib gstvideo-1.0.lib

    GSTREAMER_GL_1_0_INCLUDEPATH = $${GSTREAMER_PATH}/include \
                                   $${GSTREAMER_PATH}/lib/gstreamer-1.0/include
    win32-g++:GSTREAMER_GL_1_0_LIBS = -L$${GSTREAMER_PATH}/lib -lgstgl-1.0
    win32-msvc:GSTREAMER_GL_1_0_LIBS = /LIBPATH:$${GSTREAMER_PATH}/lib gstgl-1.0.lib

    # Extra dependencies:
    # glib-2.0, gobject-2.0 (required by gstreamer-1.0)
    # gstreamer-base-1.0 (required by gstreamer-video-1.0)

    GLIB_2_0_INCLUDEPATH = $${GSTREAMER_PATH}/include/glib-2.0 \
                           $${GSTREAMER_PATH}/lib/glib-2.0/include
    win32-g++:GLIB_2_0_LIBS = -L$${GSTREAMER_PATH}/lib -lglib-2.0 -lintl
    win32-msvc:GLIB_2_0_LIBS = /LIBPATH:$${GSTREAMER_PATH}/lib glib-2.0.lib intl.lib

    GOBJECT_2_0_INCLUDEPATH = $${GSTREAMER_PATH}/include
    win32-g++:GOBJECT_2_0_LIBS = -L$${GSTREAMER_PATH}/lib -lgobject-2.0
    win32-msvc:GOBJECT_2_0_LIBS = /LIBPATH:$${GSTREAMER_PATH}/lib gobject-2.0.lib

    GSTREAMER_BASE_1_0_INCLUDEPATH = $${GSTREAMER_PATH}/include/gstreamer-1.0
    win32-g++:GSTREAMER_BASE_1_0_LIBS = -L$${GSTREAMER_PATH}/lib -lgstbase-1.0
    win32-msvc:GSTREAMER_BASE_1_0_LIBS = /LIBPATH:$${GSTREAMER_PATH}/lib gstbase-1.0.lib

    INCLUDEPATH += $${GSTREAMER_1_0_INCLUDEPATH} \
                   $${GSTREAMER_VIDEO_1_0_INCLUDEPATH} \
                   $${GSTREAMER_GL_1_0_INCLUDEPATH} \
                   $${GLIB_2_0_INCLUDEPATH} \
                   $${GOBJECT_2_0_INCLUDEPATH} \
                   $${GSTREAMER_BASE_1_0_INCLUDEPATH}

    LIBS += $${GSTREAMER_1_0_LIBS} \
            $${GSTREAMER_VIDEO_1_0_LIBS} \
            $${GSTREAMER_GL_1_0_LIBS} \
            $${GLIB_2_0_LIBS} \
            $${GOBJECT_2_0_LIBS} \
            $${GSTREAMER_BASE_1_0_LIBS}
} else {
    PKGCONFIG += \
        gstreamer-1.0 \
        gstreamer-video-1.0 \
        gstreamer-gl-1.0
}

android:DEFINES += HAVE_QT_ANDROID
win32:DEFINES += HAVE_QT_WIN32
macx:DEFINES += HAVE_QT_MAC

versionAtLeast(QT_VERSION, "5.5") {
    win32-msvc: LIBS += opengl32.lib
    win32-g++: LIBS += -lopengl32
}

SOURCES += \
    qtgst/gstqtglutility.cc \
    qtgst/gstqsgtexture.cc \
    qtgst/gstqtsink.cc \
    qtgst/qtglrenderer.cc \
    qtgst/qtwindow.cc \
    qtgst/qtitem.cc

HEADERS += \
    qtgst/gstqsgtexture.h \
    qtgst/gstqtgl.h \
    qtgst/gstqtglutility.h \
    qtgst/gstqtsink.h \
    qtgst/qtglrenderer.h \
    qtgst/qtwindow.h \
    qtgst/qtitem.h
