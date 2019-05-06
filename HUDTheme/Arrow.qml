import QtQuick 2.0

Item {
    id:root

    property int lineSize : 15
    property int headSize : 200
    property double __arrowHeadOffset : Math.sqrt(Math.pow(lineSize/2,2)*2)/2
    property color __current_color : color
    property color color : "#ffffff"
    property color active_color : "#ff5722"
    function toggleColor (){
        if(__current_color == color)
            __current_color = active_color
        else
            __current_color = color
    }

    Item {
        id: item1
        y:(parent.height-headSize)/2 + parent.__arrowHeadOffset
        width: parent.headSize
        height: parent.headSize
        anchors.rightMargin: parent.__arrowHeadOffset
        anchors.right: parent.right
        transformOrigin: Item.Right
        rotation: 45

        Rectangle {
            color: __current_color
            height: root.lineSize
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 0
            anchors.left: parent.left
            anchors.leftMargin: 0
        }
    }


    Item {
        y: (parent.height-headSize)/2 - parent.__arrowHeadOffset
        width: parent.headSize
        height: parent.headSize
        anchors.leftMargin: 0-parent.__arrowHeadOffset
        anchors.left: parent.right
        transformOrigin: Item.Left
        rotation: 135

        Rectangle {
            color: __current_color
            height: root.lineSize
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 0
            anchors.left: parent.left
            anchors.leftMargin: 0
        }
    }


    Rectangle {
        color: __current_color
        y: parent.height/2 - height/2
        height: parent.lineSize
        rotation: 180
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: parent.__arrowHeadOffset*2
    }
}
