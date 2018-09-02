import QtQuick 2.6
Item {
    id: __drawer_item
    height: 50
    width: parent.width
    signal itemClicked(string name,url icon, string item_type)
    Image {
        id: image1
        width: height
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 5
        anchors.top: parent.top
        anchors.topMargin: 5
        fillMode: Image.PreserveAspectFit
        source:icon
        mipmap:true
    }
    Text {
        id: label1
        color: "#ffffff"
        text: name
        anchors.top: parent.top
        anchors.topMargin: 5
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 5
        anchors.right: parent.right
        anchors.rightMargin: 10
        verticalAlignment: Text.AlignVCenter
        anchors.left: image1.right
        anchors.leftMargin: 10

        font.bold: true
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onClicked: parent.itemClicked(name,icon,item_type)
    }
}

