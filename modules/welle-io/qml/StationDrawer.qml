import QtQuick 2.11
import HUDTheme 1.0
import QtQuick.Controls 2.4
import QtGraphicalEffects 1.0
import Qt.labs.settings 1.0

Item {
    id:__root

    Rectangle {
        id: rectangle
        color: "#212121"
        anchors.fill: parent
    }

    Component.onCompleted: {
    }


    Settings {
        property alias stationListSerialize: stationList.serialized
        property alias favoritsListSerialize: favouritesList.serialized
    }

    Rectangle {
        id: favouritesButton
        height: 40
        color: "#424242"
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.topMargin: 0
        anchors.leftMargin: 0

        ThemeText {
            id: text1
            text: qsTr("Favourites")
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 16
        }

        MouseArea {
            id: mouseArea2
            anchors.left: parent.left
            anchors.right: scanMenu.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.rightMargin: 8
            onClicked: __root.state = ""
        }

            ToolButton {
                id: scanMenu
                width: 40
                height: 40
                anchors.right: parent.right
                ImageButton {
                    width: 30
                    height: 30
                    anchors.verticalCenter: parent.verticalCenter
                    imageSource : "qrc:/qml/icons/navicon.png"
                    anchors.horizontalCenter: parent.horizontalCenter
                    color: HUDStyle.colors.headingText1
                    onClicked: {
                        menu.open()
                    }
                }
                Menu {
                    id: menu
                    y: parent.height
                    width: 200

                    MenuItem {
                        text: pluginContext.RadioController.isChannelScan? "Stop scanning" : "Scan for stations"
                        onClicked: {
                            if(!pluginContext.RadioController.isChannelScan){
                                pluginContext.RadioController.startScan()
                            } else {
                                pluginContext.RadioController.stopScan()
                            }
                        }
                    }
                    MenuItem {
                        text: "Clear station list"
                        onClicked: {
                            stationList.clearStations()
                        }
                    }
                }
        }
    }


    Rectangle {
        id: allStationsButton
        y: parent.height - height
        height: 40
        color: "#424242"
        anchors.left: parent.left
        anchors.right: parent.right
        ThemeText {
            id: text2
            text: qsTr("All Stations")
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 16
        }

        Rectangle {
            id: rectangle2
            height: 1
            color: "#000000"
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.topMargin: 0
            anchors.leftMargin: 0
        }

        MouseArea {
            id: mouseArea1
            anchors.fill: parent
            onClicked: __root.state = "allStations"
        }
        anchors.leftMargin: 0
    }

    StationList {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: favouritesButton.bottom
        anchors.bottom: allStationsButton.top
        anchors.leftMargin: 0

        model : StationListModel { id: favouritesList ; type: "favorites"}
        onItemClicked: {
            pluginContext.RadioController.play(channelName, stationName, stationSId)
        }
        onSetFavourite: {
            stationList.setFavorit(stationSId, channelName, favourite)
            favouritesList.removeStation(stationSId, channelName);
        }
    }

    StationList {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: allStationsButton.bottom
        anchors.bottom: parent.bottom
        clip: true
        model : StationListModel { id: stationList ; type: "all"}
        onItemClicked: {
            pluginContext.RadioController.play(channelName, stationName, stationSId)
        }
        onSetFavourite: {
            if(favourite)
                favouritesList.addStation(stationName, stationSId, channelName, true);
            else
                favouritesList.removeStation(stationSId, channelName);
        }
    }

    Connections{
        target: pluginContext.RadioController
        onNewStationNameReceived: {
            stationList.addStation(station, sId, channel, false)
        }
    }
    states: [
        State {
            name: "allStations"

            PropertyChanges {
                target: allStationsButton
                y: 40
            }
        }
    ]
    transitions:
        Transition {
        NumberAnimation { properties: "y"; easing.type: Easing.InOutQuad }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
