TEMPLATE = app
TARGET = headunit-app
QT += gui widgets qml quick bluetooth
CONFIG += c++11 link_pkgconfig welleio
QMAKE_CXXFLAGS += -Wno-unused-parameter

SOURCES += main.cpp \
    pluginmanager.cpp

RESOURCES += \
    qml_gui/qml.qrc

HEADERS += \
    includes/plugininterface.h \
    pluginmanager.h

include("qml_gui/quickcross/quickcross.pri")

rpi {
    DEFINES += RPI
}
