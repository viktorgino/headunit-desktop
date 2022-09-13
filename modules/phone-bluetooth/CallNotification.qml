import QtQuick 2.6
import QtQuick.Controls 2.0

import HUDTheme 1.0

ThemeRoot {
    id: __root

    state: pluginContext.Handsfree.callState

    Rectangle {
        id: item1
        height: parent.height * 0.2
        anchors.left: parent.left
        anchors.right: parent.right
        color: HUDStyle.colors.formBackground

        Rectangle {
            id: rectangle
            color: HUDStyle.colors.box
            anchors.fill: parent
            anchors.rightMargin: 8
            anchors.leftMargin: 8
            anchors.bottomMargin: 8
            anchors.topMargin: 8

            Image {
                id: image
                width: height
                anchors.top: parent.top
                anchors.topMargin: 8
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 8
                anchors.left: parent.left
                anchors.leftMargin: 8
                source: "qrc:/qml/icons/ios7-person.png"
                BorderImage {
                    anchors.fill: parent
                    border.bottom: 1
                    border.top: 1
                    border.right: 1
                    border.left: 1
                }
            }

            Item {
                id: textWrapper
                height: 200
                anchors.left: image.right
                anchors.right: callTime.left
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.topMargin: 0


                ThemeHeaderText {
                    id: text1
                    text: qsTr("Incoming call")
                    anchors.bottom: caller.top
                    level: 1
                }

                ThemeHeaderText {
                    id: caller
                    text: pluginContext.Handsfree.caller
                    anchors.verticalCenter: parent.verticalCenter
                    level: 3
                }

                ThemeHeaderText{
                    anchors.top: caller.bottom
                    level: 3
                }
            }

            ThemeText {
                id: callTime
                text: pluginContext.Handsfree.callTime
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: dial.left
                font.pixelSize: 12
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                width: height * 2
                height: parent.height / 2
            }

            Rectangle {
                id:dial
                width: height
                height: parent.height / 2
                color: "#558b2f"
                radius: height / 2
                anchors.right: hangup.left
                anchors.rightMargin: 8
                anchors.verticalCenter: parent.verticalCenter

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
                        pluginContext.Handsfree.answerCall()
                    }
                }
            }

            Rectangle {
                id:hangup
                width: height
                height: parent.height / 2
                color: "#c62828"
                radius: height / 2
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 8

                Image {
                    id: image2
                    anchors.bottomMargin: 0
                    anchors.rightMargin: parent.width*0.1
                    anchors.leftMargin: parent.width*0.1
                    anchors.topMargin: parent.height*0.15
                    rotation: 135
                    anchors.fill: parent
                    source: "qrc:/qml/icons/svg/android-call.svg"
                    fillMode: Image.PreserveAspectFit
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        pluginContext.Handsfree.hangupCall()
                    }
                }
            }
        }
    }

    states: [
        State {
            name: "active"

            PropertyChanges {
                target: dial
                width: 1
                visible: false
            }

            PropertyChanges {
                target: text1
                visible: false
            }
        },
        State {
            name: "alerting"

            PropertyChanges {
                target: dial
                width: 1
                visible: false
            }

            PropertyChanges {
                target: text1
                text: qsTr("Calling")
            }

            PropertyChanges {
                target: callTime
                width: 1
                visible: false
            }
        },
        State {
            name: "incoming"

            PropertyChanges {
                target: callTime
                width: 1
                visible: false
            }
        }
    ]

}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
