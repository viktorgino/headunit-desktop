TEMPLATE = app
TARGET = headunit-app
QT += gui widgets qml quick
CONFIG += c++11 link_pkgconfig welleio
QMAKE_CXXFLAGS += -Wno-unused-parameter
INCLUDEPATH += $${PWD}/includes

include("../config.pri")

SOURCES += main.cpp \
    pluginmanager.cpp \
    settingsloader.cpp \
    thememanager.cpp

HEADERS += \
    ../includes/plugininterface.h \
    pluginmanager.h \
    settingsloader.h \
    thememanager.h

rpi {
    DEFINES += RPI
}
DESTDIR = $${OUT_PWD}/../

QML_IMPORT_PATH = $${OUT_PWD}

target.path = $$PREFIX/

pluginfiles.files += \
    $${PWD}/../qml_gui/qml/theme/*

pluginfiles.path = $$PREFIX/HUDTheme

theme.files += \
    $${PWD}/theme.json

theme.path = $$PREFIX/ 

INSTALLS += target pluginfiles theme

