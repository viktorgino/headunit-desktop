import QtQuick 2.0
import QtQuick.Controls 2.0

// Import custom styles
import "."

Popup {
    property alias text: textView.text

    id: popup
    width: textView.width + 65
    height: 50
    x: parent.width/2 - width/2
    y: 0
    //modal: true
    //focus: true
    closePolicy: Popup.NoAutoClose
    enter:  Transition {
            NumberAnimation { property: "y"; from: -height ; to: 0;}
        }
    exit: Transition {
            NumberAnimation { property: "y"; from: 0 ; to: -height;}
        }
    //onOpened: closeTimer.running = true;

    Text {
        id: textView
        text: "Error Message Popup"
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        font.pixelSize: 22
        color: "#ffffff"
    }

    Rectangle {
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        Image {
            anchors.verticalCenter: parent.verticalCenter
            source: "qrc:/qml/icons/android-close.png"
            height: 25
            fillMode: Image.PreserveAspectFit
        }
    }

    background: Rectangle {color: "darkRed"}

    MouseArea {
        anchors.fill: parent
        onClicked: popup.close()
    }
}
