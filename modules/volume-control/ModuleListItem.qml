import QtQuick 2.11
import QtQuick.Controls 2.4

import QtQuick.Layouts 1.3

import HUDTheme 1.0

Item {
    id: __root
    height: 50

    property alias value : slider.value
    property int originalVolume : 100

    ThemeFormText {
        id: label
        height: 30
        anchors.left: parent.left
        anchors.right: mute.left
        verticalAlignment: Text.AlignVCenter
        anchors.leftMargin: 0
        text: modelData
        elide: Text.ElideMiddle
    }

    ThemeSlider {
        id:slider
        height: 20
        anchors.left: parent.left
        anchors.right: volumeLabel.left
        anchors.bottom: parent.bottom
        wheelEnabled: true
        anchors.rightMargin: 0
        stepSize: 1
        from: 0
        to: 100
        Component.onCompleted: {
//            value = Math.round(modelData / (655.36))
        }
        onValueChanged: {
//            modelData = value * 655.36
        }
    }

    ImageButton {
        height: 30
        anchors.right: parent.right
        id: mute
        width: 30
        checkable: true
        imageSource: "qrc:/qml/icons/volume-high.png"
        activeImageSource : "qrc:/qml/icons/volume-mute.png"
        color: "#ffffff"
        activeColor: "#ff0000"
        checked : slider === 0
        onCheckedChanged: {
            if(checked){
                __root.originalVolume = slider.value
                slider.value = 0
            } else {
                slider.value = __root.originalVolume
            }

        }
    }

    Item {
        id: volumeLabel
        width: 60
        height: 20
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        ThemeFormText {
            id: percentLabel
            text: qsTr("%")
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 0
        }

        ThemeFormText {
            text : slider.value
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: percentLabel.left
        }
    }
}

