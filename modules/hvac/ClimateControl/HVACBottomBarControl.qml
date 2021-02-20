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
            visible: HVACPlugin.hvacSettings["FrontLeft"].Temperature === true

            HVACDisplayTemperature {
                anchors.fill: parent
                value: HVACPlugin.parameters.FrontLeft.Temperature
                selected: HVACPlugin.parameters.TempSelectLeft
            }

            MouseArea {
                anchors.fill: parent
                onClicked: HVACPlugin.openOverlay(false)//openSlider(false)
            }
        }


        HVACControlSeatHeater {
            Layout.fillWidth: true
            Layout.fillHeight: true
            value: HVACPlugin.parameters.FrontLeft.SeatHeating
            onUpdateValue: HVACPlugin.setZoneParameter("FrontLeft", "SeatHeating", setValue)
            visible: HVACPlugin.hvacSettings["FrontLeft"].SeatHeating === true

        }

        TopButton {
            Layout.fillHeight: true
            Layout.fillWidth: true
            imageSrc:"qrc:/HVAC/icons/windshield-defrost.svg"
            active : HVACPlugin.parameters.ProgWindscreen
            onClicked: HVACPlugin.setParameter("ProgWindscreen", !active);
            visible: HVACPlugin.hvacSettings.FrontDefrost
        }

        TopButton {
            Layout.fillHeight: true
            Layout.fillWidth: true
            imageSrc:"qrc:/HVAC/icons/rear-window-defrost.svg"
            active : HVACPlugin.parameters.RearDefrost
            onClicked: HVACPlugin.setParameter("RearDefrost", !active);
            visible: HVACPlugin.hvacSettings.RearDefrost
        }

        TopButton {
            Layout.fillHeight: true
            Layout.fillWidth: true
            imageSrc:"qrc:/HVAC/icons/auto.svg"
            active : HVACPlugin.parameters.ProgAuto
            onClicked: HVACPlugin.setParameter("ProgAuto", !active);
            visible: HVACPlugin.hvacSettings.Auto
        }

        TopButton {
            Layout.fillHeight: true
            Layout.fillWidth: true
            imageSrc:"qrc:/HVAC/icons/ac-flat.svg"
            active : HVACPlugin.parameters.AC
            onClicked: HVACPlugin.setParameter("AC", !active);
            visible: HVACPlugin.hvacSettings.Ac
        }

        TopButton {
            Layout.fillHeight: true
            Layout.fillWidth: true
            imageSrc:"qrc:/HVAC/icons/recirc.svg"
            active : HVACPlugin.parameters.Recirculate
            onClicked: HVACPlugin.setParameter("Recirculate", !active);
            visible: HVACPlugin.hvacSettings.Recirculate
        }


        HVACControlSeatHeater{
            Layout.fillHeight: true
            Layout.fillWidth: true
            inverse: true
            value: HVACPlugin.parameters.FrontRight.SeatHeating
            onUpdateValue: HVACPlugin.setZoneParameter("FrontRight", "SeatHeating", setValue)
            visible: HVACPlugin.hvacSettings["FrontRight"].SeatHeating === true
        }

        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true
            visible: HVACPlugin.hvacSettings["FrontRight"].Temperature === true

            HVACDisplayTemperature {
                anchors.fill: parent
                value: HVACPlugin.parameters.FrontRight.Temperature
                selected: HVACPlugin.parameters.TempSelectRight
            }

            MouseArea {
                anchors.fill: parent
                onClicked: HVACPlugin.openOverlay(true)
            }
        }
    }
}
