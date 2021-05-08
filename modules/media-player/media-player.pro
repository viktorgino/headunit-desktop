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
    mediadbmanager.cpp \
    medialibrarycontainermodel.cpp \
    medialibrarymedialistmodel.cpp \
    mediaplayercoverimageprovider.cpp \
    mediaplayerplaylistmodel.cpp \
    mediascanner.cpp \
    mediaplayer.cpp

RESOURCES += qml.qrc

include("../../config.pri")

target.path = $${PREFIX}/plugins

INSTALLS += target

HEADERS += \
    mediadb.h \
    mediadbmanager.h \
    medialibrarycontainermodel.h \
    medialibrarymedialistmodel.h \
    mediaplayercoverimageprovider.h \
    mediaplayerplaylistmodel.h \
    mediascanner.h \
    mediaplayer.h

DISTFILES += \
    config.json
