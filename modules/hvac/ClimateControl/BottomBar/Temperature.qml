import QtQuick 2.11
import QtQuick.Layouts 1.3

import "../"

Item {
    id:__root
    required property QtObject pluginContext
    property string zone : "FrontLeft"
    HVACDisplayTemperature {
        id: __temp
        width: height
        height: parent.height
        value: pluginContext.parameters[__root.zone].Temperature
        selected: pluginContext.parameters.TempSelectLeft
        visible: pluginContext.hvacSettings[__root.zone].Temperature === true

    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;formeditorColor:"#808080";height:480;width:640}
}
##^##*/
