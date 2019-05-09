import QtQuick 2.6
import QtQuick.Controls 2.0
import QtQuick.Controls.Styles 1.4

Item {
    id: __root

    property string caller: "caller"
    property string callPath
    function hide(){
        __root.state = ""
    }
    function show(){
        __root.state = "open"
    }

    Item {
        id: item1
        y: -height
        height: parent.height * 0.2
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0

        Rectangle {
            id: rectangle
            color: "#eeeeee"
            anchors.fill: parent


            Rectangle {
                id:dial
                width: height * 2
                height: parent.height / 2
                color: "#558b2f"
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
                        __root.hide();
                        PhoneBluetooth.answerCall(callNotification.callPath)
                    }
                }
            }

            Rectangle {
                id:hangup
                width: height * 2
                height: parent.height / 2
                color: "#c62828"
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
                        __root.hide();
                        PhoneBluetooth.declineCall(callNotification.callPath)
                    }
                }
            }

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

            Text {
                id: text1
                y: parent.height/2 - height
                text: qsTr("Incoming call")
                anchors.left: image.right
                anchors.leftMargin: 8
                font.pixelSize: 22
            }

            Text {
                id: text2
                y: parent.height/2
                text: __root.caller
                anchors.left: image.right
                anchors.leftMargin: 8
                font.pixelSize: 22
            }
        }
    }
    transitions:
        Transition {
            NumberAnimation { properties: "y"; duration: 250}
    }
    states: [
        State {
            name: "open"

            PropertyChanges {
                target: item1
                y: 0
            }
        }
    ]
    Connections {
        target: PhoneBluetooth
        onIncomingCall:{
            __root.caller = caller_number
            __root.callPath = call_path
            __root.show();
        }
    }

}
