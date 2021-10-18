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

    Row {
        id:left_controls
        //        width: parent.width > 1000 ? 1000 : parent.width
        width: parent.width/3
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.leftMargin: 0
        anchors.topMargin: 0
        spacing: 5
        HVACDisplayTemperature {
            id: leftTemp
            width: height
            height: parent.height
            value: HVACPlugin.parameters.FrontLeft.Temperature
            selected: HVACPlugin.parameters.TempSelectLeft
            visible: HVACPlugin.hvacSettings["FrontLeft"].Temperature === true

        }

        HVACDisplayFan {
            id: leftFan
            width: height
            height: parent.height
            inverted: false
            autoEnabled: HVACPlugin.parameters.ProgAutoFanFront
            selected: HVACPlugin.parameters.FanSelectLeft
            value: HVACPlugin.parameters.FrontLeft.Fan
            visible: HVACPlugin.hvacSettings["FrontLeft"].Fan === true && !HVACPlugin.parameters.ProgAuto

        }

        HVACDisplayFanDirection {
            id: leftFanDir
            width: height
            height: parent.height
            inverted: false
            zone: "FrontLeft"
            visible: HVACPlugin.hvacSettings["FrontLeft"].Direction === true && !HVACPlugin.parameters.ProgAuto

        }

        HVACControlSeatHeater {
            id: leftSeatHeater
            width: parent.width * 0.25
            height: parent.height
            steps:HVACPlugin.hvacSettings.SeatHeatSteps
            value: HVACPlugin.parameters.FrontLeft.SeatHeating
            visible: HVACPlugin.hvacSettings["FrontLeft"].SeatHeating === true

        }
    }
    RowLayout {
        id:center_controls
        //        width: parent.width > 1000 ? 1000 : parent.width
        width: parent.width /3
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 0
        spacing: 5
        TopButton {
            id: frontDefrost
            Layout.fillHeight: true
            Layout.fillWidth: true
            imageSrc:"qrc:/HVAC/icons/windshield-defrost.svg"
            //            active : true
            visible: HVACPlugin.parameters.ProgWindscreen

        }

        TopButton {
            id: rearDefrost
            Layout.fillHeight: true
            Layout.fillWidth: true
            imageSrc:"qrc:/HVAC/icons/rear-window-defrost.svg"
            //            active : true
            visible: HVACPlugin.parameters.RearDefrost

        }

        TopButton {
            id: autoButton
            Layout.fillHeight: true
            Layout.fillWidth: true
            imageSrc:"qrc:/HVAC/icons/auto.svg"
            //            active : true
            visible: HVACPlugin.parameters.ProgAuto  && !HVACPlugin.parameters.ProgWindscreen

        }

        TopButton {
            id: acButton
            Layout.fillHeight: true
            Layout.fillWidth: true
            imageSrc:"qrc:/HVAC/icons/ac-flat.svg"
            //            active : HVACPlugin.parameters.AC
            visible: HVACPlugin.parameters.AC

        }

        TopButton {
            id: recircButton
            Layout.fillHeight: true
            Layout.fillWidth: true
            imageSrc:"qrc:/HVAC/icons/recirc.svg"
            active : HVACPlugin.parameters.Recirculate
            visible: HVACPlugin.hvacSettings.Recirculate

        }
    }

    Row {
        id:right_controls
        //        width: parent.width > 1000 ? 1000 : parent.width
        width: parent.width /3
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        layoutDirection: Qt.RightToLeft
        anchors.topMargin: 0
        spacing: 5
        HVACDisplayTemperature {
            id: rightTemp
            width: height
            height: parent.height
            value: HVACPlugin.parameters.FrontRight.Temperature
            selected: HVACPlugin.parameters.TempSelectRight
            visible: HVACPlugin.hvacSettings["FrontRight"].Temperature === true

        }

        HVACDisplayFan {
            id: rightFan
            width: height
            height: parent.height
            inverted: true
            autoEnabled: HVACPlugin.parameters.ProgAutoFanFront
            selected: HVACPlugin.parameters.FanSelectRight
            value: HVACPlugin.parameters.FrontRight.Fan
            visible: HVACPlugin.hvacSettings["FrontRight"].Fan === true && !HVACPlugin.parameters.ProgAuto

        }

        HVACDisplayFanDirection {
            id: rightFanDir
            width: height
            height: parent.height
            inverted: true
            zone: "FrontRight"
            visible: HVACPlugin.hvacSettings["FrontRight"].Direction === true && !HVACPlugin.parameters.ProgAuto

        }

        HVACControlSeatHeater{
            id: rightSeatHeater
            width: parent.width * 0.25
            height: parent.height
            inverse: true
            value: HVACPlugin.parameters.FrontRight.SeatHeating
            visible: HVACPlugin.hvacSettings["FrontRight"].SeatHeating === true

        }

    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:60;width:800}
}
##^##*/
