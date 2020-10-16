import QtQuick 2.11

Item {
    id: __media_list_item
    height: 50
    width: parent.width

    signal itemClicked(string path, int index)
//    property string name: modelData.name
    Text {
        id: label1
        height: 20
        color: "#ffffff"
        text: modelData.title
        fontSizeMode: Text.VerticalFit
        clip: true
        verticalAlignment: Text.AlignVCenter
        anchors.top: parent.top
        anchors.topMargin: 5
        anchors.right: parent.right
        anchors.rightMargin: 10
        font.pointSize: 11
        anchors.left: parent.left
        anchors.leftMargin: 10
        font.bold: true
    }


    Text {
        id: label2
        color: "#ffffff"
        text:modelData.artist
        elide: Text.ElideLeft
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 5
        font.bold: true
        anchors.rightMargin: 10
        anchors.leftMargin: 10
        anchors.topMargin: 0
        wrapMode: Text.NoWrap
        anchors.top: label1.bottom
        anchors.left: parent.left
        font.pointSize: 9
        anchors.right: parent.right
        verticalAlignment: Text.AlignVCenter
    }

    MouseArea {
        id: mouseArea
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.rightMargin: 0
        onClicked: {
            __media_list_item.itemClicked(modelData.path, index);
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

}

