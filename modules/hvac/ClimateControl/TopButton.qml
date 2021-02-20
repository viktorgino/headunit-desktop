import QtQuick 2.6
import QtGraphicalEffects 1.0
import QtQuick.Layouts 1.0
Item {
    id: top_button
    property string imageSrc: "qrc:/HVAC/icons/alert.png"
    property int margin: height*0.1
    property int ringSize: height*0.05
    property string name: ""
    property bool active: false
    signal clicked();
    function getInnerSquareSide(outterSide){
        return Math.sqrt(2*Math.pow(outterSide/2,2))
    }


    Item {
        id: item1
        anchors.top: parent.top
        anchors.topMargin: top_button.margin
        anchors.bottom: parent.bottom
        anchors.bottomMargin: top_button.margin
        anchors.left: parent.left
        anchors.leftMargin: top_button.margin
        anchors.right: parent.right
        anchors.rightMargin: top_button.margin

        Image {
            id: image3
            width:getInnerSquareSide(rectangle1.height)
            height:getInnerSquareSide(rectangle1.height)
            anchors.horizontalCenter: rectangle2.horizontalCenter
            anchors.verticalCenter: rectangle2.verticalCenter
            fillMode: Image.PreserveAspectFit
            source:top_button.imageSrc
            mipmap:true
            smooth: true
        }

        Rectangle {
            id: rectangle2
            color: "#4cffffff"
            radius: height/2
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: 0
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            visible: top_button.active
            width:height

            Rectangle {
                id: rectangle1
                radius: height/2
                anchors.right: parent.right
                anchors.rightMargin: top_button.ringSize
                anchors.left: parent.left
                anchors.leftMargin: top_button.ringSize
                anchors.top: parent.top
                anchors.topMargin: top_button.ringSize
                anchors.bottom: parent.bottom
                anchors.bottomMargin: top_button.ringSize
                visible: true
                color: "#dddddd"


            }
        }

        ColorOverlay {
            id: image3_overlay
            color: "#212121"
            anchors.fill: image3
            visible: top_button.active
            enabled: true
            source: image3
            smooth: true
        }


        MouseArea {
            id: mouseArea1
            anchors.fill: parent
            onClicked: top_button.clicked()
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;formeditorColor:"#c0c0c0";height:480;width:640}
}
##^##*/
