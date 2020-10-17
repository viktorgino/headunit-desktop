import QtQuick 2.6
import QtGraphicalEffects 1.0
import QtQuick.Layouts 1.0

Item {
    id:__root

    property int value : 0
    property int steps : 3
    Image {
        id:icon
        visible: false
        anchors.bottom: seat_heat.top
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottomMargin: 0
        fillMode: Image.PreserveAspectFit
        source: "qrc:/HVAC/icons/seat-heat.svg"
        mipmap:true
    }

    ColorOverlay {
        id: overaly
        color: __root.value >= 1?"#ff5722":"#ffffff"
        anchors.fill: icon
        enabled: true
        source: icon
    }

    RowLayout {
        id: seat_heat
        height: parent.height * 0.2
        spacing: 5
        width: parent.width * 0.4
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        Repeater{
            model:__root.steps
            Item {
                Layout.fillHeight: true
                Layout.fillWidth: true
                Rectangle {
                    width: parent.height *0.7
                    height: parent.height *0.7
                    radius: height/2
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                    Layout.fillHeight: false
                    Layout.fillWidth: false
                    color:__root.value >= index+1?"#ff5722":"#FFFFFF"
                }
            }
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: __root.value == __root.steps?__root.value = 0:__root.value++
    }
}
