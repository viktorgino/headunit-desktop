import QtQml 2.2
import QtQuick 2.6
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

Item {
    property QtObject bluezManager
    Text {
        id: text6
        text: {
            return bluezManager.devices[PhoneBluetooth.deviceIndex].mediaPlayer.track.artist + " - " + bluezManager.devices[PhoneBluetooth.deviceIndex].mediaPlayer.track.title
        }
        anchors.bottom: buttons.top
        anchors.bottomMargin: 0
        wrapMode: Text.WordWrap
        horizontalAlignment: Text.AlignHCenter
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0
        font.pixelSize: 22
    }

    RowLayout {
        id: buttons
        height: width/6
        anchors.bottom: sliderHorizontal1.top
        anchors.bottomMargin: 8
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0

        /*
     * Currently not working. The following error is returned when trying to set shuffle:
     *      BluezQt: PendingCall Error: "No such property 'Shuffle'"
     */
        //TODO:Look into why BluezQt::MediaPlayer.shuffle is not working
        /*ImageButton{
        id: shuffle_button
        Layout.fillHeight: true
        Layout.fillWidth: true
        checkable: true
        imageSource: "qrc:/qml/icons/shuffle.png"
        changeColorOnPress:false
        onClicked: {
            if(checked)
                bluezManager.devices[PhoneBluetooth.deviceIndex].mediaPlayer.shuffle = BluezQt.MediaPlayer.ShuffleAllTracks
            else
                bluezManager.devices[PhoneBluetooth.deviceIndex].mediaPlayer.shuffle = BluezQt.MediaPlayer.ShuffleOff
        }
    }*/

        ImageButton{
            id: prev_button
            Layout.fillHeight: true
            Layout.fillWidth: true
            imageSource: "qrc:/qml/icons/skip-backward.png"
            onClicked: bluezManager.devices[PhoneBluetooth.deviceIndex].mediaPlayer.previous()
        }

        ImageButton{
            Layout.fillHeight: true
            Layout.fillWidth: true
            imageSource: bluezManager.devices[PhoneBluetooth.deviceIndex].mediaPlayer.status === 0?"qrc:/qml/icons/pause.png":"qrc:/qml/icons/play.png"
            id:playButton
            onClicked: {
                if(bluezManager.devices[PhoneBluetooth.deviceIndex].mediaPlayer.status === 0)
                    bluezManager.devices[PhoneBluetooth.deviceIndex].mediaPlayer.pause()
                else
                    bluezManager.devices[PhoneBluetooth.deviceIndex].mediaPlayer.play()
            }
        }

        ImageButton{
            id: next_button
            Layout.fillHeight: true
            Layout.fillWidth: true
            imageSource: "qrc:/qml/icons/skip-forward.png"
            onClicked: bluezManager.devices[PhoneBluetooth.deviceIndex].mediaPlayer.next()
        }

        /*
     * Currently not working. The following error is returned when trying to set repeat:
     *      BluezQt: PendingCall Error: "No such property 'Repeat'"
     */
        //TODO:Look into why BluezQt::MediaPlayer.repeat is not working
        /*ImageButton {
        id: loop_button
        Layout.fillHeight: true
        Layout.fillWidth: true
        checked: true
        imageSource: "qrc:/qml/icons/refresh.png"
        changeColorOnPress:false
        text: {
            switch(bluezManager.devices[PhoneBluetooth.deviceIndex].mediaPlayer.repeat){
            case BluezQt.MediaPlayer.RepeatOff:
                checked = true;
                return "1";
            case BluezQt.MediaPlayer.RepeatSingleTrack:
                checked = true;
                return "All";
            default:
                checked = false;
                return "";
            }
        }
        onClicked: {
            switch(bluezManager.devices[PhoneBluetooth.deviceIndex].mediaPlayer.repeat){
            case BluezQt.MediaPlayer.RepeatOff:
                bluezManager.devices[PhoneBluetooth.deviceIndex].mediaPlayer.shuffle = BluezQt.MediaPlayer.RepeatSingleTrack
                break;
            case BluezQt.MediaPlayer.RepeatSingleTrack:
                bluezManager.devices[PhoneBluetooth.deviceIndex].mediaPlayer.shuffle = BluezQt.MediaPlayer.RepeatAllTracks
                break;
            default:
                bluezManager.devices[PhoneBluetooth.deviceIndex].mediaPlayer.shuffle = BluezQt.MediaPlayer.RepeatOff
                break;
            }
        }
    }*/
    }

    Slider {
        id: sliderHorizontal1
        anchors.bottom: trackInfo.top
        anchors.bottomMargin: 8
        value: bluezManager.devices[PhoneBluetooth.deviceIndex].mediaPlayer.position
        from:0
        to: bluezManager.devices[PhoneBluetooth.deviceIndex].mediaPlayer.track.duration
        stepSize: 1
        anchors.right: parent.right
        anchors.rightMargin: 8
        anchors.left: parent.left
        anchors.leftMargin: 8
        enabled: false
    }

    Timer {
        interval: 50; running: bluezManager.devices[PhoneBluetooth.deviceIndex].mediaPlayer.status === 0; repeat: true
        property int lastUpdated: 0
        onTriggered: {
            sliderHorizontal1.value = sliderHorizontal1.value + 50
        }
    }

    Connections{
        target: bluezManager.devices[PhoneBluetooth.deviceIndex].mediaPlayer
        onPositionChanged: {
            sliderHorizontal1.value = position
        }
        onTrackChanged:{
            sliderHorizontal1.value = bluezManager.devices[PhoneBluetooth.deviceIndex].mediaPlayer.position
        }
        onStatusChanged:{
            sliderHorizontal1.value = bluezManager.devices[PhoneBluetooth.deviceIndex].mediaPlayer.position
        }
    }

    Item {
        id: trackInfo
        height: 16
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0

        Text {
            text: {
                var position = sliderHorizontal1.value
                var seconds = parseInt((position / 1000) % 60);
                var minutes = parseInt((position / (60000 )) % 60);
                var hours = parseInt((position / (3600000)) % 24);
                return (hours > 0 ? hours + ":" : "") + minutes + ":" + ("0" + seconds).slice(-2);
            }
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 0
            font.pixelSize: 12
        }

        Text {
            text: {
                var duration = bluezManager.devices[PhoneBluetooth.deviceIndex].mediaPlayer.track.duration
                var seconds = parseInt((duration / 1000) % 60);
                var minutes = parseInt((duration / (60000)) % 60);
                var hours = parseInt((duration / (3600000)) % 24);
                return (hours > 0 ? hours + ":" : "") + minutes + ":" + ("0" + seconds).slice(-2);
            }
            anchors.right: parent.right
            anchors.rightMargin: 0
            font.pixelSize: 12
            anchors.verticalCenter: parent.verticalCenter
        }
    }
}
