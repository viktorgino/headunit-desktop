import QtQuick 2.5
import QtQuick.Window 2.2
import Qt.labs.settings 1.0

Window {

    id: window
    visible: true
    title: qsTr("viktorgino's HeadUnit")
    width: 800
    height: 480

    FontLoader{id:ralewayRegular; source:"qrc:/qml/fonts/Raleway-Regular.ttf"}

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
        anchors.fill: parent
        source: ThemeManager.themeSource
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
        console.log("Loader loaded");
    }
}
