import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.0

import HUDPlugins 1.0
import HUDTheme 1.0

ThemeRoot {
    id: __root
    Rectangle {
        id:overlay
        color: "#000000"
        anchors.fill: parent
    }
    SliderVolume {
        id:volumeSlider
        anchors.fill: parent
        value : pluginSettings["volume"]
        maxValue : 50
    } 
    Binding {
        target: pluginSettings
        property: "volume"
        value: volumeSlider.value
    }

    Connections{
        target: pluginSettings
        onValueChanged:{
            if(key == "volume" && volumeSlider.value != value){
                volumeSlider.value = value
            }
        }
    }
    Button {
        id: button2
        x: 200
        y: 8
        text: qsTr("Close Button")
        onClicked : pluginContext.closeOverlay()
    }
}

/*##^##
Designer {
D{i:0;autoSize:true;formeditorColor:"#808080";height:480;width:640}
}
##^##*/
