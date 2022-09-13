import QtQuick 2.9
import QtQuick.Window 2.3
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import Qt.labs.settings 1.0

import HUDTheme 1.0
Item {
    RowLayout {
        id: rowLayout
        anchors.fill: parent
        ColumnLayout {
            Layout.minimumWidth: 500
            Layout.alignment: Qt.AlignLeft | Qt.AlignTop
            Layout.rowSpan: 2
            Layout.columnSpan: 2
            Layout.fillWidth: true
            Layout.fillHeight: true
            GridLayout {
                columnSpacing: 0
                rowSpacing: 0
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.minimumHeight: 100
                Layout.minimumWidth: 200
                columns: 2
                rows: 2
                Repeater{
                    model: ["FrontLeft", "FrontRight", "RearLeft", "RearRight"]
                    ColumnLayout {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        property var outerModelData: modelData
                        spacing: 0

                        ThemeFormText {
                            text: modelData
                            Layout.fillWidth: true
                            height: 30
                            font.pointSize: 10
                            font.bold: true
                        }

                        Repeater{
                            model: ["DirUp", "DirCenter", "DirDown", "DirAuto"]
                            PropertyBool {
                                Layout.fillWidth: true
                                Layout.fillHeight: false
                                label : modelData
                                onSwitchClicked: pluginContext.setZoneParameter(outerModelData,modelData,value)
                            }
                        }
                        PropertyByte {
                            Layout.fillWidth: true
                            Layout.fillHeight: false
                            label : "Fan"
                            onValueChanged: pluginContext.setZoneParameter(outerModelData,"Fan",value)
                        }
                        PropertyByte {
                            Layout.fillWidth: true
                            Layout.fillHeight: false
                            label : "Temperature"
                            onValueChanged: pluginContext.setZoneParameter(outerModelData,"Temperature",value)
                        }
                        PropertyByte {
                            Layout.fillWidth: true
                            Layout.fillHeight: false
                            label : "SeatHeating"
                            onValueChanged: pluginContext.setZoneParameter(outerModelData,"SeatHeating",value)
                        }
                    }
                }
            }


            ThemeFormText {
                text: "HVAC Parameter Bits"
                font.bold: true
                font.pointSize: 12
                Layout.fillWidth: true
                height: 30
            }

            GridLayout {
                columns: 3
                Layout.minimumWidth: 100
                Layout.minimumHeight: 100
                Layout.fillHeight: false
                Layout.fillWidth: true
                Repeater{
                    model: ["TempSelectLeft", "TempSelectRight", "FanSelectLeft", "FanSelectRight", "ProgAuto", "ProgAutoFanFront",
                        "ProgAutoFanRear", "ProgWindscreen", "Recirculate", "RearDefrost", "AC"]
                    PropertyBool {
                        Layout.fillWidth: true
                        Layout.fillHeight: false
                        label : modelData
//                        onSwitchClicked: HCUConnection.setParameter(modelData, value)
//                        propertyEnabled: HCUConnection.ClimateControlFrame[modelData]
                    }
                }
            }
        }
    }
}


/*##^##
Designer {
    D{i:0;autoSize:true;formeditorZoom:0.75;height:480;width:640}D{i:6}D{i:8}D{i:7}D{i:9}
D{i:10}D{i:11}D{i:5}D{i:4}D{i:3}D{i:12}D{i:15}D{i:14}D{i:13}D{i:2}D{i:1}
}
##^##*/
