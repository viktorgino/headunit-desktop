import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.0
import QtMultimedia 5.7
import Qt.labs.settings 1.0
import QtGraphicalEffects 1.0
import QtQml 2.3

import HUDTheme 1.0

ThemeRoot{
    id: __root
    property alias menuButtonRectX: menuButtonRect.x
    function getReadableTime(milliseconds){
        var minutes = Math.floor(milliseconds / 60000);
        var seconds = ((milliseconds % 60000) / 1000).toFixed(0);
        return (seconds == 60 ? (minutes+1) + ":00" : minutes + ":" + (seconds < 10 ? "0" : "") + seconds);
    }
    clip: true


    Item {
        id:backgroundImageWrapper
        x: 0
        y: 48
        anchors.fill: parent
        Image {
            id:backgroundImage
            anchors.fill: parent
            source: "image://MediaPlayerPlugin/background/"+playlist.currentItemSource
            fillMode: Image.PreserveAspectCrop
            visible: false
        }
        FastBlur {
            anchors.fill: parent
            source: backgroundImage
            radius: 32
            smooth: true
            visible: true
        }
    }

    Item {
        id: main
        anchors.top: menuButtonWrapper.bottom
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.left: parent.left

        Item {
            id: wrapper
            anchors.fill: parent
            anchors.leftMargin: parent.width * 0.05
            anchors.rightMargin: parent.width * 0.05
            anchors.bottomMargin: parent.height * 0.05
            anchors.topMargin: parent.height * 0.05

            Image {
                id: thumbnail_image
                width: parent.width * 0.3
                height: width
                anchors.verticalCenter: parent.verticalCenter
                fillMode: Image.PreserveAspectCrop
                anchors.left: parent.left
                anchors.leftMargin: 0
                mipmap:true
                cache : true
                source: "image://MediaPlayerPlugin/"+playlist.currentItemSource
            }

            Item {
                id: track_info
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.bottom: buttons.top
                anchors.topMargin: 0
                anchors.rightMargin: 8
                anchors.left: thumbnail_image.right
                anchors.leftMargin: 0

                ThemeHeaderText {
                    id: media_title
                    level: 5
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                    anchors.right: parent.right
                    anchors.bottom: media_author.top
                    anchors.rightMargin: 0
                    anchors.left: parent.left
                    anchors.leftMargin: 0
                    renderType: Text.NativeRendering;
                    font.hintingPreference: Font.PreferVerticalHinting
                }

                ThemeHeaderText {
                    id: media_author
                    level: 3
                    text:mediaplayer.metaData.contributingArtist?mediaplayer.metaData.contributingArtist:""
                    anchors.verticalCenter: parent.verticalCenter
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    anchors.right: parent.right
                    anchors.rightMargin: 0
                    anchors.left: parent.left
                    anchors.leftMargin: 0
                    renderType: Text.NativeRendering;
                    font.hintingPreference: Font.PreferVerticalHinting
                }

                ThemeHeaderText {
                    id: media_album_title
                    level: 1
                    text: mediaplayer.metaData.albumTitle?mediaplayer.metaData.albumTitle:""
                    verticalAlignment: Text.AlignTop
                    horizontalAlignment: Text.AlignHCenter
                    wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                    anchors.right: parent.right
                    anchors.rightMargin: 0
                    anchors.left: parent.left
                    anchors.leftMargin: 0
                    anchors.top: media_author.bottom
                    anchors.topMargin: 0
                    renderType: Text.NativeRendering;
                    font.hintingPreference: Font.PreferVerticalHinting
                }


            }

            RowLayout {
                id: buttons
                width: height * 5
                height: parent.height*0.15
                anchors.horizontalCenter: track_info.horizontalCenter
                anchors.bottom: slider_wrapper.top
                anchors.bottomMargin: 0

                ImageButton{
                    id: shuffle_button
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    checkable: true
                    imageSource: "qrc:/qml/icons/shuffle.png"
                    onClicked: {
                        if(checked){
                            playlist.playbackMode = Playlist.Random
                        } else {
                            playlist.playbackMode = Playlist.Sequential
                        }
                    }
                }

                ImageButton{
                    id: prev_button
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    imageSource: "qrc:/qml/icons/skip-backward.png"
                    onClicked: mediaplayer.playlist.previous()
                }

                ImageButton{
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    imageSource: "qrc:/qml/icons/play.png"
                    id:playButton
                    onClicked: {
                        switch (mediaplayer.playbackState){
                        case MediaPlayer.PlayingState:
                            mediaplayer.pause()
                            break;
                        case MediaPlayer.PausedState:
                        case MediaPlayer.StoppedState:
                            mediaplayer.play()
                            break;
                        }
                    }
                }

                ImageButton{
                    id: next_button
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    imageSource: "qrc:/qml/icons/skip-forward.png"
                    onClicked: mediaplayer.playlist.next()
                }


                ImageButton {
                    id: loop_button
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    checked: (playlist.playbackMode == Playlist.CurrentItemInLoop || playlist.playbackMode == Playlist.Loop)
                    imageSource: "qrc:/qml/icons/refresh.png"
                    text : {
                        switch(playlist.playbackMode){
                        case Playlist.CurrentItemInLoop:
                            return "1";
                        case Playlist.Loop:
                            return "All";
                        default:
                            return "";
                        }
                    }

                    onClicked: {
                        if (playlist.playbackMode == Playlist.CurrentItemInLoop){
                            playlist.playbackMode = Playlist.Loop;
                        } else if (playlist.playbackMode == Playlist.Loop) {
                            playlist.playbackMode = Playlist.Sequential;
                        } else {
                            playlist.playbackMode = Playlist.CurrentItemInLoop;
                        }
                    }
                }
            }

            Item {
                id: slider_wrapper
                height: parent.height * 0.2
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 8
                anchors.left: thumbnail_image.right
                anchors.leftMargin: 8
                anchors.right: parent.right
                anchors.rightMargin: 8

                ThemeSlider {
                    id: sliderHorizontal1
                    anchors.top: parent.top
                    anchors.topMargin: 0
                    value: mediaplayer.position
                    to: mediaplayer.duration
                    stepSize: 1
                    anchors.right: parent.right
                    anchors.rightMargin: 0
                    anchors.left: parent.left
                    anchors.leftMargin: 0
                    onValueChanged: {
                        if(value != mediaplayer.position){
                            mediaplayer.seek(value)
                        }
                    }
                }

                ThemeText {
                    id: text1
                    text: getReadableTime(mediaplayer.position)
                    anchors.top: sliderHorizontal1.bottom
                    anchors.topMargin: 0
                    anchors.left: parent.left
                    anchors.leftMargin: 0
                }

                ThemeText {
                    id: text2
                    text: getReadableTime(mediaplayer.duration)
                    anchors.right: parent.right
                    anchors.rightMargin: 0
                    anchors.top: sliderHorizontal1.bottom
                    anchors.topMargin: 0
                }
            }
        }

    }

    Rectangle {
        id: overlay
        color: "#000000"
        opacity: 0
        anchors.fill : parent

        MouseArea {
            id: mouseArea5
            enabled: false
            anchors.fill: parent
            onClicked: __root.state=""
        }
    }

    Playlist{
        id: playlist
        playbackMode : Playlist.Sequential
        onCurrentIndexChanged: {
            mediaplayer_settings.nowPlayingCurrentIndex = currentIndex
        }
    }


    MediaPlayer {
        id: mediaplayer
        playlist: playlist
        autoLoad: true
        audioRole: MediaPlayer.MusicRole
        volume: pluginContext.Volume
        onError: {
            console.log("Media Player error : " , error, errorString)
        }

        onStatusChanged: {
            if(status === 3 || status === 6){
                if(mediaplayer.metaData.title && mediaplayer.metaData.title !== ""){
                    media_title.text = mediaplayer.metaData.title
                } else {
                    var url = String(playlist.currentItemSource);
                    media_title.text = url.substring(url.lastIndexOf('/')+1);
                }
            }
        }

        onPaused: {
            playButton.imageSource = "qrc:/qml/icons/play.png";
        }
        onStopped: {
            playButton.imageSource = "qrc:/qml/icons/play.png";
        }
        onPlaying: {
            playButton.imageSource = "qrc:/qml/icons/pause.png";
            pluginContext.playbackStarted()
        }
    }

    Component {
        id: mediaList

        MediaList {
            model: pluginContext.MediaListModel

            onItemClicked: {
                pluginContext.PlaylistModel.setItems(pluginContext.MediaListModel.getItems());

                playlist.clear();
                playlist.addItems(pluginContext.PlaylistModel.sources);
                playlist.currentIndex = index;

                mediaplayer.play();

                __root.state = "";
                mediaDrawer.state = ""
                stackView.clear()
            }
        }
    }


    Component {
        id: mediaContainerList
        MediaContainerList {
            model:pluginContext.ContainerModel

            onItemClicked: {
                var item = pluginContext.ContainerModel.getItem(index)

                var filter = item.title
                if(item_type === "folders"){
                    filter = item.folder_id
                } else if(item_type === "playlists"){
                    filter = item.path
                }

                pluginContext.MediaListModel.setFilter(item_type, filter)
                stackView.push(mediaList,{
                                   "thumbnail" : "file://" + item.thumbnail,
                                   "title" : item.title,
                                   "subtitle" : item.subtitle }
                               )
                __root.state = "libraryView"
            }
        }
    }

    StackView {
        id: stackView
        x: parent.width
        width: parent.width
        visible: false
        anchors.top: menuButtonWrapper.bottom
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
    }

    MediaDrawer {
        id: mediaDrawer
        x: -width
        width: parent.width * 0.3
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.top: menuButtonWrapper.bottom
        currentPlaying: playlist.currentIndex
        onPlayListItemClicked: {
            playlist.currentIndex = index;
            __root.state="";
            mediaplayer.play();
        }

        onMediaPlayeritemClicked: {

            stackView.clear()
            switch(item_type){
            case "folders":
            case "playlists":
            case "artists":
            case "albums":
            case "genres":
                pluginContext.ContainerModel.setFilter(item_type);
                stackView.push(mediaContainerList,{"icon" : icon, "name" : name, "item_type" : item_type})
                break;
            case "songs":
                pluginContext.MediaListModel.setFilter("", "")
                stackView.push(mediaList,{"icon" : icon, "name" : name, "item_type" : item_type})
                break;
            default:
                break;
            }
            __root.state = "libraryView"
        }
    }


    Rectangle {
        id: menuButtonRect
        x: - width
        width: parent.width * 0.3
        color: "#212121"
        anchors.top: parent.top
        anchors.bottom: menuButtonWrapper.bottom
    }


    Item {
        id: menuButtonWrapper
        width: height
        height: parent.height*0.10
        anchors.top: parent.top
        anchors.topMargin: 0

        Rectangle {
            id: rectangle1
            width:parent.width * 0.8
            height: parent.height * 0.8
            color: "#263238"
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            MenuButton {
                id: menuButton
                anchors.rightMargin: parent.height*0.2
                anchors.right: parent.right
                anchors.left: parent.left
                anchors.leftMargin: parent.height*0.2
                anchors.top: parent.top
                anchors.topMargin: parent.height*0.2
                anchors.bottom: parent.bottom
                anchors.bottomMargin: parent.height*0.2
            }

            MouseArea {
                anchors.fill: parent
                onClicked:{
                    if(__root.state == ""){
                        __root.state = "drawer";
                    } else if(__root.state == "libraryView") {
                        if(stackView.depth < 2){
                            stackView.clear()
                            __root.state = "drawer";
                        }
                        stackView.pop()
                    }  else if(__root.state == "drawer") {
                        stackView.clear()
                        __root.state = "";
                        mediaDrawer.state = "";
                    }
                }
            }
        }
    }

    Item {
        id: closeButtonWrapper
        width: height
        height: parent.height*0.1
        visible: false
        anchors.right: parent.right

        Rectangle {
            id: rectangle2
            width: parent.width * 0.8
            height: parent.height * 0.8
            color: "#263238"
            anchors.verticalCenter: parent.verticalCenter

            Text {
                id: text3
                color: "#ffffff"
                text: qsTr("X")
                anchors.fill: parent
                font.pixelSize: 100
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                fontSizeMode: Text.Fit
                font.bold: true
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    stackView.clear()
                    __root.state = "";
                    mediaDrawer.state = "";
                }
            }

            anchors.horizontalCenter: parent.horizontalCenter
        }
    }

    Connections{
        target: pluginContext
        onStart : {
            mediaplayer.play()
        }
        onStop : {
            mediaplayer.stop()
        }
        onPrevTrack : {
            playlist.previous()
        }
        onNextTrack : {
            playlist.next()
        }
    }

    states: [
        State {
            name: "drawer"
            PropertyChanges {
                target: main
                anchors.topMargin: 0
            }

            PropertyChanges {
                target: overlay
                opacity: 0.5
            }

            PropertyChanges {
                target: mouseArea5
                enabled: true
            }

            PropertyChanges {
                target: menuButton
                isActive: true
            }

            PropertyChanges {
                target: mediaDrawer
                x: 0
                anchors.rightMargin: 640
            }

            PropertyChanges {
                target: closeButtonWrapper
                visible: false
            }

            PropertyChanges {
                target: menuButtonRect
                x: 0
            }
        },
        State {
            name: "libraryView"
            PropertyChanges {
                target: main
                anchors.topMargin: 0
            }

            PropertyChanges {
                target: overlay
                opacity: 0.9
            }

            PropertyChanges {
                target: __root
                clip: true
            }

            PropertyChanges {
                target: stackView
                x: 0
                visible: true
                anchors.leftMargin: 0
            }

            PropertyChanges {
                target: menuButton
                isActive : true
            }

            PropertyChanges {
                target: closeButtonWrapper
                visible: true
            }

            PropertyChanges {
                target: menuButtonRect
                x: 0
                width: parent.width
            }
        }
    ]
    transitions:[
        Transition {
            NumberAnimation { properties: "anchors.leftMargin,anchors.rightMargin,opacity,width,x"; duration: 250}
        }
    ]


    Settings {
        id: mediaplayer_settings
        property int nowPlayingCurrentIndex
        property alias thumbnailImage: thumbnail_image.source
    }




    Component.onCompleted : {
        playlist.addItems(pluginContext.PlaylistModel.sources);
        playlist.currentIndex = mediaplayer_settings.nowPlayingCurrentIndex;
    }

}

/*##^##
Designer {
    D{i:0;autoSize:true;formeditorColor:"#808080";height:480;width:640}D{i:29}D{i:31}
}
##^##*/
