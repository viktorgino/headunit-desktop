import QtQuick 2.6
import QtGraphicalEffects 1.0
import QtQuick.Layouts 1.0

Item {
    id:__root

    property int value : 0
    property int steps : pluginContext.hvacSettings.SeatHeatSteps
    property bool inverted: false
    signal updateValue(var setValue)
    Item {
        height: parent.height * 0.6
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.right: parent.right

        Image {
            id:icon
            visible: false
            anchors.bottom: seat_heat.top
            anchors.right: parent.right
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottomMargin: 0
            fillMode: Image.PreserveAspectFit
            source: "qrc:/HVAC/icons/seat-heat.svg"
            mipmap:true
            mirror: __root.inverted
        }

        ColorOverlay {
            id: overaly
            color: __root.value >= 1?"#ff5722":"#ffffff"
            anchors.fill: icon
            enabled: true
            source: icon
        }

        RowLayout {
            id: seat_heat
            height: parent.height * 0.2
            spacing: 5
            width: parent.width * 0.4
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            Repeater{
                model:__root.steps
                Item {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Rectangle {
                        width: parent.height *0.7
                        height: parent.height *0.7
                        radius: height/2
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        Layout.fillHeight: false
                        Layout.fillWidth: false
                        color:{
                            if(__root.inverted){
                                //                            __root.value <= index+1?"#ff5722":"#FFFFFF"
                            } else {
                            }
                            return __root.value >= index+1?"#ff5722":"#FFFFFF"
                        }
                    }
                }
            }
        }

    }
    MouseArea {
        anchors.fill: parent
        onClicked: __root.updateValue(__root.value == __root.steps? 0 : __root.value + 1)
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;formeditorColor:"#808080";height:480;width:640}
}
##^##*/
