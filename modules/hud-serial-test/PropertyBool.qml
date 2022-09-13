import QtQuick 2.0
import QtQuick.Controls 2.2
import HUDTheme 1.0

Item {
    id: __root
    property var label
    property alias propertyLabel: text1.text
    property bool editable : false
    signal switchClicked(var value)
    height: 17

    ThemeFormText {
        id: text2
        text: __root.label
        height: 17
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: 8
        visible:  !__root.editable
    }

    TextField {
        id:text1
        placeholderText: __root.label
        visible:  __root.editable
        width: 150
        height: 17
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        padding: 0
        anchors.leftMargin: 8
    }

    Switch {
        id: control
        height: 17
        //        checked: __root.propertyEnabled
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        autoExclusive: false
        anchors.rightMargin: 8
        padding: 0
        indicator: Rectangle {
            implicitWidth: 34
            implicitHeight: 17
            x: control.leftPadding
            y: parent.height / 2 - height / 2
            radius: 13
            color: control.checked ? "#17a81a" : "#ffffff"
            border.color: control.checked ? "#17a81a" : "#cccccc"

            Rectangle {
                x: control.checked ? parent.width - width : 0
                width: 17
                height: 17
                radius: 10
                color: control.down ? "#cccccc" : "#ffffff"
                border.color: control.checked ? (control.down ? "#17a81a" : "#21be2b") : "#999999"
            }
        }
    }
    MouseArea {
        height: 17
        anchors.fill: control
        preventStealing: true
        onClicked: {
            control.checked = !control.checked
            __root.switchClicked(control.checked)
        }
    }


}

/*##^##
Designer {
    D{i:0;formeditorColor:"#c0c0c0"}
}
##^##*/
