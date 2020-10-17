import QtQuick 2.6
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.1
import QtGraphicalEffects 1.0

Item {
    id:__root
    property int min: 0
    property int max: 6
    property alias value: __tempslider.value
    property string side: "right"
    property int tickFontSize: 14
    property double stepSize : 1

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
        width:parent.width*0.4
        anchors.top: parent.top
        anchors.topMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0

        anchors.right: (side == "left")?parent.right:undefined

        Rectangle {
            color: "#33ffffff"
            anchors.fill: parent
        }

        ColumnLayout {
            id:__slider_ticks_column
            anchors.bottomMargin: 30
            anchors.topMargin: 30
            anchors.fill: parent

            Repeater{
                model:__tempslider.stepSize > 0 ? 1 + (__tempslider.maximumValue - __tempslider.minimumValue) / Math.ceil(__tempslider.stepSize) : 0
                Item{
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    property double dspNum : __tempslider.maximumValue - (index * Math.ceil(__tempslider.stepSize))
                    Text {
                        color: (dspNum == Math.floor(__tempslider.value))?"#ffffff":"#212121"
                        text: qsTr(dspNum.toString())
                        anchors.verticalCenter: parent.verticalCenter
                        verticalAlignment: Text.AlignBottom
                        anchors.horizontalCenter: parent.horizontalCenter
                        font.pixelSize: (dspNum == Math.floor(__tempslider.value))?24:14
                    }
                    MouseArea {
                        anchors.top: parent.top
                        anchors.right: parent.right
                        anchors.bottom: parent.bottom
                        anchors.left: parent.left
                        onClicked: {
                            __tempslider.value = dspNum;
                        }
                    }
                }
            }
        }

        MouseArea {
            anchors.top: __slider_ticks_column.top
            anchors.right: parent.right
            anchors.bottom: __slider_ticks_column.bottom
            anchors.left: parent.left
            propagateComposedEvents:true
            onWheel:{
                __tempslider.value = __tempslider.value + __tempslider.stepSize * (wheel.angleDelta.y/Math.abs(wheel.angleDelta.y));
            }
        }
    }

    Item {
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: (side == "left")?__slider_ticks.left:parent.right
        anchors.left: (side == "left")?parent.left:__slider_ticks.right

        Text {
            id: top_text
            height: 30
            color: "#ffffff"
            text: qsTr("HIGH")
            font.family: "Verdana"
            anchors.right: parent.right
            anchors.left: parent.left
            anchors.top: parent.top
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 14
        }

        Slider {
            id: __tempslider
            property int steps: __tempslider.stepSize > 0 ? 1 + (__tempslider.maximumValue - __tempslider.minimumValue) / __tempslider.stepSize : 0
            property int margin : (__slider_ticks_column.height/steps )/2 - 12.5
            anchors.topMargin: margin
            anchors.bottomMargin: margin
            anchors.top: top_text.bottom
            anchors.right: parent.right
            anchors.bottom: bottom_text.top
            anchors.left: parent.left
            minimumValue: __root.min
            value: 20
            orientation: Qt.Vertical
            tickmarksEnabled: false
            updateValueWhileDragging: true
            maximumValue: __root.max
            stepSize: __root.stepSize

            style: SliderStyle {
                groove:Item{
                    implicitHeight: 5
                    Rectangle {
                        anchors.verticalCenter: parent.verticalCenter
                        color: "#ffffff"
                        anchors.fill: parent
                        LinearGradient {
                            anchors.fill: parent
                            gradient: Gradient {
                                GradientStop {
                                    position: 1
                                    color: "#d50000"
                                }
                                GradientStop {
                                    position: 0.75
                                    color: "#FF3D00"
                                }
                                GradientStop {
                                    position: 0.5
                                    color: "#00C853"
                                }
                                GradientStop {
                                    position: 0.25
                                    color: "#00E5FF"
                                }
                                GradientStop {
                                    position: 0
                                    color: "#0091EA"
                                }
                            }
                            start: Qt.point(0, 0)
                            end: Qt.point(parent.width, 0)
                        }
                    }
                }
                handle: Rectangle {
                    anchors.centerIn: parent
                    color: "#BBDEFB"
                    implicitWidth: 25
                    implicitHeight: 25
                    radius: width/2
                }
            }
        }

        Text {
            id: bottom_text
            height: 30
            color: "#ffffff"
            text: qsTr("LOW")
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 14
            verticalAlignment: Text.AlignVCenter
            font.family: "Verdana"
            anchors.left: parent.left
            anchors.right: parent.right
        }
    }

}
