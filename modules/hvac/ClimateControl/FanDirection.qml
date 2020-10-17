import QtQuick 2.6
import QtGraphicalEffects 1.0
import QtQuick.Layouts 1.0

Item{
    id: fan_direction
    Layout.fillHeight: true
    Layout.fillWidth: true

    Image {
        id: buttons_bg
        width: parent.width
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.bottom: parent.bottom
        anchors.top: parent.top
        fillMode: Image.PreserveAspectFit
        source: "qrc:/HVAC/icons/ac-buttons-bg.png"
        mipmap:true
    }

    Item{
        id:buttons
        width: buttons_bg.paintedWidth
        height: buttons_bg.paintedHeight
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter

        GridLayout {
            id: gridLayout
            rows: 3
            columns: 10
            columnSpacing: 0
            rowSpacing: 0
            anchors.fill: parent

            ImageButton {
                id: recirc_button
                visible: (HVACPlugin.settings.recirculate == "true")
                fillMode: Image.Stretch
                Layout.rowSpan: 3
                Layout.columnSpan: 3
                Layout.fillHeight: true
                Layout.fillWidth: true
                source: "qrc:/HVAC/icons/ac-buttons_01.png"
                checked: HVACPlugin.parameters.Recirculate;
                onClicked:{
                    HVACPlugin.toggleParameter("Recirculate")
                }
            }

            ImageButton {
                id: windshield_button
                visible: (HVACPlugin.settings.air_dir_windshield == "true")
                Layout.columnSpan: 1
                fillMode: Image.PreserveAspectFit
                Layout.rowSpan: 3
                Layout.fillHeight: true
                Layout.fillWidth: true
                source: "qrc:/HVAC/icons/ac-buttons_02.png"
                checked: HVACPlugin.parameters.FrontDefrost;
                onClicked:{
                    HVACPlugin.toggleParameter("FrontDefrost")
                }
            }

            ImageButton {
                id: front_top_button
                visible: (HVACPlugin.settings.air_dir_rear_top == "true")
                Layout.rowSpan: 1
                Layout.columnSpan: 2
                Layout.fillHeight: true
                Layout.fillWidth: true
                source: "qrc:/HVAC/icons/ac-buttons_03.png"
                checked: HVACPlugin.parameters.FrontTop;
                onClicked:{
                    HVACPlugin.toggleParameter("FrontTop")
                }
            }

            ImageButton {
                id: rear_top_button
                visible: (HVACPlugin.settings.air_dir_top == "true")
                Layout.rowSpan: 1
                Layout.columnSpan: 2
                Layout.fillHeight: true
                Layout.fillWidth: true
                source: "qrc:/HVAC/icons/ac-buttons_04.png"
                checked: HVACPlugin.parameters.RearTop;
                onClicked:{
                    HVACPlugin.toggleParameter("RearTop")
                }
            }

            ImageButton {
                id: rear_windshield_button
                visible: (HVACPlugin.settings.rear_def == "true")
                Layout.rowSpan: 3
                Layout.columnSpan: 2
                Layout.fillHeight: true
                Layout.fillWidth: true
                source: "qrc:/HVAC/icons/ac-buttons_05.png"
                checked: HVACPlugin.parameters.RearDefrost;
                onClicked:{
                    HVACPlugin.toggleParameter("RearDefrost")
                }
            }

            ImageButton {
                id: front_middle_button
                visible: (HVACPlugin.settings.air_dir_centre == "true")
                Layout.rowSpan: 1
                Layout.columnSpan: 2
                Layout.fillHeight: true
                Layout.fillWidth: true
                source: "qrc:/HVAC/icons/ac-buttons_06.png"
                checked: HVACPlugin.parameters.FrontMiddle;
                onClicked:{
                    HVACPlugin.toggleParameter("FrontMiddle")
                }
            }

            ImageButton {
                id: rear_middle_button
                visible: (HVACPlugin.settings.air_dir_rear_centre == "true")
                Layout.rowSpan: 1
                Layout.columnSpan: 2
                Layout.fillHeight: true
                Layout.fillWidth: true
                source: "qrc:/HVAC/icons/ac-buttons_07.png"
                checked: HVACPlugin.parameters.RearMiddle;
                onClicked:{
                    HVACPlugin.toggleParameter("RearMiddle")
                }
            }

            ImageButton {
                id: front_bottom_button
                visible: (HVACPlugin.settings.air_dir_bottom == "true")
                Layout.rowSpan: 1
                Layout.columnSpan: 2
                Layout.fillHeight: true
                Layout.fillWidth: true
                source: "qrc:/HVAC/icons/ac-buttons_08.png"
                checked: HVACPlugin.parameters.FrontBottom;
                onClicked:{
                    HVACPlugin.toggleParameter("FrontBottom")
                }
            }

            ImageButton {
                id: rear_bottom_button
                visible: (HVACPlugin.settings.air_dir_rear_bottom == "true")
                Layout.rowSpan: 1
                Layout.columnSpan: 2
                Layout.fillHeight: true
                Layout.fillWidth: true
                source: "qrc:/HVAC/icons/ac-buttons_09.png"
                checked: HVACPlugin.parameters.RearBottom;
                onClicked:{
                    HVACPlugin.toggleParameter("RearBottom")
                }
            }
        }
    }


}

/*##^## Designer {
    D{i:0;autoSize:true;height:300;width:800}
}
 ##^##*/
