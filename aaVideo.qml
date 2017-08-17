import QtQuick 2.0
import QtGStreamer 1.0
import Qt.labs.settings 1.0

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
        height: width * headunit.videoHeight/headunit.videoWidth
        surface: videoSurface1

        MouseArea {
            id: mouseArea1
            anchors.fill: parent
            onPressed:{
                headunit.mouseDown(Qt.point(mouse.x, mouse.y));
            }
            onPositionChanged:{
                headunit.mouseMove(Qt.point(mouse.x, mouse.y));
            }
            onReleased:{
                headunit.mouseUp(Qt.point(mouse.x, mouse.y));
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
        target: headunit
        onVideoResized:{
            aaVideo.height = aaVideo.width * headunit.videoHeight/headunit.videoWidth
            headunit.outputHeight = aaVideo.height
            headunit.outputWidth = aaVideo.width
        }
    }

}
