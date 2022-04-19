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
            value: pluginContext.parameters.FrontLeft.Temperature
            selected: pluginContext.parameters.TempSelectLeft
            visible: pluginContext.hvacSettings["FrontLeft"].Temperature === true

        }

        HVACDisplayFan {
            id: leftFan
            width: height
            height: parent.height
            inverted: false
            autoEnabled: pluginContext.parameters.ProgAutoFanFront
            selected: pluginContext.parameters.FanSelectLeft
            value: pluginContext.parameters.FrontLeft.Fan
            visible: pluginContext.hvacSettings["FrontLeft"].Fan === true && !pluginContext.parameters.ProgAuto

        }

        HVACDisplayFanDirection {
            id: leftFanDir
            width: height
            height: parent.height
            inverted: false
            zone: "FrontLeft"
            visible: pluginContext.hvacSettings["FrontLeft"].Direction === true && !pluginContext.parameters.ProgAuto

        }

        HVACControlSeatHeater {
            id: leftSeatHeater
            width: parent.width * 0.25
            height: parent.height
            steps:pluginContext.hvacSettings.SeatHeatSteps
            value: pluginContext.parameters.FrontLeft.SeatHeating
            visible: pluginContext.hvacSettings["FrontLeft"].SeatHeating === true

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
            visible: pluginContext.parameters.ProgWindscreen

        }

        TopButton {
            id: rearDefrost
            Layout.fillHeight: true
            Layout.fillWidth: true
            imageSrc:"qrc:/HVAC/icons/rear-window-defrost.svg"
            //            active : true
            visible: pluginContext.parameters.RearDefrost

        }

        TopButton {
            id: autoButton
            Layout.fillHeight: true
            Layout.fillWidth: true
            imageSrc:"qrc:/HVAC/icons/auto.svg"
            //            active : true
            visible: pluginContext.parameters.ProgAuto  && !pluginContext.parameters.ProgWindscreen

        }

        TopButton {
            id: acButton
            Layout.fillHeight: true
            Layout.fillWidth: true
            imageSrc:"qrc:/HVAC/icons/ac-flat.svg"
            //            active : pluginContext.parameters.AC
            visible: pluginContext.parameters.AC

        }

        TopButton {
            id: recircButton
            Layout.fillHeight: true
            Layout.fillWidth: true
            imageSrc:"qrc:/HVAC/icons/recirc.svg"
            active : pluginContext.parameters.Recirculate
            visible: pluginContext.hvacSettings.Recirculate

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
            value: pluginContext.parameters.FrontRight.Temperature
            selected: pluginContext.parameters.TempSelectRight
            visible: pluginContext.hvacSettings["FrontRight"].Temperature === true

        }

        HVACDisplayFan {
            id: rightFan
            width: height
            height: parent.height
            inverted: true
            autoEnabled: pluginContext.parameters.ProgAutoFanFront
            selected: pluginContext.parameters.FanSelectRight
            value: pluginContext.parameters.FrontRight.Fan
            visible: pluginContext.hvacSettings["FrontRight"].Fan === true && !pluginContext.parameters.ProgAuto

        }

        HVACDisplayFanDirection {
            id: rightFanDir
            width: height
            height: parent.height
            inverted: true
            zone: "FrontRight"
            visible: pluginContext.hvacSettings["FrontRight"].Direction === true && !pluginContext.parameters.ProgAuto

        }

        HVACControlSeatHeater{
            id: rightSeatHeater
            width: parent.width * 0.25
            height: parent.height
            inverse: true
            value: pluginContext.parameters.FrontRight.SeatHeating
            visible: pluginContext.hvacSettings["FrontRight"].SeatHeating === true

        }

    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:60;width:800}
}
##^##*/
