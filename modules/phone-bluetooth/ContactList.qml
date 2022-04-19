import QtQuick 2.0
import QtGraphicalEffects 1.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

import HUDTheme 1.0

ListView {
    id: __root
    model : pluginContext.PhonebookModel
    focus: true
    clip: true
    signal contactClicked(var name, var numbers, var image)

    ScrollBar.vertical: ThemeScrollBar { }
    delegate: Item {
        id: delRect
        width: parent.width
        height: 50

        MouseArea {
            id: mouseArea
            anchors.fill: parent
            onClicked: {
                //__root.dial(contact)
                if(__root.isSearching){
                    __root.dial(number.num)
                } else {
                    __root.contactClicked(name, numbers, image)
                }
            }
        }

        Rectangle {
            id: imageWrapper
            width: height
            radius: height/2
            clip: true
            anchors.top: parent.top
            anchors.topMargin: 6
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 6
            anchors.left: parent.left
            anchors.leftMargin: 6
            color:"#00BCD4"


            Item {
                id:icon
                anchors.fill: parent
                layer.enabled: true
                layer.effect: OpacityMask {
                    maskSource: imageWrapper
                }
                property var nameArray : name.split(' ')

                Text {
                    x: -height/5
                    color: "#66ffffff"
                    text: icon.nameArray.length > 0 ? icon.nameArray[0].charAt(0).toUpperCase() : ""
                    font.pixelSize: parent.height * 1.4
                    anchors.bottomMargin: 0
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    anchors.topMargin: 0
                    font.bold: true
                    clip: true
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                }

                Text {
                    x: height/4
                    color: "#4cffffff"
                    text: icon.nameArray.length > 1 ? icon.nameArray[icon.nameArray.length-1].charAt(0).toUpperCase() : ""
                    font.pixelSize: parent.height * 1.4
                    anchors.bottom: parent.bottom
                    anchors.top: parent.top
                    font.bold: true
                    clip: true
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignRight
                }

                Image {
                    id: image1
                    anchors.fill: parent
                    fillMode: Image.PreserveAspectCrop

                    Component.onCompleted: {
                        if(image.size > 0 ){
                            image1.source = 'data:image/jpeg;'+image
                        }
                    }
                }
            }
        }

        ThemeText {
            id: nameText
            anchors {
                verticalCenter: parent.verticalCenter
                left: imageWrapper.right
                leftMargin: 6
            }
            text: name ? name : numbers.length > 0?numbers[0].number : ""
            elide: Text.ElideRight
            font.bold: true
        }

//                Text {
//                    id: numberText
//                    color: "#ffffff"
//                    property string num : {
//                        if(__root.isSearching){
//                            for(var i = 0; contact.phoneNumbers.length > i; i++){
//                                if(String(contact.phoneNumbers[i].number).includes(__root.dialed_num)){
//                                    return contact.phoneNumbers[i].number;
//                                }
//                            }
//                        }
//                        return "";
//                    }
//                    text: {
//                        var numIndex = num.indexOf(__root.dialed_num);
//                        return num.substring(0, numIndex) + "<b>" + __root.dialed_num + "</b>" + num.substring(numIndex+__root.dialed_num.length, num.length);
//                    }
//                    text : number.join('')
//                    text : numbers[0].number

//                    anchors.top: name.bottom
//                    anchors.topMargin: 0
//                    anchors.right: parent.right
//                    anchors.rightMargin: 8
//                    anchors.left: imageWrapper.right
//                    anchors.leftMargin: 8
//                    font.pointSize: parent.height * 0.15
//                }

    }
}
