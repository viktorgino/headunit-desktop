import QtQuick 2.9
import QtQuick.Controls 2.2
import HUDTheme 1.0

//{
//    "label":"Restart Serial",
//    "name": "action",
//    "type":"action",
//    "action" : "HUDSerialPlugin.serialRestart()"
//}
import QtQuick.Layouts 1.3
Item {
    id:__root

    Flickable {
        anchors.fill: parent
        contentHeight: column.height
        flickableDirection: Flickable.VerticalFlick
        ScrollBar.vertical: ThemeScrollBar { }
        Column {
            id: column
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: 0

            Repeater {
                id: listView
                width: parent.width
                model: HUDSerialPlugin.carSettings
                delegate: Loader {
                    width: parent.width
                    Component.onCompleted: {
                        var qmlItem;
                        if(modelData.type === "bit") {
                            qmlItem = "qrc:/qml/SettingsPage/SettingsPageItemCheckbox.qml";
                        } else if(modelData.type === "byte") {
                            if(modelData.itemType === "slider") {
                                qmlItem = "qrc:/qml/SettingsPage/SettingsPageItemSlider.qml";
                            } else if(modelData.itemType === "tumbler") {
                                qmlItem = "qrc:/qml/SettingsPage/SettingsPageItemTumbler.qml";
                            } else if(modelData.itemType === "combobox") {
                                qmlItem = "qrc:/qml/SettingsPage/SettingsPageItemCombobox.qml";
                            }
                        }
                        setSource(qmlItem, modelData);
                        if(item){
                            if(modelData.type === "bit") {
                                item.value = HUDSerialPlugin.customBits[modelData.bit]
                            } else if(modelData.type === "byte") {
                                item.value = HUDSerialPlugin.customBytes[modelData.byte]
                            }
                            valueUpdateConnection.enabled = true;
                        }
                    }
                    Connections {
                        id:valueUpdateConnection
                        enabled: false
                        target: item
                        onValueChanged : {
                            if(modelData.type === "bit") {
                                HUDSerialPlugin.setCustomBit(modelData.bit, item.value);
                            } else if(modelData.type === "byte") {
                                HUDSerialPlugin.setCustomByte(modelData.byte, item.value);
                            }
                        }
                    }
                }
            }

            SettingsPageItemCombobox {
                id: car_make
                width: parent.width
                label: "Car Make"
                values: HUDSerialPlugin.carManufacturers
                onValueChanged: {
                    HUDSettings["HUDSerialPlugin"].car_make = value
                }
                value : HUDSettings["HUDSerialPlugin"].car_make
            }

            SettingsPageItemCombobox {
                id: car
                width: parent.width
                label: "Car"
                values: HUDSerialPlugin.cars

                onValueChanged: {
                    HUDSettings["HUDSerialPlugin"].car = value
                }
                value : HUDSettings["HUDSerialPlugin"].car
            }


            SettingsPageItemCombobox {
                id: serial_speed
                width: parent.width
                label: "Speed"
                values: ["9600","19200","38400","57600","115200","230400","460800","500000","921600", "1000000", "1152000", "2000000", "4608000"]
                onValueChanged: {
                    HUDSettings["HUDSerialPlugin"].serial_speed = value
                }
                value : HUDSettings["HUDSerialPlugin"].serial_speed
            }

            SettingsPageItemCombobox {
                id: serial_port
                width: parent.width
                label: "Serial Port"
                values: HUDSerialPlugin.ports
                onValueChanged: {
                    HUDSettings["HUDSerialPlugin"].serial_port = value
                }
                value : HUDSettings["HUDSerialPlugin"].serial_port
            }

        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
