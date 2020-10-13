import QtQuick 2.0
import org.freedesktop.gstreamer.GLVideoItem 1.0
import Qt.labs.settings 1.0

Item{
    id:__root

    property int margin_width
    property int margin_height
    property string resolution

    GstGLVideoItem {
        id: aaVideo
        objectName: "aaVideoItem"
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        width:parent.width * AndroidAuto.videoHeight/AndroidAuto.videoWidth < parent.height ? parent.width : height * AndroidAuto.videoWidth/AndroidAuto.videoHeight
        height:parent.width * AndroidAuto.videoHeight/AndroidAuto.videoWidth < parent.height ? width * AndroidAuto.videoHeight/AndroidAuto.videoWidth : parent.height

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

        onWidthChanged: {
            AndroidAuto.outputWidth = aaVideo.width
        }
        onHeightChanged: {
            AndroidAuto.outputHeight = aaVideo.height
        }

        Component.onCompleted:{
            AndroidAuto.outputHeight = aaVideo.height
            AndroidAuto.outputWidth = aaVideo.width
            AndroidAuto.setVideoItem(aaVideo)
        }

    }
}
