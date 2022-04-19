import QtQuick 2.6
import QtQuick.Controls 2.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.1
import QtGraphicalEffects 1.0

Item {
    id:__root
    property int steps : pluginContext.hvacSettings.TemperatureSteps
    property double stepSize : pluginContext.hvacSettings.TemperatureStepSize
    property int offset : pluginContext.hvacSettings.TemperatureOffset

    property int value: 0
    property bool inverted : false
    property int tickFontSize: 14
    signal updateValue(var setValue)

    LinearGradient {
        anchors.fill: parent
        gradient: Gradient {
            GradientStop {
                position: 0
                color: "#1d252d"
            }
            GradientStop {
                position: 0.7
                color: "#222a32"
            }

            GradientStop {
                position: 1
                color: "#222627"
            }
        }
        start: Qt.point(0, 0)
        end: Qt.point(0, parent.height)
    }

    Item {
        id:__slider_ticks
        x: __root.inverted?0:parent.width - width
        width:parent.width*0.4
        anchors.top: parent.top
        anchors.topMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0

        Rectangle {
            color: "#33ffffff"
            anchors.fill: parent
        }

        ColumnLayout {
            id:__slider_ticks_column
            anchors.bottomMargin: 30
            anchors.topMargin: 30
            anchors.fill: parent
            Item{
                Layout.fillHeight: true
                Layout.fillWidth: true
                Text {
                    color: (__root.value === __root.steps - 1)?"#ffffff":"#212121"
                    text: qsTr("High")
                    anchors.verticalCenter: parent.verticalCenter
                    verticalAlignment: Text.AlignBottom
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.pixelSize: (__root.value === __root.steps - 1)?24:14
                }
            }
            Repeater {
                model:__root.steps - 2
                Item{
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    property double dspNum : (__root.steps - (index * __root.stepSize) ) - 2
                    Text {
                        color: (dspNum == Math.floor(__root.value))?"#ffffff":"#212121"
                        text: __root.offset + dspNum
                        anchors.verticalCenter: parent.verticalCenter
                        verticalAlignment: Text.AlignBottom
                        anchors.horizontalCenter: parent.horizontalCenter
                        font.pixelSize: (dspNum == Math.floor(__root.value))?24:14
                    }
                }
            }

            Item{
                Layout.fillHeight: true
                Layout.fillWidth: true
                Text {
                    color: (__root.value === 0)?"#ffffff":"#212121"
                    text: qsTr("Low")
                    anchors.verticalCenter: parent.verticalCenter
                    verticalAlignment: Text.AlignBottom
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.pixelSize: (__root.value === 0 )?24:14
                }
            }
        }
    }

    Item {
        id: __slider
        x: __root.inverted?parent.width - width:0
        width:parent.width*0.6
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 30
        anchors.topMargin: 30

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
            //                closeTimer.restart()
            __root.updateValue(value);
        }

        Item {
            width: 5
            height: parent.height
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.topMargin: 10
            anchors.bottomMargin: 10
            anchors.horizontalCenter: parent.horizontalCenter

            Rectangle {
                color: "#ffffff"
                anchors.fill: parent
                LinearGradient {
                    anchors.fill: parent
                    gradient: Gradient {
                        GradientStop {
                            position: 0
                            color: "#d50000"
                        }
                        GradientStop {
                            position: 0.25
                            color: "#FF3D00"
                        }
                        GradientStop {
                            position: 0.5
                            color: "#00C853"
                        }
                        GradientStop {
                            position: 0.75
                            color: "#00E5FF"
                        }
                        GradientStop {
                            position: 1
                            color: "#0091EA"
                        }
                    }
                    start: Qt.point(0, 0)
                    end: Qt.point(0, parent.height)
                }
            }
        }
        Rectangle {
            id: rectangle
            y: parent.height - ( (parent.height / __root.steps) * (__root.value + 1) )
            width: 25
            height: 25
            radius: height/2
            anchors.horizontalCenter: parent.horizontalCenter
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
        onPressed: {
            if(mouse.y > 0 && mouse.y < height){
                var value = Math.floor(mouse.y / (height / __root.steps))
                __slider.setValue(__root.steps - value - 1)
            }
        }
        onPositionChanged: {
            if(mouse.y > 0 && mouse.y < height){
                var value = Math.floor(mouse.y / (height / __root.steps))
                __slider.setValue(__root.steps - value - 1)
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

/*##^##
Designer {
    D{i:0;autoSize:true;formeditorColor:"#4c4e50";height:480;width:640}
}
##^##*/
