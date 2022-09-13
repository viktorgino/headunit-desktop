import QtQuick 2.9
import QtQuick.Window 2.3
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import Qt.labs.settings 1.0

GridLayout {
    rowSpacing: 1
    columnSpacing: 1
    columns: 6
    Repeater{
        model: ["0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
            "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K",
            "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V",
            "W", "X", "Y", "Z",
            "Enter", "Exit", "Back", "Menu", "Power",
            "Left", "Right", "Up", "Down",
            "VolumeUp", "VolumeDown", "TuneUp", "TuneDown",
            "Previous", "Next",
            "Sound", "AMFM", "CD", "Eject", "Auto", "Scan"]
        Button{
            Layout.preferredWidth: 100
            Layout.preferredHeight: 35
            text:modelData
            font.kerning: true
            font.pointSize: 8
            onClicked: pluginContext.buttonClicked(text)
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}D{i:2}D{i:1}
}
##^##*/
