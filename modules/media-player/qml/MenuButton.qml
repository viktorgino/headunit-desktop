import QtQuick 2.11

Item {
    id:root
    property bool isActive: false
    Item {
        id: item1
        width: root.width
        height: root.height *0.1

        Rectangle {
            id: rectangle1
            width: root.width
            height: root.height *0.1
            color: "#ffffff"
        }
    }


    Item {
        id: item2
        width: root.width
        height: root.height *0.1
        y:root.height-height
        Rectangle {
            id: rectangle3
            width: root.width
            height: root.height *0.1
            color: "#ffffff"
        }
    }

    states: [
        State {
            name: "Active"
            when: root.isActive

            PropertyChanges {
                target: rectangle2
                x: root.width*0.1
                width: root.width*0.90
                rotation: 180
            }

            PropertyChanges {
                target: rectangle1
                x: 0
                y: parent.height/2-height/2
                width: root.width*0.4
            }

            PropertyChanges {
                target: rectangle3
                x: 0
                y: parent.height/2-height/2
                width: root.width*0.4
            }

            PropertyChanges {
                target: item1
                x: 0
                y: root.height/2 + Math.sqrt(Math.pow(height/2+rectangle3.height/2,2)/2) - Math.sqrt(2*Math.pow(height,2)) + (Math.sqrt(2*Math.pow(height,2))-height)/2
                width: root.width*0.4
                height: root.width*0.4
                rotation: -45
            }

            PropertyChanges {
                target: item2
                x: 0
                width: root.width*0.4
                height: root.width*0.4
                rotation: 45
                y:root.height/2 - Math.sqrt(Math.pow(height/2+rectangle3.height/2,2)/2) + (Math.sqrt(2*Math.pow(height,2))-height)/2
            }
        }
    ]

    Rectangle {
        id: rectangle2
        x: 0
        y: root.height/2 - height/2
        width: root.width
        height: root.height *0.1
        color: "#ffffff"
    }

    transitions: Transition {
        NumberAnimation { properties: "x,y,rotation,height,width"; duration: 250}
    }

}
