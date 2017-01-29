import QtQuick 2.0
import QtGStreamer 1.0


VideoItem {
    id: aaVideo
    width: 800
    height: 480
    surface: videoSurface1

    MouseArea {
        id: mouseArea1
        anchors.fill: parent
        onPressed:{
            headunit.outputHeight = aaVideo.height
            headunit.outputWidth = aaVideo.width
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
