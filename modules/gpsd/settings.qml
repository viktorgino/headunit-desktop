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
                id: fence1pt1
                width: parent.width
                label: "Fence 1 Point 1"
                onValueChanged: {
                    pluginSettings.fence1pt1 = value
                }
                value : pluginSettings.fence1pt1
            }

            SettingsPageItemTextfield {
                id: fence1pt2
                width: parent.width
                label: "Fence 1 Point 2"
                onValueChanged: {
                    pluginSettings.fence1pt2 = value
                }
                value : pluginSettings.fence1pt2
            }

            SettingsPageItemTextfield {
                id: fence1pt3
                width: parent.width
                label: "Fence 1 Point 3"
                onValueChanged: {
                    pluginSettings.fence1pt3 = value
                }
                value : pluginSettings.fence1pt3
            }

            SettingsPageItemTextfield {
                id: fence1pt4
                width: parent.width
                label: "Fence 1 Point 4"
                onValueChanged: {
                    pluginSettings.fence1pt4 = value
                }
                value : pluginSettings.fence1pt4
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
