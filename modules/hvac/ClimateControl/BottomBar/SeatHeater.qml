import QtQuick 2.11
import QtQuick.Layouts 1.3

import "../"

Item {
    id:__root
    required property QtObject pluginContext
    property alias inverted : __seatHeater.inverted
    property string zone : "FrontLeft"
    HVACControlSeatHeater {
        id: __seatHeater
        width: parent.width * 0.25
        height: parent.height
        steps:pluginContext.hvacSettings.SeatHeatSteps
        value: pluginContext.parameters[__root.zone].SeatHeating
//        visible: pluginContext.hvacSettings[__root.zone].SeatHeating === true

    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;formeditorColor:"#808080";height:480;width:640}
}
##^##*/
