set(QtGLib_SRCS
    quark.cpp
    type.cpp
    paramspec.cpp
    object.cpp
    value.cpp
    signal.cpp
    error.cpp
    connect.cpp
    wrap.cpp
    init.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/gen.cpp
)

set(QtGLib_INSTALLED_HEADERS
    global.h      Global
    init.h        Init
    quark.h       Quark
    type.h        Type
    refpointer.h  RefPointer
    wrap.h
    paramspec.h   ParamSpec
    object.h      Object
    value.h       Value
    qglib_signal.h Signal
    emitimpl.h
    connect.h     Connect
    connectimpl.h
    error.h       Error
)

file(GLOB QtGLib_CODEGEN_HEADERS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "*.h")
list(REMOVE_ITEM QtGLib_CODEGEN_HEADERS "connectimpl.h" "emitimpl.h")

set(QtGLib_CODEGEN_INCLUDES
    -Iglib-object.h
)

# Setup the environment
set(QTGLIB_API_VERSION 2.0)
set(QTGLIB_SOVERSION 0)
include_directories(${GOBJECT_INCLUDE_DIR} ${GLIB2_INCLUDE_DIR})

# Add command to generate gen.cpp using codegen
run_codegen("QGlib" "${QtGLib_CODEGEN_INCLUDES}" "${QtGLib_CODEGEN_HEADERS}")

# Build and link QtGLib
add_library(${QTGLIB_LIBRARY} ${SHARED_OR_STATIC} ${QtGLib_SRCS})
set_target_properties(${QTGLIB_LIBRARY} PROPERTIES OUTPUT_NAME ${QTGLIB_LIBRARY}-${QTGLIB_API_VERSION}
                                          SOVERSION ${QTGLIB_SOVERSION}
                                            VERSION ${QTGSTREAMER_VERSION})
target_link_libraries(${QTGLIB_LIBRARY} LINK_PRIVATE ${GOBJECT_LIBRARIES})
qt4or5_use_modules(${QTGLIB_LIBRARY} LINK_PUBLIC Core)

# Install
install(TARGETS ${QTGLIB_LIBRARY} EXPORT ${EXPORT_TARGET_SET} ${QTGSTREAMER_INSTALL_TARGET_DEFAULT_ARGS})
install_headers("QGlib" ${QtGLib_INSTALLED_HEADERS})
