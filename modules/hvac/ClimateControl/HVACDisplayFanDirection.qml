import QtQuick 2.11
import QtQuick.Layouts 1.3

Item {
    id:__root
//    property bool inverted: false
    property bool inverted: true


    property string zone : ""

    Item {
        id: item1
        anchors.fill: parent
        anchors.bottomMargin: parent.height * 0.1
        anchors.rightMargin: parent.width * 0.1
        anchors.leftMargin: parent.width * 0.1
        anchors.topMargin: parent.height * 0.1

        Item {
            width: parent.width>parent.height?parent.height:parent.width
            height: parent.width>parent.height?parent.height:parent.width
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter

            Text {
                x: __root.inverted ? parent.width - width : 0
                width: parent.width * 0.4
                visible: pluginContext.parameters[__root.zone].DirAuto
                color: "#ffffff"
                text: qsTr("Auto")
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                font.pixelSize: 80
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignTop
                font.weight: Font.ExtraBold
                font.bold: true
                fontSizeMode: Text.Fit
                minimumPixelSize: 10
                font.capitalization: Font.AllUppercase
            }

            Item {
                id: columnLayout
                anchors.fill: parent
                visible: !pluginContext.parameters[__root.zone].DirAuto
                Image {
                    id: directionUpImage
                    source: "qrc:/HVAC/icons/zone-top.png"
                    mipmap: true
                    mirror: !__root.inverted
                    fillMode: Image.PreserveAspectFit
                    visible: pluginContext.parameters[__root.zone].DirUp || pluginContext.parameters[__root.zone].DirAuto
                    width: parent.width/2
                    height: parent.height/3
                    x:__root.inverted ? parent.width / 2 : 0
                }

                Image {
                    id: directionCenterImage
                    source: "qrc:/HVAC/icons/zone-middle.png"
                    mipmap: true
                    mirror: !__root.inverted
                    fillMode: Image.PreserveAspectFit
                    visible: pluginContext.parameters[__root.zone].DirCenter  || pluginContext.parameters[__root.zone].DirAuto
                    anchors.verticalCenter: parent.verticalCenter
                    width: parent.width/2
                    height: parent.height/3
                    x:__root.inverted ? parent.width / 2 : 0
                }

                Image {
                    id: directionDownImage
                    source: "qrc:/HVAC/icons/zone-bottom.png"
                    mipmap: true
                    mirror: !__root.inverted
                    fillMode: Image.PreserveAspectFit
                    visible: pluginContext.parameters[__root.zone].DirDown || pluginContext.parameters[__root.zone].DirAuto
                    anchors.bottom: parent.bottom
                    width: parent.width/2
                    height: parent.height/3
                    x:__root.inverted ? parent.width / 2 : 0
                }


            }

            Image {
                id: image
                anchors.fill: parent
                source: "../icons/ac-buttons-single-seat.png"
                mirror: __root.inverted
                fillMode: Image.PreserveAspectFit
                mipmap: true
            }
        }

    }

}

/*##^##
Designer {
    D{i:0;autoSize:true;formeditorColor:"#808080";height:480;width:640}
}
##^##*/
