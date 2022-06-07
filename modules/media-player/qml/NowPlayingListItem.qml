import QtQuick 2.11
import QtGraphicalEffects 1.0
import HUDTheme 1.0
Item {
    id: __now_playing_list_item
    height: 50
    property int currentPlaying : 0

    signal itemClicked(int index)
    ThemeText {
        id: label1
        height: 20
        text: title
        clip: true

        anchors.top: parent.top
        anchors.topMargin: 5
        anchors.right: play_button.left
        anchors.rightMargin: 10
        anchors.left: parent.left
        anchors.leftMargin: 10
        fontSizeMode: Text.VerticalFit
        verticalAlignment: Text.AlignVCenter
    }


    ThemeText {
        id: label2
        text:artist
        subtext:true

        anchors.bottom: parent.bottom
        anchors.bottomMargin: 5
        anchors.rightMargin: 10
        anchors.leftMargin: 10
        anchors.topMargin: 0
        anchors.top: label1.bottom
        anchors.left: parent.left
        anchors.right: play_button.left

        elide: Text.ElideLeft
        wrapMode: Text.NoWrap
        verticalAlignment: Text.AlignVCenter
    }



    Item {
        id: play_button
        width: height
        anchors.rightMargin: 10
        anchors.bottomMargin: 10
        anchors.topMargin: 10
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.top: parent.top

        Image {
            id: playing_button_image
            anchors.fill: parent
            source: "qrc:/qml/icons/play.png"
            fillMode: Image.PreserveAspectCrop
            visible: false
            mipmap:true
        }
        ColorOverlay {
            anchors.fill: playing_button_image
            source: playing_button_image
            color:"#00ff00"
            visible: index==currentPlaying
        }
    }

    Rectangle {
        id: rectangle
        height: 1
        color: "#4cffffff"
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onClicked: {
            __now_playing_list_item.itemClicked(index);
        }
    }

}

