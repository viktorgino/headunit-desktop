import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.1
import QtQml 2.11
import HUDTheme 1.0

ThemeRoot {
    id: __root

    signal push(var qml, var properties)
    Component {
        id: highlight
        Rectangle {
            height: 40
            anchors.right: parent.right
            anchors.left: parent.left
            color: "lightsteelblue"
            y: mediaLocations.currentItem.y
        }
    }

    Connections{
        target:pluginContext
        onLocationsUpdated : {
            mediaLocations.model = pluginContext.MediaLocations
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
                    Text {
                        x:8
                        text: modelData.relative_path
                        font.bold: true
                        anchors.verticalCenter: parent.verticalCenter
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: parent.ListView.view.currentIndex = index
                    }
                }
                model: pluginContext.MediaLocations

                ScrollBar.vertical: ScrollBar {
                }
                highlight: highlight
                highlightFollowsCurrentItem: false
                currentIndex: -1
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
                onClicked: {
                    __root.push("qrc:/MediaPlayer/qml/MediaPlayerSettingsFileBrowser.qml", {"pluginContext" : pluginContext, "pluginSettings" : pluginSettings})
                }
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
        }
    }

    Connections {
        target: pluginContext
        onScanningFinished:{
            console.log("Media scanning finished")
            mediaLocations.model = pluginContext.getLocations()
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;formeditorColor:"#4c4e50";height:480;width:640}
}
##^##*/
