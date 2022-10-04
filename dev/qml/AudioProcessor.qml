import QtQuick 2.15
import QtQuick.Controls 2.15

import "../../modules/audio-processor"
Item{
    id: item2
    PluginObject{
        id:plugin
        source: "../../modules/audio-processor/SoundSettings.qml"
        contextProperty: QtObject{

        }
        settings: QtObject{
            property int volume : 0
            property int sub : 0
            property int bass : 0
            property int middle : 0
            property int treble : 0
            property int balance : 0
            signal valueChanged(string key, int value)
        }
        property int settingsState : 1
        function openOverlay(){

        }

        function setAudioParameter(parameter, value){
            console.log("Set %s %d", parameter, value)
            settings.valueChanged(parameter, value)
            switch(parameter) {
            case "volume":
                settings.volume = value;
                break;
            case "sub":
                settings.sub = value;
                break;
            case "bass":
                settings.bass = value;
                break;
            case "middle":
                settings.middle = value;
                break;
            case "treble":
                settings.treble = value;
                break;
            case "balance":
                settings.balance = value;
                break;
            default:
                console.log("Invalid parameter")
            }
        }
    }

    SoundSettings {
        id: item1
        height: parent.height / 2
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        pluginContext: plugin
        pluginSettings: plugin.settings
    }

    Button {
        id: button
        text: qsTr("Change parameter")
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 8
        anchors.leftMargin: 8
        onClicked: {
            if (plugin.settingsState < 5)
                plugin.settingsState++
            else
                plugin.settingsState = 1
        }
    }
    function setParameter(increase){
        var parameter = ""
        switch(plugin.settingsState) {
        case 1:
            parameter = "balance"
            break;
        case 2:
            parameter = "sub"
            break;
        case 3:
            parameter = "bass"
            break;
        case 4:
            parameter = "middle"
            break;
        case 5:
            parameter = "treble"
            break;
        }
        var val = 1
        if (!increase){
            val = -1
        }

        plugin.settings[parameter] += val
    }

    Button {
        id: button1
        text: qsTr("-")
        anchors.left: button.right
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 8
        anchors.leftMargin: 8
        onClicked: {
            setParameter(false)
        }
    }

    Button {
        id: button2
        text: qsTr("+")
        anchors.left: button1.right
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 8
        anchors.leftMargin: 8
        onClicked: {
            setParameter(true)
        }
    }

}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}D{i:5}D{i:6}D{i:7}
}
##^##*/
