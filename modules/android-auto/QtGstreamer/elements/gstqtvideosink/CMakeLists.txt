glib2_genmarshal(gstqtvideosinkmarshal
    VOID:POINTER,FLOAT,FLOAT,FLOAT,FLOAT
    VOID:POINTER,DOUBLE,DOUBLE,DOUBLE,DOUBLE
    POINTER:POINTER,FLOAT,FLOAT,FLOAT,FLOAT
    POINTER:POINTER,DOUBLE,DOUBLE,DOUBLE,DOUBLE
)

set(GstQtVideoSink_SRCS
    utils/utils.cpp
    utils/bufferformat.cpp

    painters/genericsurfacepainter.cpp

    delegates/basedelegate.cpp
    delegates/qtvideosinkdelegate.cpp
    delegates/qwidgetvideosinkdelegate.cpp

    gstqtvideosinkplugin.cpp
    gstqtvideosinkbase.cpp
    gstqtvideosink.cpp
    gstqwidgetvideosink.cpp

    ${CMAKE_CURRENT_BINARY_DIR}/gstqtvideosinkmarshal.c
)

if (Qt4or5_Quick2_FOUND AND (OPENGL_FOUND OR OPENGLES2_FOUND))
    set(GstQtVideoSink_SRCS
        ${GstQtVideoSink_SRCS}
        painters/videomaterial.cpp
        painters/videonode.cpp

        delegates/qtquick2videosinkdelegate.cpp

        gstqtquick2videosink.cpp
    )
    set(GstQtVideoSink_LINK_OPENGL TRUE)
endif()

if (Qt4or5_OpenGL_FOUND AND (OPENGL_FOUND OR OPENGLES2_FOUND))
    set(GstQtVideoSink_SRCS
        ${GstQtVideoSink_SRCS}
        painters/openglsurfacepainter.cpp
        gstqtglvideosinkbase.cpp
        gstqtglvideosink.cpp
    )
    set(GstQtVideoSink_test_GL_SRCS
        painters/openglsurfacepainter.cpp
    )
    set(GstQtVideoSink_LINK_OPENGL TRUE)
else()
    add_definitions(-DGST_QT_VIDEO_SINK_NO_OPENGL)
endif()

add_definitions(
    -DQTVIDEOSINK_NAME=${QTVIDEOSINK_NAME}
    -DQTGLVIDEOSINK_NAME=${QTGLVIDEOSINK_NAME}
    -DQWIDGETVIDEOSINK_NAME=${QWIDGETVIDEOSINK_NAME}
)

if (GstQtVideoSink_LINK_OPENGL)
    if (OPENGLES2_FOUND)
        set(GstQtVideoSink_GL_LIBS ${OPENGLES2_LIBRARY})
        include_directories(${OPENGLES2_INCLUDE_DIR})
    else()
        set(GstQtVideoSink_GL_LIBS ${OPENGL_gl_LIBRARY})
        include_directories(${OPENGL_INCLUDE_DIR})
    endif()
endif()

add_library(gst${QTVIDEOSINK_NAME} MODULE ${GstQtVideoSink_SRCS})
target_link_libraries(gst${QTVIDEOSINK_NAME}
    ${GOBJECT_LIBRARIES}
    ${GSTREAMER_LIBRARY}
    ${GSTREAMER_BASE_LIBRARY}
    ${GSTREAMER_VIDEO_LIBRARY}
    ${GstQtVideoSink_GL_LIBS}
)
qt4or5_use_modules(gst${QTVIDEOSINK_NAME} Core Gui Widgets)
if (Qt4or5_Quick2_FOUND AND (OPENGL_FOUND OR OPENGLES2_FOUND))
    qt4or5_use_modules(gst${QTVIDEOSINK_NAME} Quick2)
endif()
if (Qt4or5_OpenGL_FOUND AND (OPENGL_FOUND OR OPENGLES2_FOUND))
    qt4or5_use_modules(gst${QTVIDEOSINK_NAME} OpenGL)
endif()
install(TARGETS gst${QTVIDEOSINK_NAME} DESTINATION ${QTGSTREAMER_GST_PLUGINS_INSTALL_DIR})

if (QTGSTREAMER_TESTS)
    add_executable(qtvideosink_autotest
        autotest.cpp
        utils/utils.cpp
        utils/bufferformat.cpp
        painters/genericsurfacepainter.cpp
        ${GstQtVideoSink_test_GL_SRCS}
    )
    target_link_libraries(qtvideosink_autotest
        ${GOBJECT_LIBRARIES}
        ${GSTREAMER_LIBRARY}
        ${GSTREAMER_BASE_LIBRARY}
        ${GSTREAMER_VIDEO_LIBRARY}
        ${GstQtVideoSink_GL_LIBS}
    )
    qt4or5_use_modules(qtvideosink_autotest Core Gui Widgets Test)
    if (Qt4or5_OpenGL_FOUND AND (OPENGL_FOUND OR OPENGLES2_FOUND))
        qt4or5_use_modules(qtvideosink_autotest OpenGL)
    endif()
endif()
