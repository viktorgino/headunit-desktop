import QtQuick 2.9
import QtQuick.Controls 2.2
import HUDTheme 1.0
import HUDSettingsPage 1.0
import QtQuick.Layouts 1.3

ThemeRoot {
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

            SettingsPageItemCombobox {
                id: car_make
                width: parent.width
                label: "Car Make"
                values: pluginContext.carManufacturers
                onValueChanged: {
                    pluginSettings.car_make = value
                }
                value : pluginSettings.car_make
            }

            SettingsPageItemCombobox {
                id: car
                width: parent.width
                label: "Car"
                values: pluginContext.cars

                onValueChanged: {
                    pluginSettings.car = value
                }
                value : pluginSettings.car
            }


            SettingsPageItemCombobox {
                id: serial_speed
                width: parent.width
                label: "Speed"
                values: pluginContext.speeds
                onValueChanged: {
                    pluginSettings.serial_speed = value
                }
                value : pluginSettings.serial_speed
            }

            SettingsPageItemCombobox {
                id: serial_port
                width: parent.width
                label: "Serial Port"
                values: pluginContext.ports
                onValueChanged: {
                    pluginSettings.serial_port = value
                }
                value : pluginSettings.serial_port
            }
            SettingsPageItemHeader{
                width: parent.width
            }
            
            Repeater {
                id: listView
                width: parent.width
                model: pluginContext.carSettings
                delegate: Loader {
                    width: parent.width
                    Component.onCompleted: {
                        var qmlItem;
                        if(modelData.type === "bit") {
                            qmlItem = "qrc:/qml/HUDSettingsPage/SettingsPageItemCheckbox.qml";
                        } else if(modelData.type === "byte") {
                            if(modelData.itemType === "slider") {
                                qmlItem = "qrc:/qml/HUDSettingsPage/SettingsPageItemSlider.qml";
                            } else if(modelData.itemType === "tumbler") {
                                qmlItem = "qrc:/qml/HUDSettingsPage/SettingsPageItemTumbler.qml";
                            } else if(modelData.itemType === "combobox") {
                                qmlItem = "qrc:/qml/HUDSettingsPage/SettingsPageItemCombobox.qml";
                            }
                        }
                        setSource(qmlItem, modelData);
                        if(item){
                            if(modelData.type === "bit") {
                                item.value = pluginContext.customBits[modelData.bit]
                            } else if(modelData.type === "byte") {
                                item.value = pluginContext.customBytes[modelData.byte]
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
                                pluginContext.setCustomBit(modelData.bit, item.value);
                            } else if(modelData.type === "byte") {
                                pluginContext.setCustomByte(modelData.byte, item.value);
                            }
                        }
                    }
                }
            }
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
