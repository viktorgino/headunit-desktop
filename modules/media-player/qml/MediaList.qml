import QtQuick 2.11
import QtQuick.Controls 2.4

Item {
    id:__root
    signal back()
    property alias model : listView.model;

    property alias title: titleLabel.text
    property alias subtitle: subtitleLabel.text
    property alias thumbnail: thumbnailImage.source

    signal itemClicked(int index)
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
                id: thumbnailImage
                anchors.fill: parent
                fillMode: Image.PreserveAspectCrop
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
                id: titleLabel
                color: "#ffffff"
                anchors.topMargin: 0
                font.pixelSize: 18
                anchors.bottomMargin: 152
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
            }

            Text {
                id: subtitleLabel
                color: "#ffffff"
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
        delegate: MediaListItem{
            onItemClicked: {
                __root.itemClicked(index);
            }
        }
        ScrollBar.vertical: ScrollBar {
        }
    }
}
