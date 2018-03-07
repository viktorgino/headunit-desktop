import QtQuick 2.0

Item {
    id: item1
    Image {
        id: image
        height: parent.height * 0.6
        anchors.top: parent.top
        anchors.topMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0
        source: "qrc:/qtquickplugin/images/template_image.png"
    }

}
