set(QtGStreamerQuick1_SRCS
    plugin.cpp
    videoitem.cpp
)

add_library(QtGStreamerQuick1 MODULE ${QtGStreamerQuick1_SRCS})
target_link_libraries(QtGStreamerQuick1 ${QTGSTREAMER_UI_LIBRARIES})
qt4or5_use_modules(QtGStreamerQuick1 Quick1)

if (WIN32 AND CMAKE_COMPILER_IS_GNUCXX)
  # On windows with gcc, cmake calls the binary libFOO.dll, but the Qt plugin loader
  # does not remove the lib prefix when searching for .dlls, unlike what happens on unix
  file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/qmldir "plugin libQtGStreamerQuick1")
else()
  file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/qmldir "plugin QtGStreamerQuick1")
endif()

install(TARGETS QtGStreamerQuick1 DESTINATION ${QTGSTREAMER_QTQUICK1_INSTALL_DIR}/QtGStreamer)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/qmldir DESTINATION ${QTGSTREAMER_QTQUICK1_INSTALL_DIR}/QtGStreamer)

# create a layout similar to the one in ${QTGSTREAMER_QTQUICK1_INSTALL_DIR} for testing
add_custom_command(TARGET QtGStreamerQuick1 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/QtGStreamer
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:QtGStreamerQuick1> ${CMAKE_CURRENT_BINARY_DIR}/QtGStreamer/
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/qmldir ${CMAKE_CURRENT_BINARY_DIR}/QtGStreamer/
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
