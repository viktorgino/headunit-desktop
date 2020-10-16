import QtQuick 2.11

Item {
    id:__media_drawer
    property int top_margin: 0
    signal itemClicked(string name,url icon, string item_type)
    Rectangle {
        id: rectangle
        color: "#212121"
        anchors.fill: parent
    }

    ListView {
        id: listView
        clip: true
        anchors.topMargin: top_margin
        anchors.fill: parent
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
        delegate: MediaDrawerItem {
            onItemClicked: __media_drawer.itemClicked(name,icon,item_type)
        }
    }

}
