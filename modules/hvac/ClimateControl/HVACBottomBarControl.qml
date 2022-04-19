import QtQuick 2.11
import QtGraphicalEffects 1.0
import QtQuick.Layouts 1.0

Item {
    id: __root

    Rectangle {
        color: "#212121"
        anchors.fill: parent
    }

    RowLayout {
        id:bottom_row
        spacing: 0
        anchors.fill: parent

        Item {
            Layout.fillHeight: true
            Layout.fillWidth: true
            visible: pluginContext.hvacSettings["FrontLeft"].Temperature === true

            HVACDisplayTemperature {
                anchors.fill: parent
                value: pluginContext.parameters.FrontLeft.Temperature
                selected: pluginContext.parameters.TempSelectLeft
            }

            MouseArea {
                anchors.fill: parent
                onClicked: pluginContext.openOverlay(false)//openSlider(false)
            }
        }


        HVACControlSeatHeater {
            Layout.fillWidth: true
            Layout.fillHeight: true
            value: pluginContext.parameters.FrontLeft.SeatHeating
            onUpdateValue: pluginContext.setZoneParameter("FrontLeft", "SeatHeating", setValue)
            visible: pluginContext.hvacSettings["FrontLeft"].SeatHeating === true

        }

        TopButton {
            Layout.fillHeight: true
            Layout.fillWidth: true
            imageSrc:"qrc:/HVAC/icons/windshield-defrost.svg"
            active : pluginContext.parameters.ProgWindscreen
            onClicked: pluginContext.setParameter("ProgWindscreen", !active);
            visible: pluginContext.hvacSettings.FrontDefrost
        }

        TopButton {
            Layout.fillHeight: true
            Layout.fillWidth: true
            imageSrc:"qrc:/HVAC/icons/rear-window-defrost.svg"
            active : pluginContext.parameters.RearDefrost
            onClicked: pluginContext.setParameter("RearDefrost", !active);
            visible: pluginContext.hvacSettings.RearDefrost
        }

        TopButton {
            Layout.fillHeight: true
            Layout.fillWidth: true
            imageSrc:"qrc:/HVAC/icons/auto.svg"
            active : pluginContext.parameters.ProgAuto
            onClicked: pluginContext.setParameter("ProgAuto", !active);
            visible: pluginContext.hvacSettings.Auto
        }

        TopButton {
            Layout.fillHeight: true
            Layout.fillWidth: true
            imageSrc:"qrc:/HVAC/icons/ac-flat.svg"
            active : pluginContext.parameters.AC
            onClicked: pluginContext.setParameter("AC", !active);
            visible: pluginContext.hvacSettings.Ac
        }

        TopButton {
            Layout.fillHeight: true
            Layout.fillWidth: true
            imageSrc:"qrc:/HVAC/icons/recirc.svg"
            active : pluginContext.parameters.Recirculate
            onClicked: pluginContext.setParameter("Recirculate", !active);
            visible: pluginContext.hvacSettings.Recirculate
        }


        HVACControlSeatHeater{
            Layout.fillHeight: true
            Layout.fillWidth: true
            inverse: true
            value: pluginContext.parameters.FrontRight.SeatHeating
            onUpdateValue: pluginContext.setZoneParameter("FrontRight", "SeatHeating", setValue)
            visible: pluginContext.hvacSettings["FrontRight"].SeatHeating === true
        }

        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true
            visible: pluginContext.hvacSettings["FrontRight"].Temperature === true

            HVACDisplayTemperature {
                anchors.fill: parent
                value: pluginContext.parameters.FrontRight.Temperature
                selected: pluginContext.parameters.TempSelectRight
            }

            MouseArea {
                anchors.fill: parent
                onClicked: pluginContext.openOverlay(true)
            }
        }
    }
}
