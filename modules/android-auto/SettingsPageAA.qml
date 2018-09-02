import QtQuick 2.6
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1
import Qt.labs.settings 1.0

Item {
    id: root

    Settings {
        id: settings
        category: "AndroidAuto"
        property int can_play_native_media_during_vr:_can_play_native_media_during_vr.checked & 1
        property int hide_clock:_hide_clock.checked & 1
        property int available_while_in_call:_available_while_in_call.checked & 1
        property int wifi_direct:_wifi_direct.checked & 1
        property alias ts_width:_ts_width.text
        property alias ts_height:_ts_height.text
        property alias margin_width:_margin_width.text
        property alias margin_height:_margin_height.text
        property alias dpi:_dpi.text
        property string resolution:resolution_items.get(_resolution.currentIndex).value
        property string frame_rate:frame_rate_items.get(_frame_rate.currentIndex).value
        property string transport_type:transport_type_items.get(_transport_type.currentIndex).value
        property alias resolution_index:_resolution.currentIndex
        property alias frame_rate_index:_frame_rate.currentIndex
        property alias transport_type_index:_transport_type.currentIndex
        property alias network_address:_network_address.text
    }

    Component {
        id: highlight
        Rectangle {
            height: 40
            anchors.right: parent.right
            anchors.left: parent.left
            color: "lightsteelblue"
            y: mediaLocations.currentItem.y
            Behavior on y {
                SpringAnimation {
                    spring: 3
                    damping: 0.2
                }
            }
        }
    }

    Rectangle {
        id: rectangle
        anchors.left: parent.left
        anchors.leftMargin: 8
        anchors.right: parent.right
        anchors.rightMargin: 8
        color: "#eceff1"
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 8
        anchors.top: parent.top
        anchors.topMargin: 8
        border.width: 1
        border.color: "#000000"





        Rectangle {
            id: divider
            height: 1
            color: "#000000"
            anchors.bottom: buttons.top
            anchors.bottomMargin: 0
            anchors.right: parent.right
            anchors.rightMargin: 0
            anchors.left: parent.left
            anchors.leftMargin: 0
        }

        Item {
            id: buttons
            height: 40
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.right: parent.right
            anchors.rightMargin: 0

            Button {
                id: button
                width: 150
                height: 24
                text: qsTr("Restart Android Auto")
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 8
                onClicked: AndroidAuto.restartHU();
            }
        }

        Flickable {
            id: flickable
            anchors.rightMargin: 8
            anchors.leftMargin: 8
            anchors.topMargin: 8
            anchors.bottom: divider.top
            anchors.right: parent.right
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottomMargin: 8
            clip: true

            contentWidth: column.width;
            contentHeight: column.height
            ColumnLayout {
                id: column
                width: flickable.width

                Item {
                    id: item4
                    height: 60
                    Layout.fillWidth: true
                    anchors.left: parent.left
                    anchors.leftMargin: 0
                    anchors.right: parent.right
                    anchors.rightMargin: 0

                    Text {
                        text: qsTr("Play native media during vr")
                        anchors.left: parent.left
                        anchors.leftMargin: 8
                        anchors.verticalCenter: parent.verticalCenter
                        font.pixelSize: 12
                    }

                    CheckBox {
                        id: _can_play_native_media_during_vr
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.right: parent.right
                        anchors.rightMargin: 8
                        checked: settings.can_play_native_media_during_vr
                    }
                }

                Item {
                    id: item5
                    height: 60
                    anchors.left: parent.left
                    anchors.leftMargin: 0
                    anchors.right: parent.right
                    anchors.rightMargin: 0

                    Text {
                        text: qsTr("Hide clock")
                        anchors.left: parent.left
                        anchors.leftMargin: 8
                        anchors.verticalCenter: parent.verticalCenter
                        font.pixelSize: 12
                    }

                    CheckBox {
                        id: _hide_clock
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.right: parent.right
                        anchors.rightMargin: 8
                        checked: settings.hide_clock
                    }
                }

                Item {
                    id: item2
                    height: 60
                    anchors.left: parent.left
                    anchors.leftMargin: 0
                    anchors.right: parent.right
                    anchors.rightMargin: 0

                    Text {
                        text: qsTr("Available while in call")
                        anchors.left: parent.left
                        anchors.leftMargin: 8
                        anchors.verticalCenter: parent.verticalCenter
                        font.pixelSize: 12
                    }

                    CheckBox {
                        id: _available_while_in_call
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.right: parent.right
                        anchors.rightMargin: 8
                        checked: settings.available_while_in_call
                    }
                }

                Item {
                    id: item1
                    height: 60
                    anchors.left: parent.left
                    anchors.leftMargin: 0
                    anchors.right: parent.right
                    anchors.rightMargin: 0

                    Text {
                        text: qsTr("Touchscreen width")
                        anchors.left: parent.left
                        anchors.leftMargin: 8
                        anchors.verticalCenter: parent.verticalCenter
                        font.pixelSize: 12
                    }


                    TextField {
                        id: _ts_width
                        width: 50
                        text: "800"
                        validator: IntValidator{}
                        maximumLength:4
                        anchors.right: ts_width_label.left
                        anchors.rightMargin: 0
                        anchors.verticalCenter: parent.verticalCenter
                    }

                    Text {
                        id: ts_width_label
                        width: 30
                        text: qsTr("pixels")
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.right: parent.right
                        anchors.rightMargin: 8
                        font.pixelSize: 12
                    }
                }

                Item {
                    height: 60
                    anchors.left: parent.left
                    anchors.leftMargin: 0
                    anchors.right: parent.right
                    anchors.rightMargin: 0

                    Text {
                        text: qsTr("Touchscreen height")
                        anchors.left: parent.left
                        anchors.leftMargin: 8
                        anchors.verticalCenter: parent.verticalCenter
                        font.pixelSize: 12
                    }


                    TextField {
                        id: _ts_height
                        width: 50
                        text: "480"
                        validator: IntValidator{}
                        maximumLength:4
                        anchors.right: ts_height_label.left
                        anchors.rightMargin: 0
                        anchors.verticalCenter: parent.verticalCenter
                    }

                    Text {
                        id: ts_height_label
                        x: 562
                        y: 22
                        width: 30
                        text: qsTr("pixels")
                        font.pixelSize: 12
                        anchors.rightMargin: 8
                        anchors.right: parent.right
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }

                Item {
                    height: 60
                    anchors.left: parent.left
                    anchors.leftMargin: 0
                    anchors.right: parent.right
                    anchors.rightMargin: 0

                    Text {
                        text: qsTr("Margin width")
                        anchors.left: parent.left
                        anchors.leftMargin: 8
                        anchors.verticalCenter: parent.verticalCenter
                        font.pixelSize: 12
                    }


                    TextField {
                        id: _margin_width
                        width: 50
                        text: "0"
                        validator: IntValidator{}
                        maximumLength:4
                        anchors.right: margin_width_label.left
                        anchors.rightMargin: 0
                        anchors.verticalCenter: parent.verticalCenter
                    }

                    Text {
                        id: margin_width_label
                        x: 562
                        y: 22
                        width: 30
                        text: qsTr("pixels")
                        font.pixelSize: 12
                        anchors.rightMargin: 8
                        anchors.right: parent.right
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }

                Item {
                    height: 60
                    anchors.left: parent.left
                    anchors.leftMargin: 0
                    anchors.right: parent.right
                    anchors.rightMargin: 0

                    Text {
                        text: qsTr("Margin height")
                        anchors.left: parent.left
                        anchors.leftMargin: 8
                        anchors.verticalCenter: parent.verticalCenter
                        font.pixelSize: 12
                    }


                    TextField {
                        id: _margin_height
                        width: 50
                        text: "0"
                        validator: IntValidator{}
                        maximumLength:4
                        anchors.right: margin_height_label.left
                        anchors.rightMargin: 0
                        anchors.verticalCenter: parent.verticalCenter
                    }

                    Text {
                        id: margin_height_label
                        x: 562
                        y: 22
                        width: 30
                        text: qsTr("pixels")
                        font.pixelSize: 12
                        anchors.rightMargin: 8
                        anchors.right: parent.right
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }

                Item {
                    height: 60
                    anchors.left: parent.left
                    anchors.leftMargin: 0
                    anchors.right: parent.right
                    anchors.rightMargin: 0

                    Text {
                        text: qsTr("Pixel density")
                        anchors.left: parent.left
                        anchors.leftMargin: 8
                        anchors.verticalCenter: parent.verticalCenter
                        font.pixelSize: 12
                    }


                    TextField {
                        id: _dpi
                        width: 50
                        text: "140"
                        validator: IntValidator{}
                        maximumLength:4
                        anchors.right: dpi_label.left
                        anchors.rightMargin: 0
                        anchors.verticalCenter: parent.verticalCenter
                    }

                    Text {
                        id: dpi_label
                        x: 562
                        y: 22
                        width: 30
                        text: qsTr("DPI")
                        font.pixelSize: 12
                        anchors.rightMargin: 8
                        anchors.right: parent.right
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }

                Item {
                    height: 60
                    anchors.left: parent.left
                    anchors.leftMargin: 0
                    anchors.right: parent.right
                    anchors.rightMargin: 0

                    Text {
                        text: qsTr("Resolution")
                        anchors.left: parent.left
                        anchors.leftMargin: 8
                        anchors.verticalCenter: parent.verticalCenter
                        font.pixelSize: 12
                    }

                    ComboBox {
                        id: _resolution
                        anchors.right: parent.right
                        anchors.rightMargin: 8
                        anchors.verticalCenter: parent.verticalCenter
                        model: ListModel {
                            id: resolution_items
                            ListElement { text: "800x480"; value: "1" }
                            ListElement { text: "1280x720"; value: "2" }
                            ListElement { text: "1920x1080"; value: "3" }
                        }
                    }
                }

                Item {
                    height: 60
                    anchors.left: parent.left
                    anchors.leftMargin: 0
                    anchors.right: parent.right
                    anchors.rightMargin: 0

                    Text {
                        text: qsTr("Frame rate")
                        anchors.left: parent.left
                        anchors.leftMargin: 8
                        anchors.verticalCenter: parent.verticalCenter
                        font.pixelSize: 12
                    }

                    ComboBox {
                        id: _frame_rate
                        anchors.right: parent.right
                        anchors.rightMargin: 8
                        anchors.verticalCenter: parent.verticalCenter
                        model: ListModel {
                            id: frame_rate_items
                            ListElement { text: "30 FPS"; value: "1" }
                            ListElement { text: "60 FPS"; value: "2" }
                        }
                    }
                }

                Item {
                    height: 60
                    anchors.left: parent.left
                    anchors.leftMargin: 0
                    anchors.right: parent.right
                    anchors.rightMargin: 0

                    Text {
                        text: qsTr("Transport type")
                        anchors.left: parent.left
                        anchors.leftMargin: 8
                        anchors.verticalCenter: parent.verticalCenter
                        font.pixelSize: 12
                    }

                    ComboBox {
                        id: _transport_type
                        anchors.right: parent.right
                        anchors.rightMargin: 8
                        anchors.verticalCenter: parent.verticalCenter
                        model: ListModel {
                            id: transport_type_items
                            ListElement { text: "usb"; value: "usb" }
                            ListElement { text: "network"; value: "network" }
                        }
                    }
                }

                Item {
                    height: 60
                    visible: _transport_type.currentText == "network"
                    anchors.left: parent.left
                    anchors.leftMargin: 0
                    anchors.right: parent.right
                    anchors.rightMargin: 0

                    Text {
                        text: qsTr("Network address")
                        anchors.left: parent.left
                        anchors.leftMargin: 8
                        anchors.verticalCenter: parent.verticalCenter
                        font.pixelSize: 12
                    }


                    TextField {
                        id: _network_address
                        width: 110
                        text: "127.0.0.1"
                        inputMask: "000.000.000.000;_"
                        anchors.right: parent.right
                        anchors.rightMargin: 8
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }

                Item {
                    height: 60
                    visible: _transport_type.currentText == "network"
                    anchors.left: parent.left
                    anchors.leftMargin: 0
                    anchors.right: parent.right
                    anchors.rightMargin: 0

                    Text {
                        text: qsTr("WiFi direct")
                        anchors.left: parent.left
                        anchors.leftMargin: 8
                        anchors.verticalCenter: parent.verticalCenter
                        font.pixelSize: 12
                    }

                    CheckBox {
                        id: _wifi_direct
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.right: parent.right
                        anchors.rightMargin: 8
                        checked: settings.wifi_direct
                    }
                }
            }

        }
    }


}
