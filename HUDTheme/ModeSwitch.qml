import QtQuick 2.0

Item {
    id:__root
    property color colorInactive: "#ffffff"
    property color colorActive: "#000000"
    property color sliderColor: "#212121"
    property int sliderLeftPosition : slider.margin
    property int sliderRightPosition : slide.width - slider.width - slider.margin
    property string leftValue : "left"
    property string rightValue : "right"
    property string leftText : "On"
    property string rightText : "Off"
    property var switchValue : leftValue

    function snapToValue(){
        var snap_limit = slide.width/2 - slider.width/2
        if(slider.x>snap_limit){
            changeSide("right")
        } else {
            changeSide("left")
        }
    }
    function slideClicked(){
        if(slider.x == sliderLeftPosition){
            changeSide("right")
        } else if(slider.x == sliderRightPosition){
            changeSide("left")
        }
    }

    function changeSide(side){
        if(side == "right"){
            slider.x = sliderRightPosition
            left_text.color = colorInactive
            right_text.color = colorActive
            switchValue = rightValue
        }else if(side == "left"){
            slider.x = sliderLeftPosition
            left_text.color = colorActive
            right_text.color = colorInactive
            switchValue = leftValue
        }
    }

    Rectangle {
        id: slide
        height: width * 0.5
        color: sliderColor
        radius: height/2
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0

        Rectangle {
            property int margin:parent.height * 0.05
            id: slider
            x: sliderLeftPosition
            width: parent.height - margin*2
            height: parent.height - margin*2
            radius: width/2
            anchors.verticalCenter: parent.verticalCenter
            color: "#ffffff"
            Behavior on x {
                PropertyAnimation {
                    duration: 100
                }
            }
        }

        Item {
            id: left1
            width: slider.width
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            anchors.top: parent.top
            anchors.rightMargin: slider.margin
            anchors.leftMargin: slider.margin
            anchors.bottomMargin: slider.margin
            anchors.topMargin: slider.margin

            Text {
                id: left_text
                color: colorActive
                text: leftText
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: parent.height/3
            }
        }
        Item {
            id: right1
            width: slider.width
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.top: parent.top
            anchors.rightMargin: slider.margin
            anchors.leftMargin: slider.margin
            anchors.bottomMargin: slider.margin
            anchors.topMargin: slider.margin

            Text {
                id: right_text
                color: colorInactive
                text: rightText
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: parent.height/3
            }
        }

        MouseArea {
            id: mouseArea1
            anchors.fill: parent
            drag.target: slider;
            drag.axis: "XAxis"
            drag.minimumX: sliderLeftPosition
            drag.maximumX: sliderRightPosition
            onReleased: snapToValue()
            onClicked: slideClicked()
        }

    }
    Component.onCompleted: {
        if(switchValue == leftValue)
            changeSide("left")
        else
            changeSide("right")
    }
}
