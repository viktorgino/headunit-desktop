import QtQuick 2.11
import QtQuick.Controls 2.3
import QtGraphicalEffects 1.12

Item {
    id: __root

    property int steps : HVACPlugin.hvacSettings.FanSteps
    property double value: 5
    property color backgroundColor: "#212121"
    property bool inverted : false
    property bool autoEnabled : false
    property alias selected: fanSelected.visible

    onValueChanged: {
        canvas.requestPaint()
    }
    onAutoEnabledChanged: {
        canvas.requestPaint()
    }

    Item {
        anchors.fill: parent
        anchors.rightMargin: parent.width * 0.1
        anchors.leftMargin: parent.width * 0.1
        anchors.topMargin: parent.height * 0.1

        Item {
            id: fanSelectWrapper
            height: parent.height * 0.2
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom

            Rectangle {
                id: fanSelected
                width: parent.width * 0.8
                height: parent.height * 0.5
                color: "#ffffff"
                radius: height / 2
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }

        Item{
            id: item2
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: fanSelectWrapper.top
            Item{
                id: fanLevelWrapper
                width: parent.height * 2 >= parent.width ? parent.width : height * 2
                height: parent.height * 2 >= parent.width ? width / 2 : parent.height
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                clip: false


                Canvas {
                    visible: true
                    id:canvas
                    anchors.fill: parent
                    smooth: true
                    contextType: "2d"

                    Path {
                        id:outlinePath
                        startX: canvas.width - 10; startY: canvas.height - 10
                        PathLine { x: canvas.width - 10; y: 10 }
                        PathLine { x: 10; y: canvas.height - 10 }
                        PathLine { x: canvas.width - 10; y: canvas.height - 10 }
                    }
                    Path {
                        id:outlinePathInverted
                        startX: 10; startY: canvas.height - 10
                        PathLine { x: 10; y: 10 }
                        PathLine { x: canvas.width - 10; y: canvas.height - 10 }
                        PathLine { x: 10; y: canvas.height - 10 }
                    }

                    onPaint: {
                        if(context){
                            context.strokeStyle = "#999999";
                            context.lineWidth = 5;
                            context.path = __root.inverted ? outlinePathInverted : outlinePath;
                            context.stroke();

                            if(!autoEnabled){
                                var gradient = context.createLinearGradient(9,0,canvas.width-10 ,0);
                                var gradientLimit =  (1 / (__root.steps - 1)) * __root.value;
                                if(__root.inverted){
                                    gradient.addColorStop(1 - gradientLimit, __root.backgroundColor);
                                    if(1 - gradientLimit < 1){
                                        gradient.addColorStop(1 - gradientLimit+0.01, "#ffffff");
                                    } else {
                                        gradient.addColorStop(1, __root.backgroundColor);
                                    }

                                } else {
                                    gradient.addColorStop(gradientLimit, "#ffffff");
                                    if(gradientLimit < 1){
                                        gradient.addColorStop(gradientLimit+0.01, __root.backgroundColor);
                                    }
                                }

                                context.fillStyle = gradient;
                            } else {
                                context.fillStyle = "#ffffff";
                            }

                            context.fill();
                        }
                    }
                }

                Text {
                    width: parent.width * 0.4
                    visible: __root.autoEnabled && __root.inverted
                    color: "#ffffff"
                    text: qsTr("Auto")
                    anchors.right: parent.right
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    font.pixelSize: 80
                    horizontalAlignment: Text.AlignRight
                    verticalAlignment: Text.AlignTop
                    anchors.rightMargin: 0
                    font.weight: Font.ExtraBold
                    font.bold: true
                    fontSizeMode: Text.Fit
                    minimumPixelSize: 10
                    font.capitalization: Font.AllUppercase
                }

                Text {
                    width: parent.width * 0.4
                    visible: __root.autoEnabled && !__root.inverted
                    color: "#ffffff"
                    text: qsTr("Auto")
                    anchors.left: parent.left
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    font.pixelSize: 80
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignTop
                    anchors.leftMargin: 0
                    font.weight: Font.ExtraBold
                    font.bold: true
                    fontSizeMode: Text.Fit
                    minimumPixelSize: 10
                    font.capitalization: Font.AllUppercase
                }
            }
        }

    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;formeditorColor:"#c0c0c0";height:300;width:300}D{i:1}
}
##^##*/
