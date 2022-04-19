import QtQuick 2.6
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

import HUDTheme 1.0

Item {
    id: __root

    property alias dialed_num: dialer_num.text

    TextField {
        id: dialer_num
        text: ""
        anchors.leftMargin: 2
        font.pointSize: 20
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        color: "#ffffff"
//        placeholderTextColor : "#cecece"
        validator: RegExpValidator { regExp :/[+]?[0-9#*]*/}

        background: Rectangle {
            height: 2
            border.color: "#ffffff"
            width: dialer_num.width
            y: dialer_num.height - 2
        }
        ImageButton {
            id: image
            width: height * 1.2
//            fillMode: Image.PreserveAspectFit
            anchors.right: parent.right
            anchors.rightMargin: 0
            anchors.top: parent.top
            anchors.topMargin: 0
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            imageSource: "qrc:/qml/icons/svg/backspace-outline.svg"
            onClicked: {
                dialer_num.remove(dialer_num.cursorPosition-1, dialer_num.cursorPosition);
            }
        }
    }

    GridLayout {
        id: gridLayout
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.topMargin: 8
        rows: 4
        columns: 3
        anchors.left: parent.left
        anchors.bottom: dial_buttons.top
        anchors.top: dialer_num.bottom
        Repeater{
            id: dial_nums
            model:ListModel {
                ListElement {
                    name: "1"
                }

                ListElement {
                    name: "2"
                }

                ListElement {
                    name: "3"
                }

                ListElement {
                    name: "4"
                }

                ListElement {
                    name: "5"
                }

                ListElement {
                    name: "6"
                }

                ListElement {
                    name: "7"
                }

                ListElement {
                    name: "8"
                }
                ListElement {
                    name: "9"
                }
                ListElement {
                    name: "*"
                }
                ListElement {
                    name: "0"
                }
                ListElement {
                    name: "#"
                }
            }
            delegate: Item {
                Layout.fillHeight: true
                Layout.fillWidth: true
                Text{
                    text:model.name
                    font.pointSize: height > 0 ? height * 0.6 : 12
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    anchors.fill: parent
                    color:HUDStyle.colors.text

                    MouseArea {
                        id: mouseArea
                        anchors.fill: parent
                        onClicked: dialer_num.insert(dialer_num.cursorPosition,parent.text)
                    }
                }
            }
        }
    }

    Item {
        id: dial_buttons
        height: parent.height * 0.2
        anchors.rightMargin: 0
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.left: parent.left

        Rectangle {
            id:dial
            width: height
            color: "#558b2f"
            radius: height/2
            anchors.top: parent.top
            anchors.topMargin: 8
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottomMargin: 8

            Image {
                id: image1
                anchors.rightMargin: parent.width*0.1
                anchors.leftMargin: parent.width*0.1
                anchors.bottomMargin: parent.height*0.1
                anchors.topMargin: parent.height*0.1
                fillMode: Image.PreserveAspectFit
                anchors.fill: parent
                source: "qrc:/qml/icons/svg/android-call.svg"
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    pluginContext.Handsfree.dialNumber(dialer_num.text);
                    dialer_num.text = "";
                }
            }
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;formeditorColor:"#808080";height:480;width:640}
}
##^##*/
