TEMPLATE = app
TARGET = headunit-app
QT += gui widgets qml quick bluetooth
CONFIG += c++11 link_pkgconfig welleio
QMAKE_CXXFLAGS += -Wno-unused-parameter

SOURCES += main.cpp \
    pluginmanager.cpp \
    settingsloader.cpp \
    thememanager.cpp

RESOURCES += \
    qml_gui/qml.qrc

HEADERS += \
    includes/plugininterface.h \
    pluginmanager.h \
    settingsloader.h \
    thememanager.h

include("qml_gui/quickcross/quickcross.pri")

rpi {
    DEFINES += RPI
}


QML_IMPORT_PATH = $${OUT_PWD}

pluginfiles.files += \
    $${PWD}/qml_gui/qml/theme/*

pluginfiles.path += $${OUT_PWD}/HUDTheme


theme.files += \
    $${PWD}/theme.json

theme.path += $${OUT_PWD}

INSTALLS += pluginfiles theme

DISTFILES += \
    $$PWD/qml_gui/qml/theme/qmldir \
    $$PWD/qml_gui/qml/theme/designer/hudtheme.metainfo \
    $$PWD/qml_gui/qml/theme/backgrounds/* \
    theme.json
