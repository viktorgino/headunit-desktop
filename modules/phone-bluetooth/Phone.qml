import QtQml 2.2
import QtQuick 2.6
import MeeGo.QOfono 0.2
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtBluetooth 5.7
import org.kde.bluezqt 1.0 as BluezQt
Item {
    id:__root
    property QtObject bluezManager : BluezQt.Manager

    ToolBar {
        id: toolBar
        anchors.top: parent.top
        anchors.topMargin: 8
        anchors.right: parent.right
        anchors.rightMargin: 8
        anchors.left: parent.left
        anchors.leftMargin: 8

        RowLayout {
            anchors.fill: parent
            ToolButton {
                Image {
                    width: 40
                    height: 40
                    source: "qrc:/qml/icons/gear-a.png"
                    fillMode: Image.PreserveAspectFit
                    mipmap:true
                }
                onClicked: menu.open()
                Menu {
                    id: menu
                    x: -width+parent.width
                    y: parent.height

                    MenuItem {
                        id: menuItem
                        text: "Bluetooth"

                        Switch {
                            id:btPowerSwitch
                            anchors.right: parent.right
                            anchors.rightMargin: 0
                            onCheckedChanged: {
                                bluezManager.adapters[0].powered = checked
                            }
                        }
                        Binding {
                            target:btPowerSwitch
                            property: "checked"
                            value: bluezManager.operational && bluezManager.adapters[0] && bluezManager.adapters[0].powered
                        }
                    }
                    MenuItem {
                        text: "Visible"

                        Switch {
                            id:btVisibilitySwitch
                            anchors.right: parent.right
                            anchors.rightMargin: 0
                            onCheckedChanged: {
                                bluezManager.adapters[0].discoverable = checked
                            }
                        }
                        Binding {
                            target:btVisibilitySwitch
                            property: "checked"
                            value: bluezManager.operational && bluezManager.adapters[0] && bluezManager.adapters[0].discoverable
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
                        Text{
                            height: font.pointSize+16
                            anchors.right: parent.right
                            anchors.rightMargin: 8
                            anchors.left: parent.left
                            anchors.leftMargin: 8
                            text: "Devices"
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                            font.pointSize: 11
                        }
                    }
                    Instantiator {
                        model: BluezQt.DevicesModel { }
                        onObjectAdded: menu.addItem(object)
                        onObjectRemoved: menu.removeItem(index + 3)
                        delegate: MenuItem {
                            text: Name

                            Switch {
                                id:connectionSwitch
                                checked: Connected
                                enabled: true
                                anchors.right: parent.right
                                anchors.rightMargin: 0
                            }

                            MouseArea{
                                anchors.fill: parent
                                onClicked: {
                                    if(Connected) {
                                        Device.disconnectFromDevice();
                                    } else {
                                        Device.connectToDevice();
                                    }
                                }
                            }
                        }
                    }
                }
            }
            Text {
                id: text1
                text: netreg.name?netreg.name:"No Connection"
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                font.pixelSize: 22
            }

            Text {
                id: text4
                text: qsTr("Signal: ")+netreg.strength + "%"
                anchors.right: parent.right
                anchors.rightMargin: 8
                font.pixelSize: 22
                anchors.verticalCenter: parent.verticalCenter
            }
        }
    }

    Loader{
        anchors.top: toolBar.bottom
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.rightMargin: 8
        anchors.leftMargin: 8
        anchors.bottomMargin: 8
        anchors.topMargin: 8
        active:PhoneBluetooth.deviceIndex>=0
        sourceComponent:Item {
            id: item1
            anchors.fill: parent
            RowLayout {
                id: rowLayout
                anchors.fill: parent
                Item {
                    id: item3
                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    Dialer {
                        id:dialer
                        anchors.fill: parent
                        onDial: vcm.dial(number,"")
                        onHangup: vcm.hangupAll()
                        onVoice_rec: hands_free.voiceRecognition = state
                    }
                }
                Item {
                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    StackView{
                        id:phoneStack
                        clip: true
                        initialItem: contactsComponent
                        anchors.left: parent.left
                        anchors.top: parent.top
                        anchors.right: parent.right
                        anchors.bottom: bottomButtons.top
                        anchors.bottomMargin: 0
                    }
                    Item {
                        id: bottomButtons
                        height: parent.height*0.1
                        anchors.left: parent.left
                        anchors.leftMargin: 0
                        anchors.right: parent.right
                        anchors.rightMargin: 0
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: 0

                        Rectangle {
                            color: "#212121"
                            anchors.fill: parent
                        }

                        RowLayout {
                            spacing: 0
                            anchors.fill: parent
                            Item{
                                Layout.fillHeight: true
                                Layout.fillWidth: true
                                MouseArea{
                                    anchors.fill: parent
                                    onClicked: phoneStack.push(contactsComponent)
                                }
                                Image{
                                    height: parent.height * 0.8
                                    width: height
                                    anchors.verticalCenter: parent.verticalCenter
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    source: "qrc:/qml/icons/person-stalker.png"
                                }
                            }
                            /*Item{
                                    Layout.fillHeight: true
                                    Layout.fillWidth: true
                                    Image{
                                        height: parent.height * 0.8
                                        width: height
                                        anchors.verticalCenter: parent.verticalCenter
                                        anchors.horizontalCenter: parent.horizontalCenter
                                        source: "qrc:/qml/icons/android-clock.png"
                                    }
                                }*/
                            Item{
                                Layout.fillHeight: true
                                Layout.fillWidth: true
                                MouseArea{
                                    anchors.fill: parent
                                    onClicked: phoneStack.push(mediaPlayerComponent)
                                }

                                Image{
                                    height: parent.height * 0.8
                                    width: height
                                    anchors.verticalCenter: parent.verticalCenter
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    source: "qrc:/qml/icons/headphone.png"
                                }
                            }
                        }
                    }

                    Component {
                        id:contactsComponent
                        Contacts {
                            contactCardHeight:phoneStack.height/5
                            dialed_num: dialer.dialed_num
                            onDial: {
                                vcm.dial(number,"")
                            }
                        }
                    }

                    Component {
                        id:mediaPlayerComponent
                        MediaPlayer {
                            bluezManager: __root.bluezManager
                        }
                    }
                }
            }
        }
    }


    Connections{
        target: bluezManager
        onOperationalChanged:{
        }
    }
    Connections{
        target: PhoneBluetooth
        onPhonebookChanged:{
            /*console.log("Got phonebook");
            phoneStack.clear();
            phoneStack.push(contactsComponent);*/
        }
    }
    Connections{
        target: headunit
        onBtConnectionRequest:{
            for(var i = 0 ; i < bluezManager.devices.length; i++){
                if(bluezManager.devices[i].address === address){
                    if(!bluezManager.devices[i].connected)
                        bluezManager.devices[i].connectToDevice();
                } else {
                    bluezManager.devices[i].disconnectFromDevice();
                }
            }
        }
    }

    OfonoNetworkRegistration {
        modemPath: ofonomodem.modemPath
        id: netreg
    }

    OfonoManager {
        id: manager
        onAvailableChanged: {
            if(available){
                ofonomodem.modemPath = manager.modems[0]
            } else {
                console.log("[ofono] ofono manager is not available");
            }
        }
        onModemAdded: {
            ofonomodem.modemPath = path;
        }
        onModemRemoved: {
            if(ofonomodem.modemPath == path){
                ofonomodem.modemPath = ofonomodem.defaultModem()
            }
        }
    }
    OfonoModem{
        id:ofonomodem
        onModemPathChanged: {
            console.log("[ofono] ofono modem set to : " + ofonomodem.modemPath);
            online = true;
            powered = true;
        }
    }

    OfonoVoiceCallManager{
        id:vcm
        modemPath: ofonomodem.modemPath
        onCallAdded: {
            console.log(call)
            voice_call.voiceCallPath = call
        }
        onCallRemoved: {
            voice_call.voiceCallPath = call
        }
    }
    OfonoVoiceCall{
        id:voice_call
        onIncomingLineChanged: {
            console.log("onIncomingLineChanged");
        }
        onAnswerComplete: {
            console.log("onAnswerComplete");
        }
        onHangupComplete: {
            console.log("onHangupComplete");
        }
        onDeflectComplete: {
            console.log("onDeflectComplete");
        }
        onStateChanged: {
            console.log("onStateChanged : " + state);
        }
    }
    OfonoHandsfree{
        id:hands_free
        modemPath: ofonomodem.modemPath
    }

}
