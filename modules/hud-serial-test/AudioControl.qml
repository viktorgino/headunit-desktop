import QtQuick 2.9
import QtQuick.Window 2.3
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import Qt.labs.settings 1.0


GridLayout {
    columns: 2
    Repeater{
        model: [
            "SetInput",
            "SetInputGain",
            "SetVolume",
            "SetFrontLeftOutputLevel",
            "SetFrontCenterOutputLevel",
            "SetFrontRightOutputLevel",
            "SetCenterLeftOutputLevel",
            "SetCenterRightOutputLevel",
            "SetRearLeftOutputLevel",
            "SetRearRightOutputLevel",
            "SetSubwooferOutputLevel",
            "SetBassLevel",
            "SetMidLevel",
            "SetTrebleLevel"
        ]
        PropertyByte {
            Layout.fillWidth: true
            Layout.fillHeight: false
            label : modelData
            onValueChanged: pluginContext.setAudioParameter(modelData,value)
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}D{i:2}D{i:1}D{i:4}D{i:3}
}
##^##*/
