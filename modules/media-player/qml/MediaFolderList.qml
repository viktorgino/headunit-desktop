import QtQuick 2.11
import QtQuick.Controls 2.4

Item {
    id: __media_folder_list
    clip: true
    property var model;
    property url icon;
    property string name;
    property string item_type;
    signal itemClicked(string item_type, int id, string path, string name, url thumbnail)

    ListView {
        id: listView
        boundsBehavior: Flickable.DragOverBounds
        clip: true
        anchors.top: header.bottom
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.topMargin: 0
        model: __media_folder_list.model
        delegate: MediaFolderListItem {
            onItemClicked: __media_folder_list.itemClicked(item_type,id,path,name,thumbnail)
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
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 10
            anchors.top: parent.top
            anchors.topMargin: 10
            source: __media_folder_list.icon
            mipmap:true
        }

        Text {
            id: text1
            color: "#ffffff"
            text: qsTr(__media_folder_list.name)
            anchors.left: image.right
            anchors.leftMargin: 10
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.verticalCenter: parent.verticalCenter
            font.pixelSize: 16
        }
    }

}
