import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.0

import HUDPlugins 1.0
import HUDTheme 1.0

ThemeRoot {
    id: __root
    Rectangle {
        id:overlay
        color: "#4c000000"
        anchors.fill: parent

        MouseArea {
            anchors.fill: parent
            onClicked: pluginContext.closeOverlay()
        }
    }
    ListModel {
        id: soundSettings
        ListElement {
            title : "Balance"
            action : "balance"
        }
        ListElement {
            title : "Sub"
            action : "sub"
        }
        ListElement {
            title : "Bass"
            action : "bass"
        }
        ListElement {
            title : "Middle"
            action : "middle"
        }
        ListElement {
            title : "Treble"
            action : "treble"
        }
    }
    ListView {
        id:listView
        anchors.fill: parent
        clip: true
        anchors.topMargin: 50
        model : soundSettings
        currentIndex : 0
        delegate: Item {
            id: delegateItem
            height: 100
            width : listView.width - 20

            ThemeHeaderText{
                level:2
                text: title
                anchors.horizontalCenter: parent.horizontalCenter
                color : delegateItem.ListView.isCurrentItem ? "#ffffff" : "#323232"
            }

            MouseArea {
                anchors.fill: parent
                propagateComposedEvents: true
                onClicked: {
                    listView.currentIndex = index
                }
            }

            SliderCenter{
                height: 40
                width: parent.width
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: 0
                wheelEnabled: false
                tickHeight:40
                color : delegateItem.ListView.isCurrentItem ? "#ffffff" : "#323232"
                onPressedChanged: {
                    listView.currentIndex = index
                }
                onValueChanged: {
                    pluginContext.setAudioParameter(action, value)
                }
            }
        }
        highlight  : Rectangle {
            height: 40
            width : listView.width - 20
            color: "#33ff5e00"

        }

        ScrollBar.vertical: ThemeScrollBar {

        }
    }

    Button {
        id: button
        x: 8
        y: 8
        text: qsTr("Sound Button")
        onClicked : {
            if(listView.currentIndex + 1 < listView.count){
                listView.currentIndex += 1
            } else {
                listView.currentIndex = 0
            }
        }
    }
}

/*##^##
Designer {
D{i:0;autoSize:true;formeditorColor:"#808080";height:480;width:640}
}
##^##*/
