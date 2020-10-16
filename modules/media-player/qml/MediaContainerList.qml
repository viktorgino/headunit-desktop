import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.0

Item {
    id: __media_container_list
    clip: true
    property var model : []
    property url icon
    property string name
    property string item_type
    signal itemClicked(var itemData)

    ListView {
        id: listView
        anchors.rightMargin: 0
        boundsBehavior: Flickable.DragOverBounds
        clip: true
        anchors.top: parent.top
        anchors.right: scroll_bar.left
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.topMargin: parent.height * 0.15
        model: (parent.model ? parent.model.data : [] )
        delegate: switch(item_type){
                  case "playlists":
                  case "folders":
                      media_folder_list_item
                      break;
                  case "songs":
                      media_list_item
                      break;
                  default:
                      media_container_list_item
                      break;
                  }

        Component{
            id:media_container_list_item
            MediaContainerListItem {
                onItemClicked: {
                    itemData.item_type = __media_container_list.item_type;
                    __media_container_list.itemClicked(itemData)
                }
                width: listView.width
            }
        }
        Component{
            id:media_folder_list_item
            MediaFolderListItem {
                sub_title: item_type=="folders"?"path":""
                onItemClicked: {
                    itemData.item_type = __media_container_list.item_type;
                    __media_container_list.itemClicked(itemData)
                }
                width: listView.width
            }
        }
        Component{
            id:media_list_item
            MediaListItem {
                onItemClicked: {
                    __media_container_list.model.data[index].playNow = true;
                    var itemData = {
                        item_type : __media_container_list.item_type,
                        thumbnail:__media_container_list.model.data[index].thumbnail,
                        data:__media_container_list.model.data
                    }
                    __media_container_list.itemClicked(itemData)
                }
                width: listView.width
            }
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
            source: __media_container_list.icon
            mipmap:true
        }

        Text {
            id: text1
            color: "#ffffff"
            text: qsTr(__media_container_list.name)
            anchors.left: image.right
            anchors.leftMargin: 10
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.verticalCenter: parent.verticalCenter
            font.pixelSize: 16
        }
    }
    AlphabetScrollBar{
        id: scroll_bar
        width: parent.width*0.05
        anchors.top: header.bottom
        anchors.topMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0
        count:listView.count
        letters : (model ? model.firstLetters : [] )
        topItemFirstLetter:{
            if(listView.count > 0){
                var name;
                var top_item = listView.itemAt(1,listView.contentY)
                if(top_item === null){
                    name = listView.model[0].name;
                } else {
                    name = top_item.name;
                }
                if(typeof(name) !== "undefined"){
                    var letter = name.substring(0, 1).toUpperCase();
                    if(alphabet.indexOf(letter) === -1){
                        letter = alphabet[0];
                    }
                    return letter;
                } else {
                    return alphabet[0];
                }
            } else {
                return alphabet[0];
            }
        }
        bottomItemFirstLetter:{
            if(listView.count > 0){
                var name;
                var bottom_item = listView.itemAt(1,listView.contentY+listView.height-1);
                if(bottom_item === null){
                    name = listView.model[listView.count -1].name;
                } else {
                    name = bottom_item.name;
                }
                if(typeof(name) !== "undefined"){
                    var letter = name.substring(0, 1).toUpperCase();
                    if(alphabet.indexOf(letter) === -1){
                        letter = alphabet[0];
                    }
                    return letter;
                } else {
                    return alphabet[0];
                }
            } else {
                return alphabet[0];
            }
        }
        onPositionViewAtIndex: listView.positionViewAtIndex(index,mode);
    }
}
