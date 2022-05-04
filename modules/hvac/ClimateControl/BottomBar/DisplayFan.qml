import QtQuick 2.11
import QtQuick.Controls 2.3
import QtGraphicalEffects 1.0

import "../"
Item {
    id: __root
    required property QtObject pluginContext
    property alias inverted : __fan.inverted
    property string zone : "FrontLeft"
    HVACDisplayFan {
        id: __fan
        width: height
        height: parent.height
        inverted: false
        autoEnabled: pluginContext.parameters.ProgAutoFanFront
        selected: pluginContext.parameters.FanSelectLeft
        value: pluginContext.parameters[__root.zone].Fan
//        visible: pluginContext.hvacSettings[__root.zone].Fan === true && !pluginContext.parameters.ProgAuto

    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;formeditorColor:"#c0c0c0";formeditorZoom:4;height:50;width:50}
}
##^##*/
