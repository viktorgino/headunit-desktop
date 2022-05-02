import QtQuick 2.0
import QtQuick.Controls 2.2
import HUDTheme 1.0

Item {
    id: __root
    height: 17
    property alias label: text1.placeholderText
    property alias propertyLabel: text1.text
    property bool editable : false
    property int minValue : 0
    property var maxValue : 255
    signal valueChanged(var value)

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
        visible:  __root.editable
        width: 150
        height: 17
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        padding: 0
        anchors.leftMargin: 8
    }

    Button {
        id: minusButton
        width: 20
        text: qsTr("-")
        anchors.right: numWrapper.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        onClicked: {
            numValue.decreaseValue()
        }
    }

    Button {
        id: plusButton
        width: 20
        text: qsTr("+")
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.rightMargin: 8
        onClicked: {
            numValue.increaseValue()
        }
    }

    Rectangle{
        id: numWrapper
        width: 30
        border.color: "#a6a6a6"
        border.width: 1
        anchors.right: plusButton.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom

        Text {
            id: numValue
            text : "0"
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            function increaseValue () {
                var currentValue = parseInt(numValue.text);
                var newValue;
                if(currentValue >= __root.maxValue){
                    newValue = __root.minValue;
                } else {
                    newValue = currentValue + 1;
                }
                __root.valueChanged(newValue);
                text = newValue
            }
            function decreaseValue () {
                var currentValue = parseInt(numValue.text);
                var newValue;
                if(currentValue <= __root.minValue){
                    newValue = __root.maxValue;
                } else {
                    newValue = currentValue - 1;
                }
                __root.valueChanged(newValue);
                text = newValue
            }
        }

        MouseArea {
            anchors.fill: parent
            onWheel: {
                if(wheel.angleDelta.y>0)
                    numValue.increaseValue()
                if(wheel.angleDelta.y<0)
                    numValue.decreaseValue()
            }
        }
    }
}

/*##^##
Designer {
    D{i:0;formeditorColor:"#c0c0c0"}
}
##^##*/
