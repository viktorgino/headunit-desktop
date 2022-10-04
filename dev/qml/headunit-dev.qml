import QtQuick 2.5
import QtQuick.Window 2.2
import Qt.labs.settings 1.0
import QtQuick.Layouts 1.11
import QtQuick.Controls 2.15


Window {

    id: window
    visible: true
    title: qsTr("viktorgino's HeadUnit")
    width: 800
    height: 480

    FontLoader{id:ralewayRegular; source:"../../themes/default-theme/qml/fonts/Raleway-Regular.ttf"}

    Rectangle {
        id: rectangle
        color: "#636363"
        anchors.fill: parent
    }

    Loader {
        id: loader
        anchors.left: parent.left
        anchors.right: rightMenu.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        source:"AudioProcessor.qml"
    }


    ColumnLayout {
        id: rightMenu
        width: parent.width * 0.15
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.rightMargin: 8
        anchors.bottomMargin: 8
        anchors.topMargin: 8

        Button {
            id: button
            text: qsTr("Audio Processor")
            Layout.fillWidth: true
            onClicked: {
                loader.setSource("AudioProcessor.qml")
            }
        }

        Button {
            id: button1
            text: qsTr("FM")
            Layout.fillWidth: true
        }

        Button {
            id: button2
            text: qsTr("HVAC")
            Layout.fillWidth: true
        }

        Button {
            id: button3
            text: qsTr("Media Player")
            Layout.fillWidth: true
        }

        Button {
            id: button4
            text: qsTr("Phone")
            Layout.fillWidth: true
        }

        Button {
            id: button5
            text: qsTr("Volume Control")
            Layout.fillWidth: true
        }

        Button {
            id: button6
            text: qsTr("DAB")
            Layout.fillWidth: true
        }
    }

}
