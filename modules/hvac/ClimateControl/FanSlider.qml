import QtQuick 2.6
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.0

Item{
    id:__root
    property int min: 0
    property int max: 6
    property bool slider_enabled: true
    property alias value: fanslider.value
    Layout.fillHeight: true
    Layout.fillWidth: true
    RowLayout {
        id:bg_wrapper
        anchors.fill: parent
        implicitHeight: 80
        implicitWidth: 200
        Item {
            id: left_icon
            Layout.maximumWidth: height
            Layout.fillHeight: true
            Layout.fillWidth: true

            Image {
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                source: "qrc:/HVAC/icons/ac.svg"
                width: parent.width * 0.6
                height: width
                mipmap:true
            }
        }

        Repeater{
            model: fanslider.maximumValue - fanslider.minimumValue + 1
            Item {
                Layout.fillHeight: true
                Layout.fillWidth: true
                Rectangle {
                    width: 15
                    height: 15
                    color: "#ffffff"
                    radius: width
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
        }

        Item {
            id: right_icon
            Layout.maximumWidth: height
            Layout.fillHeight: true
            Layout.fillWidth: true

            Image {
                width: parent.width
                height: width
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                source: "qrc:/HVAC/icons/ac.svg"
                //visible: (fanslider.value != fanslider.maximumValue)
                mipmap:true
            }
        }
    }

    Slider {
        id:fanslider
        width: (parent.width/bg_wrapper.children.length) * (bg_wrapper.children.length - 2)
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.top: parent.top
        value: 6
        anchors.bottomMargin: 0
        anchors.topMargin: 0
        orientation: Qt.Horizontal
        minimumValue: __root.min
        maximumValue: __root.max
        stepSize: 1
        enabled: slider_enabled
        style: SliderStyle {
            handle: Rectangle {
                property int size: fanslider.width /(fanslider.maximumValue - fanslider.minimumValue + 1)
                width: height
                height: fanslider.height<size?fanslider.height:size
                radius: height/2
                color: "#4cffffff"

                Rectangle {
                    color: "#ffffff"
                    anchors.rightMargin: parent.width * 0.1
                    anchors.leftMargin: parent.width * 0.1
                    anchors.bottomMargin: parent.height * 0.1
                    anchors.topMargin: parent.height * 0.1
                    anchors.fill: parent
                    radius: (width+height)/4

                    Text {
                        text: qsTr(fanslider.value.toString())
                        font.pointSize: height * 0.4
                        font.family: "Arial"
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        anchors.fill: parent
                    }
                }
            }
            groove:Item{}
        }
    }

}


/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
