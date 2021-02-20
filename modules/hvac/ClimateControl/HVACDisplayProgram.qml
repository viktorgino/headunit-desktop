import QtQuick 2.11

Item {
    id:__root
    property bool autoClimate : true
    property bool windscreenDefrost : true
    Item {
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter

        width: height
        height: parent.height * 0.5

        Text {
            id:autoClimateText
            anchors.fill: parent
            visible: __root.autoClimate & !__root.windscreenDefrost
            color: "#ffffff"
            text: qsTr("Auto\nClimate")
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            wrapMode: Text.WordWrap
            font.weight: Font.ExtraBold
            font.capitalization: Font.AllUppercase
            minimumPixelSize: 10
            font.pointSize: 80
            fontSizeMode: Text.Fit
            font.bold: true
        }
        Image {
            id:windscreenDefrostImage
            visible: __root.windscreenDefrost
            anchors.fill: parent
            source: "qrc:/HVAC/icons/windshield-defrost.svg"
            fillMode: Image.PreserveAspectFit
            mipmap: true
        }
    }
}
