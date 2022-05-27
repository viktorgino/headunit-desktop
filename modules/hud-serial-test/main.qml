import QtQuick 2.9
import QtQuick.Window 2.3
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import Qt.labs.settings 1.0
import HUDTheme 1.0

ThemeRoot {
    id: __root

    TabBar {
        id: tabBar
        width: 240
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: 0
        TabButton {
            text: qsTr("Climate Control")
        }
        TabButton {
            text: qsTr("Keys")
        }
        TabButton {
            text: qsTr("Custom Command")
        }
        TabButton {
            text: qsTr("Body Control Command")
        }
        TabButton {
            text: qsTr("Audio Control")
        }
    }

    StackLayout {
        id: stackLayout
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: tabBar.bottom
        anchors.bottom: parent.bottom
        currentIndex: tabBar.currentIndex
        ClimateControl{
        }
        CommandKeys{
        }
        CustomCommand{
        }
        BodyControl{
        }
        AudioControl{
        }
    }


}


/*##^##
Designer {
    D{i:0;formeditorZoom:0.75}D{i:27}
}
##^##*/
