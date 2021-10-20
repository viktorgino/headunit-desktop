import QtQuick 2.11
import QtGraphicalEffects 1.0
import HUDTheme 1.0
Item {
    id: __root
    height: 50
    width: parent.width

    property int currentPlaying : 0

    signal itemClicked()
    signal setFavourite()

    ThemeText {
        id: label1
        height: 20
        text: stationName.trim()
        clip: true

        anchors.top: parent.top
        anchors.topMargin: 5
        anchors.right: favouriteButton.left
        anchors.rightMargin: 10
        anchors.left: parent.left
        anchors.leftMargin: 10
        fontSizeMode: Text.VerticalFit
        verticalAlignment: Text.AlignVCenter
    }


    ThemeText {
        id: label2
        text: channelName
        subtext:true

        anchors.bottom: parent.bottom
        anchors.bottomMargin: 5
        anchors.leftMargin: 10
        anchors.topMargin: 0
        anchors.top: label1.bottom
        anchors.left: parent.left

        elide: Text.ElideLeft
        wrapMode: Text.NoWrap
        verticalAlignment: Text.AlignVCenter
    }
    ThemeText {
        id: label3
        text: "0x" + stationSId.toString(16).toUpperCase()
        subtext:true

        anchors.bottom: parent.bottom
        anchors.bottomMargin: 5
        anchors.rightMargin: 10
        anchors.leftMargin: 8
        anchors.topMargin: 0
        anchors.top: label1.bottom
        anchors.left: label2.right
        anchors.right: favouriteButton.left

        elide: Text.ElideLeft
        wrapMode: Text.NoWrap
        verticalAlignment: Text.AlignVCenter
    }



    Item {
        id: favouriteButton
        width: 50
        height: 50
        anchors.verticalCenter: parent.verticalCenter
        anchors.rightMargin: 24
        anchors.right: parent.right

        Image {
            id: favoriteButtonImage
            width: 25
            height: 25
            anchors.verticalCenter: parent.verticalCenter
            source: "qrc:/qml/icons/star.png"
            anchors.horizontalCenter: parent.horizontalCenter
            fillMode: Image.PreserveAspectCrop
            mipmap:true
        }
        ColorOverlay {
            anchors.fill: favoriteButtonImage
            source: favoriteButtonImage
            color:favorit?"#ffff32":"#aaaaaa"
        }
        MouseArea{
            anchors.fill: parent
            onClicked: {
                favorit = !favorit
                __root.setFavourite()
            }
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
        anchors.left: parent.left
        anchors.right: favouriteButton.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.rightMargin: 8
        onClicked: {
            __root.itemClicked()
        }
    }
}

