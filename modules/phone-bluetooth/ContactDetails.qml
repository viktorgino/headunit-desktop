import QtQuick 2.0
import QtGraphicalEffects 1.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

import HUDTheme 1.0

Item {
    id: __root

    property var name : "Contact Name"
    property var numbers : [{"number":"+1-234-567", "type": "CELL"}]
    property var image : ""

    property var nameArray : name.split(' ')

    Rectangle {
        id: contactDetails
        color:HUDStyle.colors.box
        clip: true
        height: parent.height*0.3
        anchors.top: parent.top
        anchors.topMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        ThemeText {
            x: -height/4
            opacity: 0.3
            color: HUDStyle.colors.text
            text: nameArray.length > 0 ? nameArray[0].charAt(0).toUpperCase() : ""
            font.pointSize: parent.height * 1.5
            anchors.bottomMargin: 0
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.topMargin: 0
            font.bold: true
            clip: true
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignLeft
        }

        ThemeText {
            x: height/2
            opacity: 0.4
            color: HUDStyle.colors.text
            text: nameArray.length > 1 ? nameArray[nameArray.length-1].charAt(0).toUpperCase() : ""
            font.pointSize: parent.height * 1.5
            anchors.bottom: parent.bottom
            anchors.top: parent.top
            font.bold: true
            clip: true
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignRight
        }
        Image {
            id: image1
            anchors.fill: parent
            fillMode: Image.PreserveAspectCrop

            Component.onCompleted: {
                if(image.size > 0 ){
                    image1.source = 'data:image/jpeg;'+image
                }
            }
        }

        ThemeHeaderText {
            id: text1
            color: "#ffffff"
            text: name
            elide: Text.ElideLeft
            font.pixelSize: parent.height * 0.1
            anchors.leftMargin: parent.width * 0.05
            anchors.bottomMargin: parent.height * 0.1
            anchors.left: parent.left
            anchors.bottom: parent.bottom
        }
    }

    ListView {
        clip: true
        anchors.top: contactDetails.bottom
        anchors.topMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0
        model: numbers
        delegate: Item{
            id: item1
            width: parent.width
            height: 40
            Image {
                id: image2
                width: height
                anchors.leftMargin: 8
                anchors.bottomMargin: 8
                anchors.topMargin: 8
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.top: parent.top
                source: "qrc:/qml/icons/svg/android-call.svg"
                layer.mipmap: true
            }

            ColorOverlay {
                color: HUDStyle.colors.icon
                anchors.fill: image2
                source: image2
            }

            ThemeText {
                id: numberText
                text: modelData.number
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.left: image2.right
                verticalAlignment: Text.AlignVCenter
                anchors.verticalCenterOffset: -height / 2
                anchors.leftMargin: 8
                horizontalAlignment: Text.AlignLeft
            }

            ThemeText {
                id: type
                text: {
                    switch(modelData.type){
                    case "CELL" :
                        return qsTr("Mobile")
                    case "VOICE":
                        return qsTr("Landline")
                    case "FAX" :
                        return qsTr("Fax")
                    case "WORK" :
                        return qsTr("Work")
                    case "HOME" :
                        return qsTr("Home")
                    default:
                        return qsTr("Other") + modelData.type
                    }
                }
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: image2.right
                anchors.right: parent.right
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
                anchors.leftMargin: 8
                subtext: true
                anchors.verticalCenterOffset: height / 2
            }

            MouseArea {
                id: mouseArea
                anchors.fill: parent
                onClicked: {
                    pluginContext.Handsfree.dialNumber(modelData.number)
                }
            }
        }
    }

    Button {
        id: button
        text: qsTr("Back")
        anchors.top: parent.top
        anchors.topMargin: 8
        anchors.left: parent.left
        anchors.leftMargin: 8
        onClicked: {
            contactView.pop()
        }
    }

}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
