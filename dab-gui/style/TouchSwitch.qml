import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2
import QtQuick.Layouts 1.1

// Import custom styles
import "."

Item {
    id: masterLayout
    signal changed(bool valueChecked)

    property alias name: nameView.text
    property alias objectName: switchView.objectName
    property alias checked: switchView.checked

    Layout.preferredWidth: parent.width

    Text {
        id: nameView
        font.pixelSize: 22
        color: "#ffffff"
        anchors.left: parent.left
        Behavior on x { NumberAnimation{ easing.type: Easing.OutCubic} }
    }
    Switch {
        id: switchView
        style: switchStyle
        anchors.right: parent.right
        onCheckedChanged: masterLayout.changed(switchView.checked)
    }

    /* Switch Style */
    Component {
        id: switchStyle
        SwitchStyle {

            groove: Rectangle {
                implicitHeight: 25
                implicitWidth: 70
                Rectangle {
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.bottom: parent.bottom
                    width: parent.width/2 - 2
                    height: 20
                    anchors.margins: 2
                    color: control.checked ? "#468bb7" : "#222"
                    Behavior on color {ColorAnimation {}}
                    Text {
                        font.pixelSize: 1
                        color: "white"
                        anchors.centerIn: parent
                        text: qsTr("ON")
                    }
                }
                Item {
                    width: parent.width/2
                    height: parent.height
                    anchors.right: parent.right
                    Text {
                        font.pixelSize: 1
                        color: "white"
                        anchors.centerIn: parent
                        text: qsTr("OFF")
                    }
                }
                color: "#222"
                border.color: "#444"
                border.width: 2
            }
            handle: Rectangle {
                width: parent.parent.width/2
                height: control.height
                color: "#444"
                border.color: "#555"
                border.width: 2
            }
        }
    }
}
