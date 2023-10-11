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
            
            SettingsPageItemTextfield {
                id: host
                width: parent.width
                label: "Host"
                onValueChanged: {
                    pluginSettings.host = value
                }
                value : pluginSettings.host
            }

            SettingsPageItemTextfield {
                id: port
                width: parent.width
                label: "Port"
                onValueChanged: {
                    pluginSettings.port = value
                }
                value : pluginSettings.port
            }

            SettingsPageItemTextfield {
                id: current_mode
                width: parent.width
                label: "Current Fix Mode"
                value : pluginContext.mode
            }

            SettingsPageItemTextfield {
                id: current_latitude
                width: parent.width
                label: "Current Latitude"
                value : pluginContext.latitude
            }

            SettingsPageItemTextfield {
                id: current_longitude
                width: parent.width
                label: "Current Longitude"
                value : pluginContext.longitude
            }

            SettingsPageItemTextfield {
                id: current_inFence
                width: parent.width
                label: "Currently inside fence?"
                value : pluginContext.inFence
            }

            SettingsPageItemTextfield {
                id: fence
                width: parent.width
                label: "Fence"
                onValueChanged: {
                    pluginSettings.fence = value
                }
                value : pluginSettings.fence
            }

            SettingsPageItemTextfield {
                id: fence0pt0
                width: parent.width
                label: "Fence 0 Point 0"
                onValueChanged: {
                    pluginSettings.fence0pt0 = value
                }
                value : pluginSettings.fence0pt0
            }

            SettingsPageItemTextfield {
                id: fence0pt1
                width: parent.width
                label: "Fence 0 Point 1"
                onValueChanged: {
                    pluginSettings.fence0pt1 = value
                }
                value : pluginSettings.fence0pt1
            }

            SettingsPageItemTextfield {
                id: fence0pt2
                width: parent.width
                label: "Fence 0 Point 2"
                onValueChanged: {
                    pluginSettings.fence0pt2 = value
                }
                value : pluginSettings.fence0pt2
            }

            SettingsPageItemTextfield {
                id: fence0pt3
                width: parent.width
                label: "Fence 0 Point 3"
                onValueChanged: {
                    pluginSettings.fence0pt3 = value
                }
                value : pluginSettings.fence0pt3
            }

            SettingsPageItemTextfield {
                id: fence0pt4
                width: parent.width
                label: "Fence 0 Point 4"
                onValueChanged: {
                    pluginSettings.fence0pt4 = value
                }
                value : pluginSettings.fence0pt4
            }

            SettingsPageItemTextfield {
                id: fence0pt5
                width: parent.width
                label: "Fence 0 Point 5"
                onValueChanged: {
                    pluginSettings.fence0pt5 = value
                }
                value : pluginSettings.fence0pt5
            }

            SettingsPageItemTextfield {
                id: fence0pt6
                width: parent.width
                label: "Fence 0 Point 6"
                onValueChanged: {
                    pluginSettings.fence0pt6 = value
                }
                value : pluginSettings.fence0pt6
            }

            SettingsPageItemTextfield {
                id: fence0pt7
                width: parent.width
                label: "Fence 0 Point 7"
                onValueChanged: {
                    pluginSettings.fence0pt7 = value
                }
                value : pluginSettings.fence0pt7
            }

            SettingsPageItemTextfield {
                id: fence0pt8
                width: parent.width
                label: "Fence 0 Point 8"
                onValueChanged: {
                    pluginSettings.fence0pt8 = value
                }
                value : pluginSettings.fence0pt8
            }

            SettingsPageItemTextfield {
                id: fence0pt9
                width: parent.width
                label: "Fence 0 Point 9"
                onValueChanged: {
                    pluginSettings.fence0pt9 = value
                }
                value : pluginSettings.fence0pt9
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
