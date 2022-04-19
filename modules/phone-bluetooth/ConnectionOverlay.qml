import QtQuick 2.7
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3

import org.kde.bluezqt 1.0 as BluezQt

Item {
    id: item2
    property QtObject bluezManager
    Rectangle {
        id: rectangle
        color: "#ffffff"
        anchors.fill: parent

        Text {
            id: connectText
            color: "#707070"
            text: qsTr("To connect a Bluetooth device search for:")
            anchors.top: parent.top
            font.pixelSize: 16
            anchors.topMargin: 8
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Rectangle {
            id: divider
            y: parent.height * 0.2
            width: parent.width * 0.9
            height: 1
            color: "#707070"
            border.width: 0
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Item {
            id: item1
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: connectText.bottom
            anchors.bottom: divider.top
            anchors.leftMargin: 0

            Text {
                id: adapterName
                color: "#707070"
                text: pluginSettings.adapterName
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 26
                fontSizeMode: Text.Fit
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }

        Item {
            id: item3
            width: parent.width * 0.8
            anchors.top: divider.bottom
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: 8
            anchors.bottomMargin: 8
            Text {
                id: connectedDeviceText
                color: "#707070"
                text: qsTr("Connected Device")
                anchors.top: parent.top
                font.pixelSize: 20
            }

            Item {
                id: connectedDevice
                anchors.left: connectedDeviceText.left
                anchors.top: connectedDeviceText.bottom
                height: 30

                Text {
                    text: pluginContext.activeDeviceName
                    anchors.verticalCenter: parent.verticalCenter
                    font.bold: true
                }
            }

            Text {
                id: pairedDevicesText
                color: "#707070"
                text: qsTr("Paired Devices")
                anchors.top: connectedDevice.bottom
                font.pixelSize: 20
            }

            ListView {
                id: listView
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: pairedDevicesText.bottom
                anchors.bottom: parent.bottom
                clip: true
                anchors.topMargin: 8
                delegate:
                    Item {
                    height: 30
                    width: listView.width
                    Text {
                        text: pluginContext.pairedDevices[modelData]
                        anchors.verticalCenter: parent.verticalCenter
                        font.bold: true
                    }

                    MouseArea{
                        anchors.fill: parent
                        onClicked: {
                            pluginContext.connectToDevice(modelData)
                        }
                    }
                }
                ScrollBar.vertical: ScrollBar {}
                model: Object.keys(pluginContext.pairedDevices)
            }
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:350;width:512}D{i:10}D{i:6}
}
##^##*/
