TEMPLATE = app
TARGET = headunit-app
QT += gui widgets qml quick bluetooth
CONFIG += c++11 link_pkgconfig welleio
QMAKE_CXXFLAGS += -Wno-unused-parameter
INCLUDEPATH += $${PWD}/includes

include("../config.pri")

SOURCES += main.cpp \
    pluginmanager.cpp \
    settingsloader.cpp \
    thememanager.cpp

RESOURCES += \
    ../qml_gui/qml.qrc

HEADERS += \
    ../includes/plugininterface.h \
    pluginmanager.h \
    settingsloader.h \
    thememanager.h

include("../qml_gui/quickcross/quickcross.pri")

rpi {
    DEFINES += RPI
}

QML_IMPORT_PATH = $${OUT_PWD}

target.path = $$PREFIX/

pluginfiles.files += \
    $${PWD}/../qml_gui/qml/theme/*

pluginfiles.path = $$PREFIX/HUDTheme

theme.files += \
    $${PWD}/theme.json

theme.path = $$PREFIX/ 

INSTALLS += target pluginfiles theme

DISTFILES += \
    $${PWD}/../qml_gui/qml/theme/qmldir \
    $${PWD}/../qml_gui/qml/theme/designer/hudtheme.metainfo \
    $${PWD}/../qml_gui/qml/theme/backgrounds/* \
    $${PWD}/theme.json

