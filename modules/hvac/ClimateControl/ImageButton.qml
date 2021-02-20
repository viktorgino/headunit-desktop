import QtQuick 2.5
import QtGraphicalEffects 1.0


Image {
    id: __root

    property url imageSource
    property color baseColor: "#aaaaaaaa"
    property color activeColor: "#cc00ff22"
    property color pressedColor: "#ccffffff"
    property bool checked: false
    signal clicked()

    ColorOverlay {
        id: overlay
        color: baseColor
        anchors.fill: __root
        enabled: true
        source: __root
    }

    MouseArea{
        anchors.fill: parent
        onPressed: {
            overlay.color = __root.pressedColor;
        }
        onReleased: {
            //parent.checked = !parent.checked;
            parent.clicked()
            overlay.color = __root.checked?activeColor:baseColor;
        }
    }
    onCheckedChanged: {
        overlay.color = __root.checked?activeColor:baseColor;
    }
}



/*##^## Designer {
D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
