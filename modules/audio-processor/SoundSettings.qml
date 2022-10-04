import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.0
import QtGraphicalEffects 1.12

import HUDTheme 1.0

ThemeRoot {
    id: __root
    Item {
        id:background
        height: 200
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        Rectangle {
            id:overlay
            height: 100
            color: "#212121"
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
        }
        LinearGradient {
            id:gradient
            height: 100
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.leftMargin: 0
            start: Qt.point(0, 0)
            end: Qt.point(0, 100)
            gradient: Gradient {
                GradientStop { position: 0.0; color: "#212121" }
                GradientStop { position: 0.2; color: "#212121" }
                GradientStop { position: 1.0; color: "#00212121" }
            }
        }
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
    Item{
        id:listWrapper
        anchors.fill: background
        anchors.rightMargin: 8
        anchors.leftMargin: 8
        anchors.topMargin: 8
        anchors.bottomMargin: 50
        clip: true

        ListView {
            id:listView
            height: 100
            anchors.left: parent.left
            anchors.right: parent.right
            clip: false
            model : soundSettings
            currentIndex : pluginContext.settingsState - 1
            delegate: Item {
                id: delegateItem
                height: 100
                width : listView.width

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
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;formeditorColor:"#808080";height:600;width:640}D{i:1}
}
##^##*/
