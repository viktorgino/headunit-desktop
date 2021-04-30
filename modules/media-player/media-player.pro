TEMPLATE = lib
CONFIG += c++11 plugin link_pkgconfig
QT += quick multimedia multimediawidgets sql
TARGET = $$qtLibraryTarget(media-player-plugin)
DEFINES += QT_DEPRECATED_WARNINGS
INCLUDEPATH += $${PWD}/../../includes
DESTDIR = $${OUT_PWD}/../../plugins

PKGCONFIG += libudev taglib

SOURCES += \
    mediadb.cpp \
    medialibrary.cpp \
    mediaplayercoverimageprovider.cpp \
    mediascanner.cpp \
    mediaplayer.cpp

RESOURCES += qml.qrc

include("../../config.pri")

target.path = $${PREFIX}/plugins

INSTALLS += target

HEADERS += \
    mediadb.h \
    medialibrary.h \
    mediaplayercoverimageprovider.h \
    mediascanner.h \
    mediaplayer.h

DISTFILES += \
    config.json
