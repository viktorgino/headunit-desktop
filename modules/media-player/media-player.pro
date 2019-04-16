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
    mediascanner.cpp \
    mediaplayer.cpp

RESOURCES += qml.qrc

## Default rules for deployment.
#qnx: target.path = /tmp/$${TARGET}/bin
#else: unix:!android: target.path = /opt/$${TARGET}/bin
#!isEmpty(target.path): INSTALLS += target

HEADERS += \
    mediadb.h \
    medialibrary.h \
    mediascanner.h \
    mediaplayer.h

DISTFILES += \
    config.json
