import QtQuick 2.11
import QtQuick.Layouts 1.3

import "../"

Item {
    id:__root
    required property QtObject pluginContext
    property alias inverted : __fanDir.inverted
    property string zone : "FrontLeft"
    HVACDisplayFanDirection {
        id: __fanDir
        width: height
        height: parent.height
        inverted: false
        zone: __root.zone
//        visible: pluginContext.hvacSettings[zone].Direction === true && !pluginContext.parameters.ProgAuto
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;formeditorColor:"#808080";height:480;width:640}
}
##^##*/
