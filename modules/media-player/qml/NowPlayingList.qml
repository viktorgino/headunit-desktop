import QtQuick 2.11
import QtQuick.Controls 2.4

Item {
    id: __now_playing_list
    clip: true
    property var model
    property url icon
    property string name
    property string item_type
    property int currentPlaying : 0
    signal itemClicked(int index)
    onCurrentPlayingChanged: {
        listView.positionViewAtIndex(currentPlaying,ListView.Beginning);
    }

    Rectangle {
        id: rectangle1
        color: "#212121"
        anchors.fill: parent
    }

    ListView {
        id: listView
        boundsBehavior: Flickable.DragOverBounds
        clip: true
        anchors.top: header.bottom
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.topMargin: 0
        model: parent.model
        delegate: NowPlayingListItem {
            currentPlaying: __now_playing_list.currentPlaying
            onItemClicked: __now_playing_list.itemClicked(index)
        }
        ScrollBar.vertical: ScrollBar {
        }
    }

    Item {
        id: header
        height: parent.height * 0.15
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0

        Rectangle {
            id: rectangle
            color: "#424242"
            anchors.fill: parent
        }

        Image {
            id: image
            width: height
            source: "qrc:/qml/icons/radio-waves.png"
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 10
            anchors.top: parent.top
            anchors.topMargin: 10
            mipmap:true
        }

        Text {
            id: text1
            color: "#ffffff"
            text: "Now Playing"
            anchors.left: image.right
            anchors.leftMargin: 10
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.verticalCenter: parent.verticalCenter
            font.pixelSize: 16
        }
    }


}
