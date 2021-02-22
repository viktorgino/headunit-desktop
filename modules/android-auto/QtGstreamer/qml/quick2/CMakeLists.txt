set(QtGStreamerQuick2_SRCS
    plugin.cpp
)

add_library(QtGStreamerQuick2 MODULE ${QtGStreamerQuick2_SRCS})
target_link_libraries(QtGStreamerQuick2 ${QTGSTREAMER_QUICK_LIBRARIES})
qt4or5_use_modules(QtGStreamerQuick2 Qml)

if (WIN32 AND CMAKE_COMPILER_IS_GNUCXX)
  # On windows with gcc, cmake calls the binary libFOO.dll, but the Qt plugin loader
  # does not remove the lib prefix when searching for .dlls, unlike what happens on unix
  file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/qmldir "module QtGStreamer\nplugin libQtGStreamerQuick2")
else()
  file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/qmldir "module QtGStreamer\nplugin QtGStreamerQuick2")
endif()

install(TARGETS QtGStreamerQuick2 DESTINATION ${QTGSTREAMER_QTQUICK2_INSTALL_DIR}/QtGStreamer/)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/qmldir DESTINATION ${QTGSTREAMER_QTQUICK2_INSTALL_DIR}/QtGStreamer/)

# create a layout similar to the one in ${QTGSTREAMER_QTQUICK2_INSTALL_DIR} for testing
add_custom_command(TARGET QtGStreamerQuick2 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/QtGStreamer
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:QtGStreamerQuick2> ${CMAKE_CURRENT_BINARY_DIR}/QtGStreamer/
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/qmldir ${CMAKE_CURRENT_BINARY_DIR}/QtGStreamer/
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
