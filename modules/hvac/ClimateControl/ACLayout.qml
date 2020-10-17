import QtQuick 2.6
import QtGraphicalEffects 1.0
import QtQuick.Layouts 1.0

Item {
    id: __cclayout
    anchors.fill: parent
    property int right_seat_heat_rate : 0

    Item {
        id: item4
        anchors.bottomMargin: parent.height * 0.05
        anchors.bottom: bottomBar.top
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.top: parent.top


        Item {
            id: item5
            anchors.bottom: fan_sliders.top
            anchors.top: parent.top
            anchors.topMargin: 0
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.right: parent.right

            FanDirection {
                id: fan_direction
                x: 64
                y: 38
                width: parent.width * 0.8
                height: parent.height * 0.8
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
            }
        }

        ColumnLayout{
            id:fan_sliders
            height: parent.height *0.3
            spacing: parent.height * 0.1
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.right: parent.right
            anchors.rightMargin: 0

            FanSlider {
                visible: (HVACPlugin.settings.fan1 == "true")
                id: fan_slider1
                Layout.fillHeight: true
                Layout.fillWidth: true
                min:HVACPlugin.settings.fan2_min
                max:HVACPlugin.settings.fan2_max
                value: HVACPlugin.parameters.Fan1
                onValueChanged: HVACPlugin.setParameterValue("Fan1", value)
            }

            FanSlider {
                visible: (HVACPlugin.settings.fan2 == "true")
                id: fan_slider2
                Layout.fillHeight: true
                Layout.fillWidth: true
                min:HVACPlugin.settings.fan2_min
                max:HVACPlugin.settings.fan2_max
                value: HVACPlugin.parameters.Fan2
                onValueChanged: HVACPlugin.setParameterValue("Fan2", value)
            }
        }


    }

    Rectangle {
        id:overlay
        color: "#4c000000"
        visible: false
        anchors.fill: parent

        MouseArea {
            anchors.fill: parent
            onClicked: __cclayout.state = "base state"
        }
    }

    Item {
        id: bottomBar
        height: parent.height * 0.15
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0

        Rectangle {
            color: "#212121"
            anchors.fill: parent
        }

        RowLayout {
            id:bottom_row
            spacing: 0
            anchors.fill: parent

            Item {
                id: item1
                visible: (HVACPlugin.settings.temp1 == "true")
                Layout.fillHeight: true
                Layout.fillWidth: true

                Text {
                    id: front_temp
                    color: "#ffffff"
                    text: qsTr(left_tempslider.value.toString())
                    font.family: "Tahoma"
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: parent.height*0.6
                }


                Text {
                    color: "#ffffff"
                    text: qsTr("°C")
                    anchors.top: front_temp.top
                    anchors.topMargin: 0
                    anchors.left: front_temp.right
                    anchors.leftMargin: 0
                    font.pixelSize: parent.height*0.2
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: __cclayout.state = __cclayout.state == "left slider"?"base state":"left slider"
                }

                TempSlider {
                    id:left_tempslider
                    width:(__cclayout.width/bottom_row.children.length) * 2
                    height: __cclayout.height * 0.85
                    visible: false
                    anchors.bottom: parent.top
                    anchors.bottomMargin: 0
                    anchors.left: parent.left
                    anchors.leftMargin: 0
                    side:"left"
                    min:HVACPlugin.settings.temp1_min
                    max:HVACPlugin.settings.temp1_max
                    stepSize: HVACPlugin.settings.temp1_step_size
                    value: HVACPlugin.parameters.Temperature1
                    onValueChanged: HVACPlugin.setParameterValue("Temperature1", value)
                }
            }

            Item {
                id: item3
                Layout.fillWidth: true
                Layout.fillHeight: true

                SeatHeaterButton{
                    width: parent.width
                    height: parent.height * 0.6
                    anchors.verticalCenter: parent.verticalCenter
                    visible: (HVACPlugin.settings.left_seat_heat == "true")
                    Layout.fillWidth: true
                    Layout.fillHeight: false
                    steps:HVACPlugin.settings.left_seat_heat_steps
                    value: HVACPlugin.parameters.SeatHeatLeft
                    onValueChanged: HVACPlugin.setParameterValue("SeatHeatLeft",value)
                }
            }

            TopButton {
                Layout.fillHeight: true
                Layout.fillWidth: true
                visible: (HVACPlugin.settings.power == "true")
                onClicked: HVACPlugin.toggleParameter("Power")
                active: HVACPlugin.parameters.Power
                imageSrc:"qrc:/HVAC/icons/power.svg"
            }


            TopButton {
                Layout.fillHeight: true
                Layout.fillWidth: true
                visible: (HVACPlugin.settings.autoac == "true")
                onClicked: HVACPlugin.toggleParameter("AutoAc")
                active: HVACPlugin.parameters.AutoAc
                imageSrc:"qrc:/HVAC/icons/automatic-loading.svg"
            }



            Item {
                id: item2
                Layout.fillHeight: true
                Layout.fillWidth: true

                SeatHeaterButton{
                    width: parent.width
                    height: parent.height * 0.6
                    anchors.verticalCenter: parent.verticalCenter
                    visible: (HVACPlugin.settings.right_seat_heat == "true")
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    steps:HVACPlugin.settings.right_seat_heat_steps
                    value: HVACPlugin.parameters.SeatHeatRight
                    onValueChanged: HVACPlugin.setParameterValue("SeatHeatRight", value)
                }
            }

            Item {
                visible: (HVACPlugin.settings.temp2 == "true")
                Layout.fillWidth: true
                Layout.fillHeight: true

                Text {
                    id: back_temp
                    color: "#ffffff"
                    text: right_tempslider.value
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.family: "Tahoma"
                    font.pixelSize: parent.height*0.6
                    anchors.verticalCenter: parent.verticalCenter
                }

                Text {
                    color: "#ffffff"
                    text: qsTr("°C")
                    anchors.leftMargin: 0
                    anchors.topMargin: 0
                    font.pixelSize: parent.height*0.2
                    anchors.left: back_temp.right
                    anchors.top: back_temp.top
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: __cclayout.state = __cclayout.state == "right slider"?"base state":"right slider"
                }

                TempSlider {
                    id:right_tempslider
                    width:(__cclayout.width/bottom_row.children.length) * 2
                    height: __cclayout.height * 0.85
                    visible: false
                    anchors.bottomMargin: 0
                    anchors.bottom: parent.top
                    anchors.right: parent.right
                    anchors.rightMargin: 0
                    side:"right"
                    min:HVACPlugin.settings.temp2_min
                    max:HVACPlugin.settings.temp2_max
                    value: HVACPlugin.parameters.Temperature2
                    stepSize: HVACPlugin.settings.temp2_step_size
                    onValueChanged: HVACPlugin.setParameterValue("Temperature2", value)
                }
            }


        }
    }
    states: [
        State {
            name: "left slider"

            PropertyChanges {
                target: left_tempslider
                visible: true
            }

            PropertyChanges {
                target: overlay
                visible: true
            }

            PropertyChanges {
                target: fan_slider1
                enabled: false
            }

            PropertyChanges {
                target: fan_slider2
                enabled: false
            }
        },
        State {
            name: "right slider"

            PropertyChanges {
                target: right_tempslider
                visible: true
            }

            PropertyChanges {
                target: overlay
                visible: true
            }

            PropertyChanges {
                target: fan_slider1
                enabled: false
            }

            PropertyChanges {
                target: fan_slider2
                enabled: false
            }
        }
    ]

}

/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}D{i:42;anchors_width:200}D{i:26;anchors_height:43.199999999999996;anchors_x:"-80";anchors_y:14}
D{i:29;anchors_height:72;anchors_x:"-91";anchors_y:0}
}
 ##^##*/
