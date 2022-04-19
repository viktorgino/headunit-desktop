import QtQml 2.2
import QtQuick 2.6

import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtBluetooth 5.7
import org.kde.bluezqt 1.0 as BluezQt
import HUDTheme 1.0

ThemeRoot {
    id:__root
    property BluezQt.Manager bluezManager : BluezQt.Manager
    property BluezQt.Device bluezDevice : bluezManager.deviceForUbi(pluginContext.activeDevice)
    property BluezQt.MediaPlayer bluezMediaPlayer : bluezDevice.mediaPlayer

    Connections {
        target: bluezManager
        onInitFinished : {
            bluezDevice = bluezManager.deviceForUbi(pluginContext.activeDevice)
        }
    }

    Connections{
        target: bluezMediaPlayer
        ignoreUnknownSignals: true
        onStatusChanged : {
            if(bluezMediaPlayer.status === 0 ){
                pluginContext.mediaPlaybackStarted()
            }
        }
    }

    ToolBar {
        id: toolBar
        anchors.top: parent.top
        anchors.topMargin: 8
        anchors.right: parent.right
        anchors.rightMargin: 8
        anchors.left: parent.left
        anchors.leftMargin: 8
        background: Rectangle {
            implicitHeight: 40
            color: HUDStyle.colors.formBox
        }

        ToolButton {
            id: debugMenu
            anchors.left: parent.left
            Layout.fillHeight: true
            Layout.fillWidth: true
            ImageButton {
                width: 30
                height: 30
                anchors.verticalCenter: parent.verticalCenter
                imageSource : "qrc:/qml/icons/bug.png"
                anchors.horizontalCenter: parent.horizontalCenter
                color: HUDStyle.colors.headingText1
                onClicked: menu.open()
            }
            Menu {
                id: menu
                x: -width+parent.width
                y: parent.height
                width: 200

                MenuItem {
                    id: menuItem
                    text: "Bluetooth"

                    MenuSwitch {
                        id:btPowerSwitch
                        anchors.right: parent.right
                        anchors.rightMargin: 0
                        checked: bluezManager.adapters[0].powered
                    }
                    MouseArea {
                        anchors.fill: btPowerSwitch
                        onClicked: bluezManager.adapters[0].powered = !btPowerSwitch.checked
                    }
                }
                MenuItem {
                    text: "Visible"

                    MenuSwitch {
                        id:btVisibilitySwitch
                        anchors.right: parent.right
                        anchors.rightMargin: 0
                        checked: bluezManager.adapters[0].discoverable
                    }
                    MouseArea {
                        anchors.fill: btVisibilitySwitch
                        onClicked: bluezManager.adapters[0].discoverable = !btVisibilitySwitch.checked
                    }
                }
                MenuItem {
                    text: "Pairable"

                    MenuSwitch {
                        id:btPairableSwitch
                        anchors.right: parent.right
                        anchors.rightMargin: 0
                        checked: bluezManager.adapters[0].pairable
                    }
                    MouseArea {
                        anchors.fill: btPairableSwitch
                        onClicked: bluezManager.adapters[0].pairable = !btPairableSwitch.checked
                    }
                }
                MenuItem {
                    Rectangle {
                        height: 1
                        color: "#1E000000"
                        anchors.right: parent.right
                        anchors.rightMargin: 8
                        anchors.left: parent.left
                        anchors.leftMargin: 8
                    }
                    ThemeText{
                        height: font.pointSize+16
                        anchors.right: parent.right
                        anchors.rightMargin: 8
                        anchors.left: parent.left
                        anchors.leftMargin: 8
                        text: "Devices"
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                    }
                }
                MenuItem{
                    height: menuListView.height
                    ListView {
                        id:menuListView
                        width : 150
                        height: count * 30
                        model: BluezQt.DevicesModel {
                            manager:bluezManager
                        }
                        delegate : Item {
                            id: item1
                            height: 30
                            width : 200
                            Text {
                                text: Name
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.left: parent.left
                                anchors.leftMargin: 6
                            }

                            MenuSwitch {
                                id:connectionSwitch
                                anchors.verticalCenter: parent.verticalCenter
                                checked: Connected
                                anchors.right: removeRect.left
                            }

                            MouseArea {
                                anchors.fill: connectionSwitch
                                onClicked: {
                                    if(Connected) {
                                        Device.disconnectFromDevice();
                                    } else {
                                        Device.connectToDevice();
                                    }
                                }
                            }

                            Rectangle {
                                id: removeRect
                                anchors.verticalCenter: parent.verticalCenter
                                width: 15
                                height: 15
                                anchors.right: parent.right
                                anchors.rightMargin: 5
                                color: "#da5454"
                                radius: width / 2
                                Text {
                                    id:removeText
                                    text: "-"
                                    anchors.fill: parent
                                    horizontalAlignment: Text.AlignHCenter
                                    verticalAlignment: Text.AlignVCenter
                                }
                                MouseArea {
                                    anchors.fill: parent
                                    onClicked: {
                                        bluezManager.adapters[0].removeDevice(Device)
                                    }
                                }
                            }

                        }
                    }
                }
            }
        }

        ImageButton {
            width: 30
            height: 30
            anchors.left: debugMenu.right
            anchors.leftMargin: 8
            anchors.verticalCenter: parent.verticalCenter
            imageSource: "qrc:/qml/icons/bluetooth.png"
            color: HUDStyle.colors.headingText1
            onClicked: {
                connectionOverlay.visible = true
                pluginContext.enablePairing()
            }
        }
        ImageButton {
            width: 30
            height: 30
            anchors.verticalCenter: parent.verticalCenter
            color: HUDStyle.colors.headingText1
            anchors.right: networkName.left
            anchors.rightMargin: 16
            imageSource: "qrc:/qml/icons/svg/mic-a.svg"
            visible: pluginContext.Handsfree.online
            onClicked: {
                pluginContext.Handsfree.activateVoiceControl()

            }
        }

        ThemeHeaderText {
            id: notConnected
            text: "Not Connected"
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            anchors.rightMargin: 16
            visible: !pluginContext.Handsfree.online
        }


        ThemeHeaderText {
            id: networkName
            text: pluginContext.Handsfree.networkName
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: batteryLevel.left
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            anchors.rightMargin: 16
            visible: pluginContext.Handsfree.online
        }

        ThemeHeaderText {
            id: batteryLevel
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: signal.left
            text: qsTr("Battery: ") + pluginContext.Handsfree.batteryCharge
            horizontalAlignment: Text.AlignRight
            verticalAlignment: Text.AlignVCenter
            anchors.rightMargin: 16
            visible: pluginContext.Handsfree.online
        }

        ThemeHeaderText {
            id: signal
            text: qsTr("Signal: ") + pluginContext.Handsfree.signalStrength + "%"
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            horizontalAlignment: Text.AlignRight
            verticalAlignment: Text.AlignVCenter
            anchors.rightMargin: 8
            visible: pluginContext.Handsfree.online
        }


    }

    Rectangle {
        color: HUDStyle.colors.formBackground
        anchors.top: toolBar.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.rightMargin: 8
        anchors.leftMargin: 8
        anchors.bottomMargin: 8
    }

    property Component currentComponent : phoneComponent
    Loader {
        id:mainLoader
        anchors.top: toolBar.bottom
        anchors.right: parent.right
        anchors.bottom: bottomButtons.top
        anchors.bottomMargin: 8
        anchors.left: parent.left
        anchors.rightMargin: 8
        anchors.leftMargin: 8
        anchors.topMargin: 8
        sourceComponent:pluginContext.activeDevice !== "" ? currentComponent : null
    }

    Component {
        id:phoneComponent
        RowLayout {
            anchors.fill: parent
            Dialer {
                id:dialer
                Layout.fillHeight: true
                Layout.fillWidth: true
            }
            Contacts{
                id:phoneStack
                Layout.fillHeight: true
                Layout.fillWidth: true
                dialed_num: dialer.dialed_num
                onDial: {
                    pluginContext.Handsfree.dialNumber(number);
                }
            }
        }
    }



    Component {
        id:mediaPlayerComponent
        MediaPlayer {
            mediaPlayerObj: bluezMediaPlayer
        }
    }


    Item {
        id: bottomButtons
        height: 40
        anchors.left: parent.left
        anchors.leftMargin: 16
        anchors.right: parent.right
        anchors.rightMargin: 16
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 16

        RowLayout {
            spacing: 8
            anchors.fill: parent
            Rectangle{
                color: HUDStyle.colors.formBox
                Layout.fillHeight: true
                Layout.fillWidth: true

                ImageButton{
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: parent.height * 0.2
                    anchors.topMargin: parent.height * 0.2
                    imageSource: "qrc:/qml/icons/android-call.png"
                    onClicked: {
                        currentComponent = phoneComponent
                    }
                }
            }

            Rectangle {
                color: HUDStyle.colors.formBox
                Layout.fillHeight: true
                Layout.fillWidth: true

                ImageButton{
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: parent.height * 0.2
                    anchors.topMargin: parent.height * 0.2
                    imageSource: "qrc:/qml/icons/music-note.png"
                    onClicked: {
                        currentComponent = mediaPlayerComponent
                    }
                }
            }
        }
    }

    //    Connections{
    //        target: AndroidAuto
    //        ignoreUnknownSignals: true
    //        onBtConnectionRequest:{
    //            for(var i = 0 ; i < bluezManager.devices.length; i++){
    //                if(bluezManager.devices[i].address === address){
    //                    if(!bluezManager.devices[i].connected)
    //                        bluezManager.devices[i].connectToDevice();
    //                } else {
    //                    bluezManager.devices[i].disconnectFromDevice();
    //                }
    //            }
    //        }
    //    }
    Connections{
        target: pluginContext
        onStart : {
            bluezMediaPlayer.play()
        }
        onStop : {
            bluezMediaPlayer.stop()
        }
        onPrevTrack : {
            bluezMediaPlayer.next()
        }
        onNextTrack : {
            bluezMediaPlayer.next()
        }
        onActiveDeviceChanged: {
            if(pluginContext.activeDevice){
                bluezDevice = bluezManager.deviceForUbi(pluginContext.activeDevice)
            } else {
                console.log("Active device removed")
                bluezDevice = 0
            }
        }
    }
    Rectangle {
        id: connectionOverlay
        color: HUDStyle.colors.formBackground
        anchors.fill: parent
        visible: false

        MouseArea {
            id: mouseArea
            anchors.fill: parent
            onClicked: {
                connectionOverlay.visible = false
                pluginContext.disablePairing()
            }
        }
        ConnectionOverlay {
            width: parent.width * 0.8
            height: parent.height * 0.8
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            bluezManager : __root.bluezManager
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
