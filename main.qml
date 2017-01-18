import QtQuick 2.5
import QtQuick.Window 2.2
import QtQuick 2.5

Window {
    visible: true
    width: 800
    height: 480
    title: qsTr("Hello World")

    MainForm {
        anchors.fill: parent
    }

    Keys.onPressed: {
        switch(event.key){
        case Qt.Key_Left:
            console.log("Key Left");
            break;
        }
    }
}
