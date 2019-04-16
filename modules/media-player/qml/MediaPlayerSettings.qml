import QtQuick 2.6
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.1

import HUDTheme 1.0
Item {
    id: root

    Component {
        id: highlight
        Rectangle {
            height: 40
            anchors.right: parent.right
            anchors.left: parent.left
            color: "lightsteelblue"
            y: mediaLocations.currentItem.y
            Behavior on y {
                SpringAnimation {
                    spring: 3
                    damping: 0.2
                }
            }
        }
    }

    Rectangle {
        id: rectangle
        height: parent.height -16
        anchors.left: parent.left
        anchors.leftMargin: 8
        anchors.right: parent.right
        anchors.rightMargin: 8
        color: "#eceff1"
        anchors.top: parent.top
        anchors.topMargin: 8
        border.width: 1
        border.color: "#000000"


        Item {
            anchors.rightMargin: 1
            anchors.leftMargin: 1
            anchors.topMargin: 1
            clip: true
            anchors.bottom: rectangle1.top
            anchors.right: parent.right
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottomMargin: 1

            ListView {
                id: mediaLocations
                anchors.fill: parent
                clip: true
                delegate: Item {
                    height: 40
                    width:parent.width
                    property string path : modelData.volume_path + modelData.relative_path
                    Text {
                        text: modelData.name  + " ("+path+")"
                        font.bold: true
                        anchors.verticalCenter: parent.verticalCenter
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: parent.ListView.view.currentIndex = index
                    }
                }
                model: MediaPlayerPlugin.getLocations()

                ScrollBar.vertical: ScrollBar {
                }
                highlight: highlight
                highlightFollowsCurrentItem: false
                focus: true
            }
        }

        Rectangle {
            id: rectangle1
            height: 1
            color: "#000000"
            anchors.bottom: buttons.top
            anchors.bottomMargin: 0
            anchors.right: parent.right
            anchors.rightMargin: 0
            anchors.left: parent.left
            anchors.leftMargin: 0
        }

        Item {
            id: buttons
            height: 40
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.right: parent.right
            anchors.rightMargin: 0

            Button {
                id: button
                width: 100
                height: 24
                text: qsTr("New")
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 8
                onClicked: root.state = "file browser"
            }

            Button {
                id: button1
                width: 100
                height: 24
                text: qsTr("Remove")
                enabled: false
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 8
            }

            Button {
                id: button2
                width: 100
                height: 24
                text: qsTr("Edit")
                enabled: false
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: button1.left
                anchors.rightMargin: 6
            }
        }


    }

    states: [
        State {
            name: "file browser"

            PropertyChanges {
                target: rectangle
                anchors.topMargin: -parent.height + 8
                clip: true
                anchors.bottomMargin: parent.height
            }

            PropertyChanges {
                target: rectangle2
                anchors.topMargin: 8
            }
        }
    ]

    transitions:
        Transition {
        NumberAnimation { properties: "anchors.topMargin,anchors.bottomMargin"; duration: 250}
    }


    Rectangle {
        id: rectangle2
        height: parent.height -16
        anchors.left: parent.left
        anchors.leftMargin: 8
        anchors.right: parent.right
        anchors.rightMargin: 8
        color: "#eceff1"
        anchors.topMargin: parent.height + 8
        anchors.top: parent.top
        border.width: 1
        border.color: "#000000"
        FileBrowser {
            id: fileBrowser
            anchors.bottom: rectangle3.top
            anchors.right: parent.right
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottomMargin: 0
        }

        Rectangle {
            id: rectangle3
            height: 1
            color: "#000000"
            anchors.bottom: buttons1.top
            anchors.bottomMargin: 0
            anchors.rightMargin: 0
            anchors.right: parent.right
            anchors.leftMargin: 0
            anchors.left: parent.left
        }

        Item {
            id: buttons1
            height: 40
            anchors.rightMargin: 0
            anchors.right: parent.right
            anchors.leftMargin: 0
            anchors.bottom: parent.bottom
            Button {
                id: button3
                width: 100
                height: 24
                text: qsTr("Add")
                anchors.verticalCenter: parent.verticalCenter
                anchors.leftMargin: 8
                anchors.left: parent.left
                onClicked: {
                    MediaPlayerPlugin.addLocation(fileBrowser.folder)
                    root.state = "base state"
                    fileBrowser.reset()
                }
            }

            Button {
                id: button4
                width: 100
                height: 24
                text: qsTr("Cancel")
                anchors.right: parent.right
                anchors.rightMargin: 8
                anchors.verticalCenter: parent.verticalCenter
                onClicked: {
                    root.state = "base state"
                    fileBrowser.reset()
                }
            }
            anchors.bottomMargin: 0
            anchors.left: parent.left
        }
    }


    Connections {
        target: MediaPlayerPlugin
        onMediaScanningFinished:mediaLocations.model = MediaPlayerPlugin.getLocations()
    }
}
