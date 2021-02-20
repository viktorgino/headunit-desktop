import QtQuick 2.11
import QtQuick.Controls 2.3
import QtGraphicalEffects 1.12

Item {
    id: __root
    property alias selected: tempSelected.visible
    property double value : 0

    property int steps : HVACPlugin.hvacSettings.TemperatureSteps
    property double stepSize : HVACPlugin.hvacSettings.TemperatureStepSize
    property int offset : HVACPlugin.hvacSettings.TemperatureOffset

    Item {
        id: item2
        anchors.fill: parent
        anchors.rightMargin: parent.width * 0.1
        anchors.leftMargin: parent.width * 0.1
        anchors.topMargin: parent.height * 0.1
        Text {
            id: back_temp
            color: "#ffffff"
            visible: (__root.value != __root.steps/__root.stepSize && __root.value !== 0)
            text: __root.offset + (__root.stepSize * __root.value)
            anchors.horizontalCenter: parent.horizontalCenter
            font.family: "Tahoma"
            font.pixelSize: parent.height*0.6
            anchors.verticalCenter: parent.verticalCenter
        }

        Text {
            id:tempUnit
            visible: back_temp.visible
            color: "#ffffff"
            text: "°C"
            anchors.leftMargin: 0
            anchors.topMargin: 0
            font.pixelSize: parent.height*0.2
            anchors.left: back_temp.right
            anchors.top: back_temp.top
        }

        Item {
            id: fanSelectWrapper
            height: parent.height * 0.2
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom

            Rectangle {
                id: tempSelected
                width: parent.width * 0.8
                height: parent.height * 0.5
                color: "#ffffff"
                radius: height / 2
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }

        Text {
            width: parent.width * 0.75
            height: parent.height * 0.5
            visible: !back_temp.visible
            color: "#ffffff"
            text: __root.value === 0 ? qsTr("Low") : qsTr("High")
            anchors.verticalCenter: parent.verticalCenter
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.bold: true
            font.capitalization: Font.AllUppercase
            font.pointSize: 200
            fontSizeMode: Text.Fit
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;formeditorColor:"#808080";height:480;width:640}
}
##^##*/
