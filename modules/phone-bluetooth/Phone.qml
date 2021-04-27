import QtQml 2.2
import QtQuick 2.6
import MeeGo.QOfono 0.2
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtBluetooth 5.7
import org.kde.bluezqt 1.0 as BluezQt
import HUDTheme 1.0

Item {
    id:__root
    property QtObject bluezManager : BluezQt.Manager
    property QtObject bluezDevice : bluezManager.deviceForUbi(PhoneBluetooth.activeDevice)
    property QtObject bluezMediaPlayer : bluezDevice.mediaPlayer

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
            color: HUDStyle.Colors.formBox
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
            onClicked: {
                connectionOverlay.visible = true
                PhoneBluetooth.enablePairing()
            }
        }

        ThemeHeaderText {
            text: netreg.name?netreg.name:"No Connection"
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: batteryLevel.left
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            anchors.rightMargin: 16
        }

        ThemeHeaderText {
            id: batteryLevel
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: signal.left
            //                text: qsTr("Battery: ")+hands_free.batteryChargeLevel
            horizontalAlignment: Text.AlignRight
            verticalAlignment: Text.AlignVCenter
            anchors.rightMargin: 16
        }

        ThemeHeaderText {
            id: signal
            text: qsTr("Signal: ")+netreg.strength + "%"
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            horizontalAlignment: Text.AlignRight
            verticalAlignment: Text.AlignVCenter
            anchors.rightMargin: 8
        }


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
        sourceComponent:PhoneBluetooth.activeDevice !== "" ? currentComponent : null
    }

    Component {
        id:phoneComponent
        RowLayout {
            anchors.fill: parent
            Dialer {
                id:dialer
                Layout.fillHeight: true
                Layout.fillWidth: true
                onDial: vcm.dial(number,"")
                onHangup: vcm.hangupAll()
                onVoice_rec: {
                    PhoneBluetooth.toggleVoice()
                }
            }
            Contacts{
                id:phoneStack
                Layout.fillHeight: true
                Layout.fillWidth: true
                dialed_num: dialer.dialed_num
                onDial: {
                    vcm.dial(number,"")
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
        anchors.leftMargin: 8
        anchors.right: parent.right
        anchors.rightMargin: 8
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 8

        Rectangle {
            color: HUDStyle.Colors.formBox
            anchors.fill: parent
        }

        RowLayout {
            spacing: 0
            anchors.fill: parent
            Item{
                Layout.fillHeight: true
                Layout.fillWidth: true

                ImageButton{
                    height: parent.height * 0.8
                    width: height
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    imageSource: "qrc:/qml/icons/android-call.png"
                    color: "#ffffff"
                    onClicked: {
                        currentComponent = phoneComponent
                    }
                }
            }

            Item{
                Layout.fillHeight: true
                Layout.fillWidth: true

                ImageButton{
                    height: parent.height * 0.8
                    width: height
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    imageSource: "qrc:/qml/icons/music-note.png"
                    color: "#ffffff"
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

    Rectangle {
        id: connectionOverlay
        color: HUDStyle.Colors.formBackground
        anchors.fill: parent
        visible: false

        MouseArea {
            id: mouseArea
            anchors.fill: parent
            onClicked: {
                connectionOverlay.visible = false
                PhoneBluetooth.disablePairing()
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

    OfonoNetworkRegistration {
        id: netreg
        modemPath : "/hfp" + PhoneBluetooth.activeDevice
    }

    OfonoModem{
        id:ofonomodem
        modemPath : "/hfp" + PhoneBluetooth.activeDevice
    }

    OfonoVoiceCallManager{
        id:vcm
        modemPath : "/hfp" + PhoneBluetooth.activeDevice
    }


    OfonoHandsfree{
        id:hands_free
        modemPath : "/hfp" + PhoneBluetooth.activeDevice
        onModemPathChanged : {
            batteryLevel.text = "Battery : " + batteryChargeLevel
        }
    }


}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
