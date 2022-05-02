import QtQuick 2.9
import QtQuick.Window 2.3
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import Qt.labs.settings 1.0


GridLayout {
    columns: 2
    Repeater{
        model: ["IndicatorLeft", "IndicatorRight", "Braking", "Reversing", "HandBrake", "SeatBelt",
            "PassengerSeatOccupied", "RearLeftOccupied", "RearMiddleOccupied", "RearRightOccupied",
            "PassengerSeatBelt", "RearLeftSeatBelt", "RearMiddleSeatBelt", "RearRightSeatBelt", "NightLight"]
        PropertyBool {
            Layout.fillWidth: true
            Layout.fillHeight: false
            label : modelData
//            onSwitchClicked: HCUConnection.setBodyControlParameter(modelData, value)
//            propertyEnabled: HCUConnection.BodyControlCommandFrame[modelData]
        }
    }
    Repeater{
        model: ["DashBrightness", "speed", "engineRpm", "frontLeftWheelSpeed", "frontRightWheelSpeed",
            "rearLeftWheelSpeed", "rearRightWheelSpeed"]
        PropertyByte {
            Layout.fillWidth: true
            Layout.fillHeight: false
            label : modelData
            //                                maxValue :  65535
//            onValueChanged: HCUConnection.setDriveTrainParameter(modelData,value)
//            propertyValue: HCUConnection.DriveTrainControlCommandFrame[modelData]
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}D{i:2}D{i:1}D{i:4}D{i:3}
}
##^##*/
