set(GST_PACKAGE_NAME "QtGStreamer" CACHE STRING "The name of this package")
set(GST_PACKAGE_ORIGIN "http://gstreamer.freedesktop.org/" CACHE STRING "The origin url of this package")

add_definitions(-DPACKAGE="qt-gstreamer"
                -DPACKAGE_NAME="${GST_PACKAGE_NAME}"
                -DPACKAGE_ORIGIN="${GST_PACKAGE_ORIGIN}"
                -DPACKAGE_VERSION="${QTGSTREAMER_VERSION}"
                -DGST_DISABLE_XML
                -DGST_DISABLE_LOADSAVE)

include_directories(${GSTREAMER_INCLUDE_DIRS} ${GLIB2_INCLUDE_DIR})

if (GSTREAMER_BASE_LIBRARY_FOUND AND GSTREAMER_VIDEO_LIBRARY_FOUND)
    include_directories(${GSTREAMER_BASE_INCLUDE_DIR} ${GSTREAMER_VIDEO_INCLUDE_DIR})
    add_subdirectory(gstqtvideosink)
endif()
