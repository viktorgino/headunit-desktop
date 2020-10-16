import QtQuick 2.11
import QtGraphicalEffects 1.0


Item{
    id: __image_button

    property bool checked: false
    property bool checkable: false
    property bool changeColorOnPress: !checkable
    property string text: ""
    property url imageSource
    property color baseColor: "#ffffff"
    property color activeColor: "#ff5722"
    property color pressedColor: Qt.darker(color,1.5);
    property color color:baseColor
    signal clicked()


    Image {
        id:__image_button_image
        anchors.fill: parent
        source: parent.imageSource
        fillMode: Image.PreserveAspectFit
        mipmap:true
        visible: true
    }

    ColorOverlay {
        id: __image_button_color
        color: __image_button.color
        anchors.fill: __image_button_image
        enabled: true
        source: __image_button_image
    }
    Text {
        text: parent.text
        color: __image_button.baseColor
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
    }

    MouseArea{
        anchors.fill: parent
        onPressed: {
            if(changeColorOnPress)
                __image_button_color.color = parent.pressedColor;
        }
        onReleased: {
            if(checkable)
                parent.checked = !parent.checked;
            parent.clicked()
            if(changeColorOnPress)
                __image_button_color.color = parent.color;
        }
    }
    onCheckedChanged: {
        if(checked){
            color = activeColor;
        } else {
            color = baseColor;
        }
    }
}
