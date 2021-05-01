import QtQuick 2.11


Item {
    id: __root
    signal menuClicked();
    property double bg_opacity: 0.5
    property alias menuButtonActive: menuButton.isActive

    Rectangle {
        id: overlay1
        anchors.leftMargin: 0
        anchors.top: parent.top
        anchors.rightMargin: 0
        anchors.topMargin: 0
        anchors.bottomMargin: 0
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        color: "#000000"
        opacity: bg_opacity
        visible: true
    }

    Rectangle {
        id: rectangle1
        width:height
        color: "#263238"
        anchors.topMargin: parent.height*0.1
        anchors.bottomMargin: parent.height*0.1
        anchors.leftMargin: parent.height*0.1
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        MenuButton {
            id: menuButton
            anchors.rightMargin: parent.height*0.2
            anchors.right: parent.right
            anchors.left: parent.left
            anchors.leftMargin: parent.height*0.2
            anchors.top: parent.top
            anchors.topMargin: parent.height*0.2
            anchors.bottom: parent.bottom
            anchors.bottomMargin: parent.height*0.2
        }

        MouseArea {
            anchors.fill: parent
            onClicked:{
                __root.menuClicked();
            }
        }
    }

    Item {
        width: parent.width/2
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: parent.height*0.1
        anchors.bottomMargin: parent.height*0.1
        anchors.top: parent.top
        anchors.bottom: parent.bottom

        Text {
            id:clock
            color: "#ffffff"
            text: Qt.formatDateTime(new Date(),"dddd d/M/yyyy")
            anchors.fill: parent
            font.pixelSize: parent.height/2
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
        }
        Timer {
            interval: 500; running: true; repeat: true
            onTriggered: {
                clock.text = Qt.formatDateTime(new Date(),"dddd d/M/yyyy")
                date.text = Qt.formatDateTime(new Date(),"HH:mm:ss")
            }
        }

    }

    Item {
        width: height*3
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.topMargin: parent.height*0.1
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.bottomMargin: parent.height*0.1
        Text {
            id: date
            color: "#ffffff"
            text: Qt.formatDateTime(new Date(),"HH:mm:ss")
            anchors.fill: parent
            font.pixelSize: parent.height/2
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
        }

        Timer {
            repeat: true
            interval: 1000
            running: true
        }
    }
}
