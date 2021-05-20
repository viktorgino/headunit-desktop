import QtQml 2.2
import QtQuick 2.6
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

import HUDTheme 1.0

import org.kde.bluezqt 1.0 as BluezQt

Item {
    id:__root
    property QtObject mediaPlayerObj

    ThemeHeaderText {
        id: text6
        text: {
            return __root.mediaPlayerObj.track.artist + " - " + __root.mediaPlayerObj.track.title
        }
        anchors.bottom: buttons.top
        anchors.bottomMargin: 0
        wrapMode: Text.WordWrap
        horizontalAlignment: Text.AlignHCenter
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0
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
//        ImageButton{
//        id: shuffle_button
//        Layout.fillHeight: true
//        Layout.fillWidth: true
//        checkable: true
//        imageSource: "qrc:/qml/icons/shuffle.png"
//        checked : __root.mediaPlayerObj.shuffle
//        onClicked: {
//            if(checked)
//                __root.mediaPlayerObj.shuffle = 1//BluezQt.MediaPlayer.ShuffleAllTracks
//            else
//                __root.mediaPlayerObj.shuffle = 0//BluezQt.MediaPlayer.ShuffleOff
//        }
//    }

        ImageButton{
            id: prev_button
            Layout.fillHeight: true
            Layout.fillWidth: true
            imageSource: "qrc:/qml/icons/skip-backward.png"
            onClicked: __root.mediaPlayerObj.previous()
        }

        ImageButton{
            Layout.fillHeight: true
            Layout.fillWidth: true
            imageSource: __root.mediaPlayerObj.status === 0?"qrc:/qml/icons/pause.png":"qrc:/qml/icons/play.png"
            id:playButton
            onClicked: {
                if(__root.mediaPlayerObj.status === 0)
                    __root.mediaPlayerObj.pause()
                else
                    __root.mediaPlayerObj.play()
            }
        }

        ImageButton{
            id: next_button
            Layout.fillHeight: true
            Layout.fillWidth: true
            imageSource: "qrc:/qml/icons/skip-forward.png"
            onClicked: __root.mediaPlayerObj.next()
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
        text: {
            switch(__root.mediaPlayerObj.repeat){
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
            switch(__root.mediaPlayerObj.repeat){
            case BluezQt.MediaPlayer.RepeatOff:
                __root.mediaPlayerObj.shuffle = BluezQt.MediaPlayer.RepeatSingleTrack
                break;
            case BluezQt.MediaPlayer.RepeatSingleTrack:
                __root.mediaPlayerObj.shuffle = BluezQt.MediaPlayer.RepeatAllTracks
                break;
            default:
                __root.mediaPlayerObj.shuffle = BluezQt.MediaPlayer.RepeatOff
                break;
            }
        }
    }*/
    }

    Slider {
        id: sliderHorizontal1
        anchors.bottom: trackInfo.top
        anchors.bottomMargin: 8
        value: __root.mediaPlayerObj.position
        from:0
        to: __root.mediaPlayerObj.track.duration
        stepSize: 1
        anchors.right: parent.right
        anchors.rightMargin: 8
        anchors.left: parent.left
        anchors.leftMargin: 8
        enabled: false
    }

    Timer {
        interval: 50; running: __root.mediaPlayerObj.status === 0; repeat: true
        property int lastUpdated: 0
        onTriggered: {
            sliderHorizontal1.value = sliderHorizontal1.value + 50
        }
    }

    Connections{
        target: __root.mediaPlayerObj
        onPositionChanged: {
            sliderHorizontal1.value = position
        }
        onTrackChanged:{
            sliderHorizontal1.value = __root.mediaPlayerObj.position
        }
        onStatusChanged:{
            sliderHorizontal1.value = __root.mediaPlayerObj.position
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

        ThemeText {
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
        }

        ThemeText {
            text: {
                var duration = __root.mediaPlayerObj.track.duration
                var seconds = parseInt((duration / 1000) % 60);
                var minutes = parseInt((duration / (60000)) % 60);
                var hours = parseInt((duration / (3600000)) % 24);
                return (hours > 0 ? hours + ":" : "") + minutes + ":" + ("0" + seconds).slice(-2);
            }
            anchors.right: parent.right
            anchors.rightMargin: 0
            anchors.verticalCenter: parent.verticalCenter
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
