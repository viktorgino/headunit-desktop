import QtQuick 2.0
import QtGraphicalEffects 1.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

import HUDTheme 1.0


ListView {
    id:__root
    model : pluginContext.CallHistoryModel
    focus: true
    clip: true

    ScrollBar.vertical: ThemeScrollBar { }
    delegate: Item {
        width: __root.width
        height: 50

        MouseArea {
            id: mouseArea
            anchors.fill: parent
            onClicked: {
                if(numbers.length > 0){
                    pluginContext.Handsfree.dialNumber(numbers[0].number)
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

            ImageIcon {
                width: height
                height: parent.height * 0.75
                anchors.verticalCenter: parent.verticalCenter
                source: {
                    if(callType == "MISSED") {
                        return "qrc:/PhoneBluetooth/icons/missed-call.svg";
                    } else if(callType == "RECEIVED") {
                        return "qrc:/PhoneBluetooth/icons/incoming-call.svg";
                    } else if(callType == "DIALED") {
                        return "qrc:/PhoneBluetooth/icons/outgoing-call.svg";
                    }
                }
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }

        ThemeText {
            id: nameText
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: imageWrapper.right
            anchors.leftMargin: 6
            text: name ? name : numbers[0].number
            elide: Text.ElideRight
            font.bold: true
        }
    }
}

