import QtQuick 2.6
import QtGraphicalEffects 1.0
import QtQuick.Layouts 1.0
import QtQml 2.11

Item {
    id: __root
    property bool inverted: false
    property string zone : ""
    Item {
        width: parent.width>parent.height?parent.height:parent.width
        height: parent.width>parent.height?parent.height:parent.width
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter

        Image {
            id: image
            anchors.fill: parent
            fillMode: Image.PreserveAspectFit
            source: "qrc:/HVAC/icons/zone-background.png"
            mirror: !__root.inverted
            mipmap:true
        }
        Item {
            id: columnLayout
            anchors.fill: parent

            ImageButton {
                id: directionTop
                width: parent.width/2
                height: parent.height/3
                anchors.top: parent.top
                visible: pluginContext.hvacSettings[__root.zone].DirUp === true
                source: "qrc:/HVAC/icons/zone-top.png"
                mipmap: true
                anchors.topMargin: 0
                fillMode: Image.PreserveAspectFit
                mirror: !__root.inverted
                x:__root.inverted ? parent.width / 2 : 0
                checked: pluginContext.parameters[__root.zone].DirUp
                onClicked: pluginContext.setZoneParameter(__root.zone, "DirUp", !checked)
            }
            ImageButton {
                id: directionCenter
                width: parent.width/2
                height: parent.height/3
                anchors.verticalCenter: parent.verticalCenter
                visible: pluginContext.hvacSettings[__root.zone].DirCenter=== true
                source: "qrc:/HVAC/icons/zone-middle.png"
                mipmap: true
                fillMode: Image.PreserveAspectFit
                mirror: !__root.inverted
                x:__root.inverted ? parent.width / 2 : 0
                checked: pluginContext.parameters[__root.zone].DirCenter
                onClicked: pluginContext.setZoneParameter(__root.zone, "DirCenter", !checked)
            }
            ImageButton {
                id: directionBottom
                width: parent.width/2
                height: parent.height/3
                anchors.bottom: parent.bottom
                visible: pluginContext.hvacSettings[__root.zone].DirDown=== true
                source: "qrc:/HVAC/icons/zone-bottom.png"
                mipmap: true
                fillMode: Image.PreserveAspectFit
                mirror: !__root.inverted
                x:__root.inverted ? parent.width / 2 : 0
                checked: pluginContext.parameters[__root.zone].DirDown
                onClicked: pluginContext.setZoneParameter(__root.zone, "DirDown", !checked)
            }
        }

        Item {
            id: directionAuto
            x:__root.inverted ? 0 : parent.width / 2
            width: parent.width/2
            height: parent.height / 5
            visible: pluginContext.hvacSettings[__root.zone].DirAuto=== true

            ImageButton {
                width: parent.width * 0.6
                height: parent.height * 0.6
                anchors.verticalCenter: parent.verticalCenter
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                source: "qrc:/HVAC/icons/auto.svg"
                anchors.horizontalCenter: parent.horizontalCenter
                fillMode: Image.PreserveAspectFit
                mipmap: true
                smooth: true
                checked: pluginContext.parameters[__root.zone].DirAuto
                onClicked: pluginContext.setZoneParameter(__root.zone, "DirAuto", !checked)
            }
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;formeditorColor:"#4c4e50";height:300;width:800}
}
##^##*/
