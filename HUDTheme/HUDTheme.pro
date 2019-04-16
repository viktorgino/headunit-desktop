TEMPLATE = lib
CONFIG += c++11
QT += qml quick

TARGET = $$qtLibraryTarget(hudtheme)
DEFINES += QT_DEPRECATED_WARNINGS



RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

HEADERS += \
    plugin.h

SOURCES += \
    plugin.cpp

#QMAKE_POST_LINK += $$system(qmlplugindump -nonrelocatable HUDTheme 1.0 $${PWD}/qml_gui/qml/theme/ > $${PWD}/qml_gui/qml/theme/plugins.qmltypes)

pluginfiles.files += \
    $${PWD}/qmldir \
    $${PWD}/AlphabetScrollBar.qml \
    $${PWD}/Arrow.qml \
    $${PWD}/ImageButton.qml \
    $${PWD}/MenuButton.qml \
    $${PWD}/ModeSwitch.qml \
    $${PWD}/TopMenu.qml \
    $${PWD}/ThemeText.qml

pluginfiles.path += $${OUT_PWD}
INSTALLS += pluginfiles

DISTFILES += \
    $$PWD/qmldir \
    $$PWD/designer/hudtheme.metainfo

QML_FILES += \
    $$PWD/designer/hudtheme.metainfo
QML_FILES += \
    $${PWD}/AlphabetScrollBar.qml \
    $${PWD}/Arrow.qml \
    $${PWD}/ImageButton.qml \
    $${PWD}/MenuButton.qml \
    $${PWD}/ModeSwitch.qml \
    $${PWD}/TopMenu.qml \
    $${PWD}/ThemeText.qml
