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
