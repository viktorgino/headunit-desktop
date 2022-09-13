import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.0

import HUDPlugins 1.0
import HUDTheme 1.0

ThemeRoot {
    id: __root

    Button {
        id: button
        x: 8
        y: 8
        text: qsTr("Sound Button")
        onClicked : {
            pluginContext.openOverlay();
            pluginContext.settingsState = 1;
        }
    }
}

/*##^##
Designer {
D{i:0;autoSize:true;formeditorColor:"#808080";height:480;width:640}
}
##^##*/
