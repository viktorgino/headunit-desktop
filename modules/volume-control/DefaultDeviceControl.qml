import QtQuick 2.11
import QtQuick.Controls 2.4

import QtQuick.Layouts 1.3
import HUDTheme 1.0
import HUDVolume 1.0

Item {
    id: __root
    height: 50

    property QtObject model
    Component.onCompleted: {
        console.log(typeof model , model)
        for(var dev in model){
            console.log(dev)
        }
    }

    ThemeFormText {
        id: label
        height: 30
        anchors.left: parent.left
        anchors.right: portsMenu.left
        verticalAlignment: Text.AlignVCenter
        anchors.rightMargin: 8
        anchors.leftMargin: 0
        text: model.defaultSink.Description
        elide: Text.ElideMiddle
    }

    ImageButton {
        id: mute
        height: 30
        anchors.right: parent.right
        width: 30
        checkable: true
        imageSource: "qrc:/qml/icons/volume-high.png"
        activeImageSource : "qrc:/qml/icons/volume-mute.png"
        color: "#000000"
        activeColor: "#ff0000"
        checked : device.Muted
        onCheckedChanged: {
            device.Muted = checked;
        }
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
        enabled: device.VolumeWritable
        Component.onCompleted: {
            value = Math.round(Volume / (655.36))
        }
        onValueChanged: {
            device.Volume = value * 655.36
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

    ToolButton {
        id: portsMenu
        width: 30
        height: 30
        anchors.right: mute.left
        anchors.rightMargin: 8
        ImageButton {
            width: 30
            height: 30
            imageSource : "qrc:/qml/icons/navicon.png"
            color: "#000000"
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            onClicked: menu.open()
        }
        Menu {
            id:menu
            x: -width+parent.width
            y: parent.height
            width: 200

            ButtonGroup {
                buttons: portItems.children
            }

            Repeater{
                id:portItems
                model : {
                    var ports = []
                    Ports.forEach(function(port) {
                        ports.push(port.description);
                    });
                    return ports;
                }
                MenuItem {
                    id: menuItem
                    CheckBox {
                        width: 20
                        height : 20
                        text: qsTr("Parent")
                        anchors.verticalCenter: parent.verticalCenter
                        checked : (index === device.ActivePortIndex)
                    }
                    ThemeFormText {
                        x:20
                        height : 20
                        anchors.verticalCenter: parent.verticalCenter
                        text:modelData
                    }
                    MouseArea{
                        anchors.fill: parent
                        onClicked: {
                            device.ActivePortIndex = index
                        }
                    }
                }
            }
        }
    }

}

