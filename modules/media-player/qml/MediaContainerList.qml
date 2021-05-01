import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.0

Item {
    id: __root
    clip: true
    property alias model : listView.model
    property url icon
    property string name
    property string item_type
    signal itemClicked(var index)

    ListView {
        id: listView
        anchors.rightMargin: 0
        boundsBehavior: Flickable.DragOverBounds
        clip: true
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.topMargin: parent.height * 0.15
        delegate: MediaContainerListItem {
            onItemClicked: {
                __root.itemClicked(index)
            }
            width: listView.width
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
            source: __root.icon
            mipmap:true
        }

        Text {
            id: text1
            color: "#ffffff"
            text: qsTr(__root.name)
            anchors.left: image.right
            anchors.leftMargin: 10
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.verticalCenter: parent.verticalCenter
            font.pixelSize: 16
        }
    }
    //    AlphabetScrollBar {
    //        id: scroll_bar
    //        width: parent.width*0.05
    //        anchors.top: header.bottom
    //        anchors.topMargin: 0
    //        anchors.bottom: parent.bottom
    //        anchors.bottomMargin: 0
    //        anchors.right: parent.right
    //        anchors.rightMargin: 0
    //        count:listView.count
    //        letters : (model ? model.firstLetters : [] )
    //        topItemFirstLetter:{
    //            if(listView.count > 0){
    //                var name;
    //                var top_item = listView.itemAt(1,listView.contentY)
    //                if(top_item === null){
    //                    name = listView.model[0].name;
    //                } else {
    //                    name = top_item.name;
    //                }
    //                if(typeof(name) !== "undefined"){
    //                    var letter = name.substring(0, 1).toUpperCase();
    //                    if(alphabet.indexOf(letter) === -1){
    //                        letter = alphabet[0];
    //                    }
    //                    return letter;
    //                } else {
    //                    return alphabet[0];
    //                }
    //            } else {
    //                return alphabet[0];
    //            }
    //        }
    //        bottomItemFirstLetter:{
    //            if(listView.count > 0){
    //                var name;
    //                var bottom_item = listView.itemAt(1,listView.contentY+listView.height-1);
    //                if(bottom_item === null){
    //                    name = listView.model[listView.count -1].name;
    //                } else {
    //                    name = bottom_item.name;
    //                }
    //                if(typeof(name) !== "undefined"){
    //                    var letter = name.substring(0, 1).toUpperCase();
    //                    if(alphabet.indexOf(letter) === -1){
    //                        letter = alphabet[0];
    //                    }
    //                    return letter;
    //                } else {
    //                    return alphabet[0];
    //                }
    //            } else {
    //                return alphabet[0];
    //            }
    //        }
    //        onPositionViewAtIndex: listView.positionViewAtIndex(index,mode);
    //    }
}
