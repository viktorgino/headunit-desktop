import QtQuick 2.6
import QtQuick.Controls 2.0

Item {
    id:__media_list
    signal back()
    property var model;
    property url thumbnail: "qrc:/qml/images/music_placeholder.jpg"
    property string title: "Title"
    property string sub_title:"Sub title"
    signal itemClicked(string path)
    Item {
        id: container_info
        height: parent.height*0.4
        anchors.top: parent.top
        anchors.topMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0

        Item {
            id: item1
            width: height
            height: parent.height * 0.6
            anchors.horizontalCenterOffset: 0
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: 8

            Image {
                id: thumbnail
                anchors.fill: parent
                fillMode: Image.PreserveAspectCrop
                source: __media_list.thumbnail
                mipmap:true
            }
        }

        Item {
            id: item2
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            anchors.rightMargin: 0
            anchors.right: parent.right
            anchors.leftMargin: 0
            anchors.topMargin: 0
            anchors.left: parent.left
            Text {
                id: title
                color: "#ffffff"
                text: __media_list.title
                anchors.topMargin: 0
                font.pixelSize: 18
                anchors.bottomMargin: 152
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
            }

            Text {
                id: sub_title
                color: "#ffffff"
                text: __media_list.sub_title
                anchors.topMargin: 30
                font.pixelSize: 12
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
            }
            anchors.top: item1.bottom
        }

        Image {
            id: image
            width: 50
            height: 50
            anchors.top: parent.top
            anchors.topMargin: 8
            anchors.left: parent.left
            anchors.leftMargin: 8
            source: "qrc:/qml/icons/svg/android-arrow-back.svg"

            MouseArea {
                id: mouseArea
                anchors.fill: parent
                onClicked: back()
            }
        }

    }

    ListView {
        id: listView
        clip: true
        anchors.top: container_info.bottom
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.topMargin: 0
        model: __media_list.model
        delegate: MediaListItem{
            onItemClicked: {
                __media_list.model[index].playNow = true;
                __media_list.itemClicked(path);
            }
        }
        ScrollBar.vertical: ScrollBar {
        }
    }
}
