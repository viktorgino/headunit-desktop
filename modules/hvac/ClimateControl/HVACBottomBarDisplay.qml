import QtQuick 2.11
import QtGraphicalEffects 1.0
import QtQuick.Layouts 1.0
import QtQml 2.11
Item {
    id: __root
    Rectangle {
        id: rectangle
        color: "#212121"
        anchors.fill: parent
    }

    RowLayout {
        id:bottom_row
        //        width: parent.width > 1000 ? 1000 : parent.width
        width: parent.width
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 0
        spacing: 0
        HVACDisplayTemperature {
            Layout.fillWidth: true
            Layout.fillHeight: true
            value: HVACPlugin.parameters.FrontLeft.Temperature
            selected: HVACPlugin.parameters.TempSelectLeft
            visible: HVACPlugin.hvacSettings["FrontLeft"].Temperature === true
        }

        HVACDisplayFan {
            Layout.fillWidth: true
            Layout.fillHeight: true
            inverted: false
            autoEnabled: HVACPlugin.parameters.ProgAutoFanFront
            selected: HVACPlugin.parameters.FanSelectLeft
            value: HVACPlugin.parameters.FrontLeft.Fan
            visible: HVACPlugin.hvacSettings["FrontLeft"].Fan === true
        }

        HVACDisplayFanDirection {
            Layout.fillWidth: true
            Layout.fillHeight: true
            inverted: false
            zone: "FrontLeft"
            visible: HVACPlugin.hvacSettings["FrontLeft"].Direction === true
        }

        HVACControlSeatHeater {
            Layout.fillWidth: true
            Layout.fillHeight: true
            steps:HVACPlugin.hvacSettings.SeatHeatSteps
            value: HVACPlugin.parameters.FrontLeft.SeatHeating
            visible: HVACPlugin.hvacSettings["FrontLeft"].SeatHeating === true
        }

        TopButton {
            Layout.fillHeight: true
            Layout.fillWidth: true
            imageSrc:"qrc:/HVAC/icons/windshield-defrost.svg"
            active : HVACPlugin.parameters.ProgWindscreen
            visible: HVACPlugin.hvacSettings.FrontDefrost
        }

        TopButton {
            Layout.fillHeight: true
            Layout.fillWidth: true
            imageSrc:"qrc:/HVAC/icons/rear-window-defrost.svg"
            active : HVACPlugin.parameters.RearDefrost
            visible: HVACPlugin.hvacSettings.RearDefrost
        }

        TopButton {
            Layout.fillHeight: true
            Layout.fillWidth: true
            imageSrc:"qrc:/HVAC/icons/auto.svg"
            active : HVACPlugin.parameters.ProgAuto
            visible: HVACPlugin.hvacSettings.Auto
        }

        TopButton {
            Layout.fillHeight: true
            Layout.fillWidth: true
            imageSrc:"qrc:/HVAC/icons/ac-flat.svg"
            active : HVACPlugin.parameters.AC
            visible: HVACPlugin.hvacSettings.Ac
        }

        TopButton {
            Layout.fillHeight: true
            Layout.fillWidth: true
            imageSrc:"qrc:/HVAC/icons/recirc.svg"
            active : HVACPlugin.parameters.Recirculate
            visible: HVACPlugin.hvacSettings.Recirculate
        }


        HVACControlSeatHeater{
            Layout.fillHeight: true
            Layout.fillWidth: true
            inverse: true
            value: HVACPlugin.parameters.FrontRight.SeatHeating
            visible: HVACPlugin.hvacSettings["FrontRight"].SeatHeating === true
        }

        HVACDisplayFanDirection {
            Layout.fillWidth: true
            Layout.fillHeight: true
            inverted: true
            zone: "FrontRight"
            visible: HVACPlugin.hvacSettings["FrontRight"].Direction === true
        }

        HVACDisplayFan {
            Layout.fillWidth: true
            Layout.fillHeight: true
            inverted: true
            autoEnabled: HVACPlugin.parameters.ProgAutoFanFront
            selected: HVACPlugin.parameters.FanSelectRight
            value: HVACPlugin.parameters.FrontRight.Fan
            visible: HVACPlugin.hvacSettings["FrontRight"].Fan === true
        }

        HVACDisplayTemperature {
            Layout.fillWidth: true
            Layout.fillHeight: true
            value: HVACPlugin.parameters.FrontRight.Temperature
            selected: HVACPlugin.parameters.TempSelectRight
            visible: HVACPlugin.hvacSettings["FrontRight"].Temperature === true
        }
    }


}

/*##^##
Designer {
    D{i:0;autoSize:true;height:100;width:800}D{i:17}
}
##^##*/
