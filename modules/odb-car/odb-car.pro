TEMPLATE = lib
CONFIG += c++11 plugin
QT += quick
TARGET = $$qtLibraryTarget(odb-car-plugin)
DEFINES += QT_DEPRECATED_WARNINGS
INCLUDEPATH += ../../includes
DESTDIR = ../../plugins

SOURCES += \
    odbcar.cpp \
    odbcontroller.cpp

RESOURCES += qml.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    odbcar.h \
    odbcontroller.h

DISTFILES += \
    config.json
