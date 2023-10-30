import QtQuick 2.5
import QtQuick.Window 2.2
import Qt.labs.settings 1.0

import HUDLoader 1.0

Window {

    id: window
    visible: true
    title: qsTr("viktorgino's HeadUnit")
    width: 800
    height: 480

    required property HUDLoader appLoader

    Rectangle {
        color: "#000000"
        anchors.fill: parent
    }

    Image {
        anchors.fill: parent
        source:"qrc:/boot-logo-800x480.png"
        fillMode: Image.PreserveAspectFit
    }

    Loader {
        id: themeLoader
        anchors.fill: parent
    }
    Connections {
        target: appLoader
        onThemeSourceChanged: {
            themeLoader.setSource(source)
        }
    }

    Shortcut {
        sequence: "F11"
        onActivated: {
            if(window.visibility == Window.FullScreen)
                window.visibility = Window.Windowed
            else if(window.visibility == Window.Windowed)
                window.visibility = Window.Maximized
            else
                window.visibility = Window.FullScreen
        }
    }
    Component.onCompleted: {
        console.log("Loading app");
        appLoader.load()
    }
}
