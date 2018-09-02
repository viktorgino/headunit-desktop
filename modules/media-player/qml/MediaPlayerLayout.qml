import QtQuick 2.6
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.0
import QtMultimedia 5.7
import Qt.labs.settings 1.0

Item {
    id: __media_player_layout
    function getReadableTime(milliseconds){
        var minutes = Math.floor(milliseconds / 60000);
        var seconds = ((milliseconds % 60000) / 1000).toFixed(0);
        return (seconds == 60 ? (minutes+1) + ":00" : minutes + ":" + (seconds < 10 ? "0" : "") + seconds);
    }
    property bool isBase: false
    clip: true
    function changeState(caller){
        if(__media_player_layout.state == "" && caller == "button"){
            __media_player_layout.state="drawer";
            top_menu.menuButtonActive = true;
            isBase = false;
        } else if(caller == "button"){
            __media_player_layout.state="";
            top_menu.menuButtonActive = false;
            isBase = false;
        } else if(caller == "toList"){
            __media_player_layout.state="media list";
        } else if(caller == "toContainer"){
            __media_player_layout.state="container list";
        } else if(caller == "toNowPlaying"){
            top_menu.menuButtonActive = true;
            __media_player_layout.state="now playing";
        }
    }

    Item {
        id: main
        anchors.top: top_menu.bottom
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.topMargin: 0


        Item {
            id: wrapper
            anchors.rightMargin: 15
            anchors.leftMargin: 15
            anchors.bottomMargin: 15
            anchors.topMargin: 15
            anchors.fill: parent

            Item {
                id: item6
                height: parent.height * 0.2
                anchors.top: parent.top
                anchors.topMargin: 0
                anchors.right: parent.right
                anchors.rightMargin: 0
                anchors.left: parent.left
                anchors.leftMargin: 0
                ImageButton{
                    width: height
                    anchors.topMargin: parent.height * 0.2
                    anchors.bottomMargin: parent.height * 0.2
                    anchors.leftMargin: parent.height * 0.2
                    anchors.left: parent.left
                    anchors.bottom: parent.bottom
                    anchors.top: parent.top
                    imageSource: "qrc:/qml/icons/radio-waves.png"
                    onClicked: changeState("toNowPlaying")
                }
            }

            Image {
                id: thumbnail_image
                width: parent.width * 0.3
                height: width
                anchors.topMargin: 0
                fillMode: Image.PreserveAspectCrop
                anchors.left: parent.left
                anchors.leftMargin: 0
                anchors.top: item6.bottom
                mipmap:true
            }

            Item {
                id: track_info
                anchors.top: item6.bottom
                anchors.topMargin: 0
                anchors.bottom: buttons.top
                anchors.bottomMargin: 0
                anchors.right: parent.right
                anchors.rightMargin: 8
                anchors.left: thumbnail_image.right
                anchors.leftMargin: 0

                Text {
                    id: media_title
                    color: "#ffffff"
                    text: mediaplayer.metaData.title?mediaplayer.metaData.title:""
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                    anchors.right: parent.right
                    anchors.rightMargin: 0
                    anchors.left: parent.left
                    anchors.leftMargin: 0
                    anchors.top: parent.top
                    anchors.topMargin: 0
                    font.pixelSize: 24
                }

                Text {
                    id: media_author
                    color: "#ffffff"
                    text:mediaplayer.getArtist(mediaplayer.metaData)
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    anchors.right: parent.right
                    anchors.rightMargin: 0
                    anchors.left: parent.left
                    anchors.leftMargin: 0
                    anchors.top: media_title.bottom
                    anchors.topMargin: 0
                    font.pixelSize: 16
                }

                Text {
                    id: media_album_title
                    color: "#ffffff"
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
                    font.pixelSize: 16
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
                    changeColorOnPress:false
                    onClicked: {
                        if(checked){
                            nowPlaying.playbackMode = Playlist.Random
                        } else {
                            nowPlaying.playbackMode = Playlist.Sequential
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
                    checked: (nowPlaying.playbackMode == Playlist.CurrentItemInLoop || nowPlaying.playbackMode == Playlist.Loop)
                    imageSource: "qrc:/qml/icons/refresh.png"
                    changeColorOnPress:false
                    text: {
                        switch(nowPlaying.playbackMode){
                        case Playlist.CurrentItemInLoop:
                            return "1";
                        case Playlist.Loop:
                            return "All";
                        default:
                            return "";
                        }
                    }
                    onClicked: {
                        shuffle_button.checked = false
                        if(nowPlaying.playbackMode == Playlist.Sequential || nowPlaying.playbackMode == Playlist.Random){
                            nowPlaying.playbackMode = Playlist.CurrentItemInLoop;
                        } else if (nowPlaying.playbackMode == Playlist.CurrentItemInLoop){
                            nowPlaying.playbackMode = Playlist.Loop;
                        } else {
                            nowPlaying.playbackMode = Playlist.Sequential;
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

                Slider {
                    id: sliderHorizontal1
                    anchors.top: parent.top
                    anchors.topMargin: 0
                    value: mediaplayer.position
                    maximumValue: mediaplayer.duration
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

                Text {
                    id: text1
                    color: "#ffffff"
                    text: getReadableTime(mediaplayer.position)
                    anchors.top: sliderHorizontal1.bottom
                    anchors.topMargin: 0
                    anchors.left: parent.left
                    anchors.leftMargin: 0
                    font.pixelSize: 12
                }

                Text {
                    id: text2
                    color: "#ffffff"
                    text: getReadableTime(mediaplayer.duration)
                    anchors.right: parent.right
                    anchors.rightMargin: 0
                    anchors.top: sliderHorizontal1.bottom
                    font.pixelSize: 12
                    anchors.topMargin: 0
                }
            }




        }

        Rectangle {
            id: overlay
            color: "#00000000"
            z: 10
            anchors.topMargin: 0
            visible: false
            anchors.top: parent.top
            anchors.right: parent.right
            anchors.rightMargin: 0
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            opacity: 0.5

            MouseArea {
                id: mouseArea5
                enabled: false
                anchors.fill: parent
                onClicked: changeState("button")
            }

        }
    }

    Playlist{
        id: nowPlaying
        playbackMode : Playlist.Sequential
    }

    MediaPlayer {
        id: mediaplayer
        playlist: nowPlaying
        autoLoad: true
        audioRole: MediaPlayer.MusicRole
        onPaused: {
            playButton.imageSource = "qrc:/qml/icons/play.png";
        }
        onStopped: {
            playButton.imageSource = "qrc:/qml/icons/play.png";
        }
        onPlaying: {
            playButton.imageSource = "qrc:/qml/icons/pause.png";
        }

        function getArtist(){
            var m = metaData;
            return m.contributingArtist?m.contributingArtist:
                                         m.contributingArtist?m.leadPerformer:
                                                               m.contributingArtist?m.albumArtist:
                                                                                     m.contributingArtist?m.metaData.author:
                                                                                                           m.contributingArtist?m.writer:"";
        }
        function setPlaylist(model){
            playlist.clear();
            var itemToPlay = 0;
            for(var i=0; i<model.length; i++){
                if(model[i].playNow)
                    itemToPlay = i;
                playlist.addItem("file://"+model[i].path);
            }
            nowPlaying.currentIndex = itemToPlay;
            play();
        }
    }

    MediaList {
        id: mediaList
        width: parent.width * 0.7
        anchors.leftMargin: -1 * width
        opacity: 0
        anchors.top: top_menu.bottom
        anchors.topMargin: 0
        anchors.left: main.left
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        onItemClicked: {
            changeState("button");
            nowPlayingList.model = model;
            thumbnail_image.source = thumbnail;
        }
        onBack: changeState("toContainer")
    }

    MediaContainerList {
        id: media_container_list
        width: parent.width * 0.7
        anchors.leftMargin: parent.width
        opacity: 0
        anchors.top: top_menu.bottom
        anchors.topMargin: 0
        anchors.left: main.left
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        onItemClicked: {
            switch(itemData.item_type){
            case "folders":
                changeState("toList");
                mediaList.model = MediaPlayerPlugin.audioFolderContent(itemData.id).data;
                mediaList.thumbnail = itemData.thumbnail;
                mediaList.title = itemData.name;
                mediaList.sub_title = itemData.path;
                break;
            case "playlists":
                changeState("toList");
                mediaList.model = MediaPlayerPlugin.getPlaylistContent(itemData.path, itemData.name);
                break;
            case "artists":
                changeState("toList");
                mediaList.model = MediaPlayerPlugin.getArtistContent(itemData.name).data;
                break;
            case "albums":
                changeState("toList");
                mediaList.model = MediaPlayerPlugin.getAlbumContent(itemData.name).data;
                break;
            case "genres":
                changeState("toList");
                mediaList.model = MediaPlayerPlugin.getGenreContent(itemData.name).data;
                break;
            case "songs":
                changeState("button");
                nowPlayingList.model = itemData.data;
                thumbnail_image.source = itemData.thumbnail;
                break;
            default:
                break;
            }
        }
    }


    MediaDrawer {
        id: mediaDrawer
        width: parent.width * 0.3
        top_margin: top_menu.height
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.top: parent.top
        anchors.topMargin: 0
        anchors.right: main.left
        anchors.rightMargin: 0
        onItemClicked: {
            media_container_list.icon = icon;
            media_container_list.name = name;
            media_container_list.model = 0;
            media_container_list.item_type = item_type;
            switch(item_type){
            case "folders":
                media_container_list.model = MediaPlayerPlugin.audioFolders;
                break;
            case "playlists":
                media_container_list.model = MediaPlayerPlugin.playlists;
                break;
            case "artists":
                media_container_list.model = MediaPlayerPlugin.getArtists();
                break;
            case "albums":
                media_container_list.model = MediaPlayerPlugin.getAlbums();
                break;
            case "genres":
                media_container_list.model = MediaPlayerPlugin.getGenres();
                break;
            case "songs":
                media_container_list.model = MediaPlayerPlugin.getSongs();
                break;
            default:
                media_container_list.model = 0;
                break;
            }
            __media_player_layout.changeState("toContainer");
        }
    }

    NowPlayingList {
        id: nowPlayingList
        x: -1* parent.width * 0.3
        width: parent.width * 0.3
        transformOrigin: Item.Center
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.top: top_menu.bottom
        anchors.topMargin: 0
        currentPlaying: nowPlaying.currentIndex
        onItemClicked: {
            nowPlaying.currentIndex = index;
            changeState("button");
            mediaplayer.play();
        }
        onModelChanged: {
            mediaplayer.setPlaylist(model)
        }
    }

    TopMenu{
        id: top_menu
        height: parent.height*0.10
        anchors.top: parent.top
        anchors.topMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0
        onMenuClicked: changeState("button")
        bg_opacity: overlay.opacity
    }


    states: [
        State {
            name: "drawer"
            PropertyChanges {
                target: main
                anchors.topMargin: 0
                anchors.rightMargin: -1* parent.width * 0.3
                anchors.leftMargin: parent.width * 0.3
            }

            PropertyChanges {
                target: overlay
                visible: true
            }

            PropertyChanges {
                target: mouseArea5
                enabled: true
            }
        },
        State {
            name: "media list"
            PropertyChanges {
                target: main
                anchors.leftMargin: parent.width * 0.3
                anchors.rightMargin: -1* parent.width * 0.3
                anchors.topMargin: 0
            }

            PropertyChanges {
                target: overlay
                color: "#000000"
                opacity: 0.9
                visible: true
            }

            PropertyChanges {
                target: __media_player_layout
                clip: true
            }

            PropertyChanges {
                target: mediaList
                anchors.leftMargin: 0
                opacity: 1
                visible: true
            }
        },
        State {
            name: "now playing"

            PropertyChanges {
                target: nowPlayingList
                x: 0
            }

            PropertyChanges {
                target: mouseArea5
                enabled: true
            }

            PropertyChanges {
                target: overlay
                visible: true
            }
        },
        State {
            name: "container list"
            PropertyChanges {
                target: main
                anchors.topMargin: 0
                anchors.rightMargin: -1* parent.width * 0.3
                anchors.leftMargin: parent.width * 0.3
            }

            PropertyChanges {
                target: overlay
                color: "#000000"
                opacity: 0.9
                visible: true
            }

            PropertyChanges {
                target: media_container_list
                opacity: 1
                anchors.leftMargin: 0
                visible: true
            }

            PropertyChanges {
                target: __media_player_layout
                clip: true
            }
        }
    ]
    transitions:
        Transition {
        SequentialAnimation {
            NumberAnimation { properties: "anchors.leftMargin,anchors.rightMargin,opacity,width,x"; duration: 250}
            NumberAnimation { properties: "visible"; duration: 1}
        }
    }


    Settings {
        id: mediaplayer_settings
        property alias nowPlaying: nowPlayingList.model
        property alias nowPlayingCurrentIndex: nowPlaying.currentIndex
        property alias thumbnailImage: thumbnail_image.source
    }
}
