import QtQuick 2.11
import HUDTheme 1.0
import QtQuick.Controls 2.4
import QtGraphicalEffects 1.0

Item {
    id:__root
    property alias currentPlaying: nowPlayingList.currentPlaying
    signal mediaPlayeritemClicked(string name,url icon, string item_type)
    signal playListItemClicked(int index)
    Rectangle {
        id: rectangle
        color: "#212121"
        anchors.fill: parent
    }


    Rectangle {
        id: nowPlayingButton
        height: 40
        color: "#424242"
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.topMargin: 0
        anchors.leftMargin: 0

        ThemeText {
            id: text1
            text: qsTr("Now Playing")
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 16
        }

        MouseArea {
            id: mouseArea2
            anchors.fill: parent
            onClicked: __root.state = ""
        }
    }


    Rectangle {
        id: mediaLibraryButton
        y: parent.height - height
        height: 40
        color: "#424242"
        anchors.left: parent.left
        anchors.right: parent.right
        ThemeText {
            id: text2
            text: qsTr("Media Library")
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
            onClicked: __root.state = "mediaLibrary"
        }
        anchors.leftMargin: 0
    }

    NowPlayingList {
        id: nowPlayingList
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: nowPlayingButton.bottom
        anchors.bottom: mediaLibraryButton.top
        anchors.leftMargin: 0

        model : pluginContext.PlaylistModel
        onItemClicked: __root.playListItemClicked(index)
    }

    ListView {
        id: listView
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: mediaLibraryButton.bottom
        anchors.bottom: parent.bottom
        clip: true
        model: ListModel {
            ListElement {
                name: "Playlists"
                icon: "qrc:/qml/icons/svg/android-list.svg"
                item_type: "playlists"
            }

            ListElement {
                name: "Artists"
                icon: "qrc:/qml/icons/svg/android-person.svg"
                item_type: "artists"
            }

            ListElement {
                name: "Albums"
                icon: "qrc:/qml/icons/disc.png"
                item_type: "albums"
            }

            ListElement {
                name: "Songs"
                icon: "qrc:/qml/icons/music-note.png"
                item_type: "songs"
            }

            ListElement {
                name: "Genres"
                icon: "qrc:/qml/icons/headphone.png"
                item_type: "genres"
            }

            ListElement {
                name: "Folders"
                icon: "qrc:/qml/icons/folder.png"
                item_type: "folders"
            }
        }
        delegate: Item {
            id: __drawer_item
            height: 40
            width: parent.width
            ImageIcon {
                id: image1
                width: height
                source : icon
                anchors.left: parent.left
                anchors.bottom: parent.bottom
                anchors.top: parent.top
                anchors.leftMargin: 10
                anchors.topMargin: 4
                anchors.bottomMargin: 4
            }
            ThemeText {
                id: label1
                text: name
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.right: parent.right
                anchors.rightMargin: 10
                verticalAlignment: Text.AlignVCenter
                anchors.left: image1.right
                anchors.leftMargin: 10
            }

            MouseArea {
                id: mouseArea
                anchors.fill: parent
                onClicked: {
                    __root.mediaPlayeritemClicked(name,icon,item_type)
                }
            }
        }
    }
    states: [
        State {
            name: "mediaLibrary"

            PropertyChanges {
                target: mediaLibraryButton
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
