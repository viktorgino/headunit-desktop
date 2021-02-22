set(EXPORT_TARGET_SET ${QTGSTREAMER_PACKAGE_NAME}Targets)

if (QTGSTREAMER_STATIC)
    set(SHARED_OR_STATIC "STATIC")
else()
    set(SHARED_OR_STATIC "SHARED")
    if (CMAKE_COMPILER_IS_GNUCXX AND NOT WIN32)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden -fvisibility-inlines-hidden")
    endif ()
endif()

# Macro to run codegen from the subdirs
macro(run_codegen _dir_name _includes _headers)
    if (QTGSTREAMER_CODEGEN)
        set(_prefixed_headers "")
        foreach(_header ${_headers})
            list(APPEND _prefixed_headers ${_dir_name}/${_header})
        endforeach()

        add_custom_command(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/gen.cpp
                           COMMAND codegen
                           ARGS ${_includes} ${_prefixed_headers}
                                > ${CMAKE_CURRENT_SOURCE_DIR}/gen.cpp
                           DEPENDS codegen ${_headers}
                           WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/..)
    endif()

    add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/gen.cpp
                       COMMAND ${CMAKE_COMMAND}
                       ARGS -E copy ${CMAKE_CURRENT_SOURCE_DIR}/gen.cpp
                            ${CMAKE_CURRENT_BINARY_DIR}/gen.cpp
                       DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/gen.cpp
                       COMMENT "Copying gen.cpp to the build directory")
endmacro()

# Macro to install headers from the subdirs
macro(install_headers _dir_name)
    foreach(header ${ARGN})
        get_filename_component(header_path ${header} PATH)
        install(FILES ${header} DESTINATION ${QTGSTREAMER_INCLUDES_INSTALL_DIR}/${_dir_name}/${header_path})
    endforeach()
endmacro()

# Setup common environment
include_directories(${QTGSTREAMER_INCLUDES})
add_definitions(${QTGSTREAMER_DEFINITIONS})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${QTGSTREAMER_FLAGS}")

add_definitions(
    -DQTVIDEOSINK_NAME="${QTVIDEOSINK_NAME}"
    -DQTGLVIDEOSINK_NAME="${QTGLVIDEOSINK_NAME}"
    -DQWIDGETVIDEOSINK_NAME="${QWIDGETVIDEOSINK_NAME}"
)

# Build the libraries
add_subdirectory(QGlib)
add_subdirectory(QGst)

# Build the qml plugins
add_subdirectory(qml)

# Install the cmake scripts that are used to find the installed library from external projects
include(CMakePackageConfigHelpers)
configure_package_config_file(
    ${CMAKE_SOURCE_DIR}/cmake/modules/QtGStreamerConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/${QTGSTREAMER_PACKAGE_NAME}Config.cmake
    INSTALL_DESTINATION ${QTGSTREAMER_CMAKE_CONFIG_INSTALL_DIR}
    PATH_VARS QTGSTREAMER_INCLUDES_INSTALL_DIR
    NO_CHECK_REQUIRED_COMPONENTS_MACRO)
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/${QTGSTREAMER_PACKAGE_NAME}ConfigVersion.cmake
    VERSION ${QTGSTREAMER_VERSION}
    COMPATIBILITY SameMajorVersion)
install(EXPORT ${EXPORT_TARGET_SET} DESTINATION ${QTGSTREAMER_CMAKE_CONFIG_INSTALL_DIR})
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${QTGSTREAMER_PACKAGE_NAME}Config.cmake
              ${CMAKE_CURRENT_BINARY_DIR}/${QTGSTREAMER_PACKAGE_NAME}ConfigVersion.cmake
              ${CMAKE_SOURCE_DIR}/cmake/modules/QtGStreamerConfigCommon.cmake
        DESTINATION ${QTGSTREAMER_CMAKE_CONFIG_INSTALL_DIR})

# Install pkgconfig files
file(GLOB_RECURSE PC_IN_FILES "*.pc.in")
foreach(pc_in_file ${PC_IN_FILES})
    get_filename_component(pc_out_file ${pc_in_file} NAME)
    string(REPLACE ".pc.in" ".pc" pc_out_file ${pc_out_file})
    if (USE_QT5)
        string(REPLACE "Qt" "Qt5" pc_out_file ${pc_out_file})
    endif()
    # Qt4 version of the QtGStreamerQuick .pc file should not be installed
    if (NOT ${pc_out_file} MATCHES "QtGStreamerQuick.*")
        configure_file(${pc_in_file} ${CMAKE_CURRENT_BINARY_DIR}/${pc_out_file} @ONLY)
        install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${pc_out_file}
                DESTINATION ${QTGSTREAMER_PC_INSTALL_DIR})
    endif()
endforeach()
