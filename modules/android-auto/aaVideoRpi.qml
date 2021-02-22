import QtQuick 2.0
import QtGStreamer 1.0
import Qt.labs.settings 1.0
import QtQml 2.3

Item{
    id:__root
    Rectangle {
        id: rectangle
        color: "#505050"
        anchors.fill: parent
    }
    property int margin_width
    property int margin_height
    property string resolution

    VideoItem {
        id: aaVideo
        anchors.verticalCenter: parent.verticalCenter
        width:parent.width
        height: width * AndroidAuto.videoHeight/AndroidAuto.videoWidth
        surface: AndroidAuto.videoSurface

        MouseArea {
            id: mouseArea1
            anchors.fill: parent
            onPressed:{
                AndroidAuto.mouseDown(Qt.point(mouse.x, mouse.y));
            }
            onPositionChanged:{
                AndroidAuto.mouseMove(Qt.point(mouse.x, mouse.y));
            }
            onReleased:{
                AndroidAuto.mouseUp(Qt.point(mouse.x, mouse.y));
            }
        }

        Keys.onPressed: {
            switch(event.key){
            case Qt.Key_Left:
                console.log("Key Left");
                break;
            }
        }
    }
    Connections{
        target: AndroidAuto
        onVideoResized:{
            aaVideo.height = AndroidAuto.width * AndroidAuto.videoHeight/AndroidAuto.videoWidth
            AndroidAuto.outputHeight = aaVideo.height
            AndroidAuto.outputWidth = aaVideo.width
        }
    }

}
