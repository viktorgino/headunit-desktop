import QtQuick 2.0
import QtQuick.Window 2.0

Window {

    id: window
    visible: true
    title: qsTr("viktorgino's HeadUnit")
    //    visibility: Window.Maximized
    width: 800
    height: 480

    Text {
        id: text1
        text: qsTr("Loading UI")
        anchors.verticalCenter: parent.verticalCenter
        font.pixelSize: 50
        anchors.horizontalCenter: parent.horizontalCenter
    }

}
