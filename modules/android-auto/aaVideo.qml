import QtQuick 2.0
import org.freedesktop.gstreamer.GLVideoItem 1.0
import Qt.labs.settings 1.0

Item{
    id:__root

    property int margin_width
    property int margin_height
    property string resolution

    Rectangle {
        visible: AndroidAuto.status === 0 /*AndroidAuto.NO_CONNECTION*/ || AndroidAuto.status === 1/*AndroidAuto.VIDEO_WAITING*/
        width: parent.width * 0.6
        height: parent.height * 0.4
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        color: "#66000000"
        Text{
            visible: AndroidAuto.status === 0/*AndroidAuto.NO_CONNECTION*/
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            text:"Please connect a device"
            color: "#ffffff"
        }
        Text{
            visible: AndroidAuto.status === 1/*AndroidAuto.VIDEO_WAITING*/
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            text:"Waiting for video"
            color: "#ffffff"
        }
    }

    GstGLVideoItem {
        id: aaVideo
        objectName: "aaVideoItem"
        visible: AndroidAuto.status === 2 /*AndroidAuto.RUNNING*/
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
