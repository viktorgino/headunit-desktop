import QtQuick 2.6
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.0
import QtQml 2.11
import QtGraphicalEffects 1.0

Item{
    id:__root
    state: "closed"
    property int steps : pluginContext.hvacSettings.FanSteps
    property bool slider_enabled: true
    property int value: 0
    property bool inverted : false
    property bool selected: false
    property bool autoEnabled: false
    property string zone: ""
    signal opening()
    signal updateValue(var setValue)

    function openSlider(){
        __root.opening()
        __root.state = ""
        closeTimer.restart()
    }
    function closeSlider(){
        __root.state = "closed"
    }

    function toggleSlider() {
        if(__root.state == "") {
            closeSlider()
        } else {
            openSlider()
        }
    }

    Timer {
        id:closeTimer
        interval: 5000; running: false; repeat: false
        onTriggered: __root.closeSlider()
    }

    Item {
        id:slider_wrapper
        width: parent.width
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        Item {
            id: left_icon
            width: 100
            height: 100
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter

            Image {
                id:left_icon_image
                width: 60
                height: 60
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                source: "qrc:/HVAC/icons/ac.svg"
                sourceSize: Qt.size(parent.width, parent.height)
            }
        }

        Item {
            id: right_icon
            width: 100
            height: 100
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right

            Image {
                id: right_icon_image
                width: 75
                height: 75
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                source: "qrc:/HVAC/icons/ac.svg"
                visible: true
                sourceSize: Qt.size(parent.width, parent.height)
            }
        }


        Item {
            id: __slider
            anchors.left: left_icon.right
            anchors.right: right_icon.left
            anchors.bottom: parent.bottom
            anchors.top: parent.top

            function increaseValue() {
                if(__root.value < __root.steps - 1) {
                    setValue(__root.value + 1);
                }
            }

            function decreaseValue() {
                if(__root.value > 0 ) {
                    setValue(__root.value - 1);
                }
            }

            function setValue(value) {
                closeTimer.restart()
                __root.updateValue(value);
            }

            RowLayout {
                anchors.fill: parent
                spacing: 0
                Repeater{
                    model: __root.steps
                    Item {
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        Rectangle {
                            width: 15
                            height: 15
                            color: "#ffffff"
                            radius: width
                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.verticalCenter: parent.verticalCenter
                        }
                    }
                }
            }
            Item {
                anchors.fill: parent
                anchors.rightMargin: parent.width/__root.steps/2
                anchors.leftMargin: parent.width/__root.steps/2

                Rectangle {
                    id: rectangle
                    x: (- width / 2)  + (((parent.width + width) / __root.steps) * __root.value)
                    width: 40
                    height: 40
                    radius: height/2
                    anchors.verticalCenter: parent.verticalCenter
                    color: "#4cffffff"
                    Rectangle {
                        color: "#ffffff"
                        width: parent.width * 0.8
                        height: width
                        radius: height/2
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.horizontalCenter: parent.horizontalCenter
                    }
                }
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    if(mouse.x > 0 && mouse.x < width){
                        var value = Math.floor(mouse.x / (width / __root.steps))
                        __slider.setValue(value)
                    }
                }
                onPositionChanged: {
                    if(mouse.x > 0 && mouse.x < width){
                        var value = Math.floor(mouse.x / (width / __root.steps))
                        __slider.setValue(value)
                    }
                }

                onWheel: {
                    if(wheel.angleDelta.y > 0){
                        __slider.increaseValue();
                    } else {
                        __slider.decreaseValue();
                    }
                }
            }

        }
    }

    HVACDisplayFan {
        id: fanDisplayIcon
        width: 150
        visible: false
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 0
        inverted: __root.inverted
        steps: __root.steps
        value: __root.value
        selected: __root.selected
        autoEnabled: __root.autoEnabled

        MouseArea {
            id: leftMouse
            anchors.fill: parent
            onClicked: toggleSlider()
            onWheel: {
                if(wheel.angleDelta.y > 0){
                    __slider.increaseValue()
                } else {
                    __slider.decreaseValue()
                }
            }
        }
    }

    transitions: [
        Transition {
            from: "*"; to: "*";
            NumberAnimation {
                easing.type: Easing.InOutQuad;
                properties: "x,width,opacity,height";
                duration: 400;
            }
        }
    ]
    states: [
        State {
            name: "closed"
            PropertyChanges {
                target: slider_wrapper
                width: 0
                opacity: 0
            }

            PropertyChanges {
                target: fanDisplayIcon
                visible: true
            }
        }
    ]

}


/*##^##
Designer {
    D{i:0;autoSize:true;formeditorColor:"#000000";formeditorZoom:0.8999999761581421;height:100;width:1080}
}
##^##*/
