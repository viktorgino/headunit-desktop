import QtQuick 2.11

Item {
    id: __media_folder_list_item
    height: 50
    width: parent.width

    property string sub_title: "path"
    signal itemClicked(var itemData)
    Image {
        id: thumbnail
        width: height
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.top: parent.top
        anchors.topMargin: 0
        fillMode: Image.PreserveAspectCrop
        source:modelData.thumbnail == ""?"":"file:/"+modelData.thumbnail
        mipmap:true
    }
    Text {
        id: label1
        height: 20
        color: "#ffffff"
        text:modelData.title
        elide: Text.ElideLeft
        clip: true
        verticalAlignment: Text.AlignVCenter
        anchors.top: parent.top
        anchors.topMargin: 5
        anchors.right: parent.right
        anchors.rightMargin: 10
        font.pointSize: 11
        anchors.left: thumbnail.right
        anchors.leftMargin: 10
        font.bold: true
    }


    Text {
        id: label2
        color: "#ffffff"
        text: sub_title == "path"?modelData.path:modelData.count + (modelData.count==1? " Item" : " Items")
        elide: Text.ElideLeft
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 5
        font.bold: true
        anchors.rightMargin: 10
        anchors.leftMargin: 10
        anchors.topMargin: 0
        wrapMode: Text.NoWrap
        anchors.top: label1.bottom
        anchors.left: thumbnail.right
        font.pointSize: 9
        anchors.right: parent.right
        verticalAlignment: Text.AlignVCenter
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onClicked:{
            __media_folder_list_item.itemClicked(modelData);
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
