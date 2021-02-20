import QtQuick 2.11
import QtQuick.Layouts 1.3
import QtQml 2.11

Item {
    id: __root
    property bool inverted: false
    property string zone : ""

    HVACControlFanDirection{
        id:direction
        zone: __root.zone
        inverted:__root.inverted
        visible: HVACPlugin.hvacSettings[__root.zone].Direction === true
        width: parent.width / 2
        x: __root.inverted ? 0 : parent.width/2
        anchors.top: parent.top
        anchors.bottom: parent.bottom
    }

    HVACControlFanSlider {
        id: hVACControlFanSlider
        width: parent.width / 2
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        x: __root.inverted ? parent.width/2 : 0
        inverted: __root.inverted
        visible: HVACPlugin.hvacSettings[__root.zone].Fan === true
        value: HVACPlugin.parameters[__root.zone].Fan
        onUpdateValue: HVACPlugin.setZoneParameter(__root.zone, "Fan", setValue)
        onStateChanged: {
            if(state == "closed"){
                __root.state = ""
            } else {
                __root.state = "open"
            }
        }
    }


    states: [
        State {
            name: "open"

            PropertyChanges {
                target: direction
                width: 0
                opacity: 0
            }

            PropertyChanges {
                target: hVACControlFanSlider
                x: 0
                width: parent.width
            }
        }
    ]
}

/*##^##
Designer {
    D{i:0;autoSize:true;formeditorColor:"#808080";height:500;width:800}
}
##^##*/
