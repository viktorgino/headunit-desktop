import QtQuick 2.11
import QtGraphicalEffects 1.0
import QtQuick.Layouts 1.0
import QtQml 2.12

Item {
    id:__root
    property bool rightHand : false

    Rectangle {
        id:overlay
        color: "#4c000000"
        anchors.fill: parent

        MouseArea {
            anchors.fill: parent
            onClicked: HVACPlugin.closeOverlay()
        }
    }

    HVACControlTempSlider {
        id:__tempslider_left
        width: parent.width * 0.25
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        visible: !__root.rightHand
        inverted: false
        value: HVACPlugin.parameters.FrontLeft.Temperature
        onUpdateValue: {
            HVACPlugin.openOverlay(false)
            HVACPlugin.setZoneParameter("FrontLeft", "Temperature", setValue)
        }
    }

    HVACControlTempSlider {
        id:__tempslider_right
        width: parent.width * 0.25
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        visible: __root.rightHand
        inverted: true
        value: HVACPlugin.parameters.FrontRight.Temperature
        onUpdateValue: {
            HVACPlugin.openOverlay(true)
            HVACPlugin.setZoneParameter("FrontRight", "Temperature", setValue)
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
