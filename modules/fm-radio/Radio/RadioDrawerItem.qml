import QtQuick 2.6

Item{
    height: favourite||listType == "all"?60:0
    anchors.left: parent.left
    anchors.right: parent.right
    opacity: favourite||listType == "all"

    Rectangle {
        id: rectangle1
        color: "#cddc39"
        anchors.fill: parent
        visible: activeStation == name
    }

    Rectangle {
        height: 1
        color: "#ffffff"
        opacity: 0.5
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
    }
    Item {
        anchors.rightMargin: parent.height*0.1
        anchors.leftMargin: parent.height*0.1
        anchors.bottomMargin: parent.height*0.1
        anchors.topMargin: parent.height*0.1
        anchors.fill: parent

        Image {
            id: favimage
            width: height
            fillMode: Image.PreserveAspectFit
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.top: parent.top
            anchors.topMargin: parent.height*0.2
            anchors.bottom: parent.bottom
            anchors.bottomMargin: parent.height*0.2
            source: favourite?"qrc:/qml/icons/svg/android-star.svg":"qrc:/qml/icons/svg/android-star-outline.svg"
            //source: "../icons/svg/android-star-outline.svg"
            mipmap:true

            MouseArea {
                anchors.fill: parent
                onClicked: favourite=!favourite
            }
        }

        Text{
            color: "#ffffff"
            text:name
            font.pixelSize: 10
            anchors.left: favimage.right
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.top: parent.top
            anchors.leftMargin: 16
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
            wrapMode: Text.WordWrap
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    activeStation = name;
                    drawer.itemClicked(frequency,activeStation);
                }
            }
        }
    }
}
