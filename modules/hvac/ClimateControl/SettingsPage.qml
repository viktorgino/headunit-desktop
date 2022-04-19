import QtQuick 2.9
import HUDTheme 1.0
Item {
    id:__root
    SettingsPageItemCombobox {
        id: car_make
        name: ""
        anchors.left: parent.left
        anchors.right: parent.right
        label: "Car Make"
        values: pluginContext.carManufacturers
        onValueChanged: {
            pluginSettings.car_make = value
        }
        value : pluginSettings.car_make
    }

    SettingsPageItemCombobox {
        id: car
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: car_make.bottom
        label: "Car"
        values: pluginContext.cars

        onValueChanged: {
            pluginSettings.car = value
        }
        value : pluginSettings.car
    }

    ListView {
        id: listView
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: car.bottom
        anchors.bottom: parent.bottom
        model: pluginContext.hvacSettings.CarSettings
        delegate: Loader{
            width: __root.width
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
    Connections{
        target: HVACPlugin
        onSettingsChanged : {
            listView.model = pluginContext.hvacSettings.CarSettings
        }
    }
}
