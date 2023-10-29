import QtQuick 2.9
import QtQuick.Controls 2.2
import HUDTheme 1.0
import HUDSettingsPage 1.0
import QtQuick.Layouts 1.3

ThemeRoot {
    id:__root

    Flickable {
        anchors.fill: parent
        contentHeight: column.height
        flickableDirection: Flickable.VerticalFlick
        ScrollBar.vertical: ThemeScrollBar { }
        Column {
            id: column
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: 0

            SettingsPageItemCombobox {
                id: sensor_model
                width: parent.width
                label: "Sensor Model"
                values: pluginContext.sensorModels
                onValueChanged: {
                    pluginSettings.sensor_model = value
                }
                value : pluginSettings.sensor_model
            }

            SettingsPageItemCombobox {
                id: i2c_port
                width: parent.width
                label: "I2C Port"
                values: pluginContext.ports
                onValueChanged: {
                    pluginSettings.i2c_port = value
                }
                value : pluginSettings.i2c_port
            }

            SettingsPageItemTextfield {
                id: refresh_interval
                width: parent.width
                label: "Refresh Interval (0 to disable)"
				suffix: "s"
                onValueChanged: {
                    pluginSettings.refresh_interval = value
                }
                value : pluginSettings.refresh_interval
            }

            SettingsPageItemTextfield {
                id: day_threshold
                width: parent.width
                label: "Day Threshold"
                onValueChanged: {
                    pluginSettings.day_threshold = value
                }
                value : pluginSettings.day_threshold
            }

            SettingsPageItemTextfield {
                id: night_threshold
                width: parent.width
                label: "Night Threshold"
                onValueChanged: {
                    pluginSettings.night_threshold = value
                }
                value : pluginSettings.night_threshold
            }

            SettingsPageItemTextfield {
                id: current_light
                width: parent.width
                label: "Current Light Level"
                value : pluginContext.currentLight
            }

            SettingsPageItemHeader{
                width: parent.width
            }
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
