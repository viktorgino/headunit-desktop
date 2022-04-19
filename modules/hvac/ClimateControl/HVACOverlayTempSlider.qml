import QtQuick 2.11
import QtGraphicalEffects 1.0
import QtQuick.Layouts 1.0
import QtQml 2.11

Item {
    id:__root
    property bool rightHand : false

    Rectangle {
        id:overlay
        color: "#4c000000"
        anchors.fill: parent

        MouseArea {
            anchors.fill: parent
            onClicked: pluginContext.closeOverlay()
        }
    }

    HVACControlTempSlider {
        id:__tempslider_left
        width: parent.width * 0.25
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        visible: !__root.rightHand
        inverted: false
        value: pluginContext.parameters.FrontLeft.Temperature
        onUpdateValue: {
            pluginContext.openOverlay(false)
            pluginContext.setZoneParameter("FrontLeft", "Temperature", setValue)
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
        value: pluginContext.parameters.FrontRight.Temperature
        onUpdateValue: {
            pluginContext.openOverlay(true)
            pluginContext.setZoneParameter("FrontRight", "Temperature", setValue)
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
