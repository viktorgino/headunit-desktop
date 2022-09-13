import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.0

import HUDPlugins 1.0
import HUDTheme 1.0

ThemeRoot {
    id: __root
    Rectangle {
        id:overlay
        color: "#000000"
        anchors.fill: parent
    }

    ListModel {
        id: soundSettings
        ListElement {
            title : "Balance"
            action : "balance"
            sliderFrom: -15
            sliderTo: 15
            sliderStepSize: 1
        }
        ListElement {
            title : "Sub"
            action : "sub"
            sliderFrom: -100
            sliderTo: 15
            sliderStepSize: 5
        }
        ListElement {
            title : "Bass"
            action : "bass"
            sliderFrom: -15
            sliderTo: 15
            sliderStepSize: 1
        }
        ListElement {
            title : "Middle"
            action : "middle"
            sliderFrom: -15
            sliderTo: 15
            sliderStepSize: 1
        }
        ListElement {
            title : "Treble"
            action : "treble"
            sliderFrom: -15
            sliderTo: 15
            sliderStepSize: 1
        }
    }
    ListView {
        id:listView
        anchors.fill: parent
        clip: true
        anchors.topMargin: 50
        model : soundSettings
        currentIndex : pluginContext.settingsState - 1
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
                id:centerSlider
                height: 40
                from:sliderFrom
                to:sliderTo
                stepSize:sliderStepSize
                width: parent.width
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: 0
                wheelEnabled: false
                tickHeight:40
                color : delegateItem.ListView.isCurrentItem ? "#ffffff" : "#323232"
                value : pluginSettings[action]
                onPressedChanged: {
                    listView.currentIndex = index
                }
            }
            Binding {
                target: pluginSettings
                property: action
                value: centerSlider.value
            }

            Connections{
                target: pluginSettings
                onValueChanged:{
                    if(key == action && centerSlider.value != value){
                        centerSlider.value = value
                    }
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
            if(pluginContext.settingsState + 1 < listView.count){
                pluginContext.settingsState += 1
            } else {
                pluginContext.settingsState = 0
            }
        }
    }
    Button {
        id: button2
        x: 200
        y: 8
        text: qsTr("Close Button")
        onClicked : pluginContext.closeOverlay()
    }
}

/*##^##
Designer {
D{i:0;autoSize:true;formeditorColor:"#808080";height:480;width:640}
}
##^##*/
