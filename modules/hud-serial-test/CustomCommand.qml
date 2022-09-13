import QtQuick 2.9
import QtQuick.Window 2.3
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import Qt.labs.settings 1.0

import HUDTheme 1.0

Item {
    ColumnLayout {
        id: columnLayout1
        anchors.fill: parent
        spacing: 0
        ThemeFormText {
            text: "Custom Command Bits"
            font.bold: true
            font.pointSize: 12
            Layout.fillWidth: true
            height: 30
        }
        Repeater{
            model: 8
            PropertyBool {
                Layout.fillWidth: true
                Layout.fillHeight: false
                label : "Custom Bit " + index
//                onSwitchClicked: HCUConnection.setCustomCommandBit(index, value)
//                propertyEnabled: HCUConnection.CustomCommandBits[index]
                editable : true
            }
        }
        ThemeFormText {
            text: "Custom Command Bytes"
            font.pointSize: 12
            font.bold: true
            Layout.fillWidth: true
            height: 30
        }
        Repeater {
            model : 8
            PropertyByte {
                Layout.fillWidth: true
                Layout.fillHeight: false
                label : "Custom Byte " + index
//                onValueChanged: HCUConnection.setCustomCommandByte(index,value)
//                propertyValue: HCUConnection.CustomCommandBytes[index]
                editable : true
            }
        }
    }
}
