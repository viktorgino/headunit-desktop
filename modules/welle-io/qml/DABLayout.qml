import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.0
import QtMultimedia 5.7
import Qt.labs.settings 1.0
import QtGraphicalEffects 1.0
import QtQml 2.3

import HUDTheme 1.0

ThemeRoot {
    id: __root

    clip: true

    Item {
        id: main
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.left: stationDrawer.right

        Item {
            id: wrapper
            anchors.fill: parent
            anchors.leftMargin: parent.width * 0.1
            anchors.rightMargin: parent.width * 0.1
            anchors.bottomMargin: parent.height * 0.1
            anchors.topMargin: parent.height * 0.1
            Rectangle {
                color: HUDStyle.colors.formBackground
                anchors.fill: parent
            }
            Item {
                id: track_info
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.bottom: buttons.top
                anchors.bottomMargin: 16
                anchors.topMargin: 16
                anchors.leftMargin: 16
                anchors.rightMargin: 16
                anchors.left: parent.left
                RowLayout {
                    id: signalStrength
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.leftMargin: 5
                    spacing: 2

                    Accessible.name: qsTr("Signal noise ratio: " + pluginContext.RadioController.snr )

                    Rectangle{
                        height: 4
                        width: 4
                        color: (pluginContext.RadioController.snr > 2) ? "green" : "dimgrey"
                        Accessible.ignored: true
                    }
                    Rectangle{
                        height: 8
                        width: 4
                        color: (pluginContext.RadioController.snr > 5) ? "green" : "dimgrey"
                        Accessible.ignored: true
                    }
                    Rectangle{
                        height: 12
                        width: 4
                        color: (pluginContext.RadioController.snr > 8) ? "green" : "dimgrey"
                        Accessible.ignored: true
                    }
                    Rectangle{
                        height: 16
                        width: 4
                        color: (pluginContext.RadioController.snr > 11) ? "green" : "dimgrey"
                        Accessible.ignored: true
                    }

                    Rectangle{
                        height: 20
                        width: 4
                        color: (pluginContext.RadioController.snr > 15) ? "green" : "dimgrey"
                        Accessible.ignored: true
                    }
                }
                ThemeText {
                    id: dabMode
                    text: (pluginContext.RadioController.isDAB ? "DAB" : "DAB+")
                          + " " + pluginContext.RadioController.audioMode
                    anchors.left: signalStrength.right
                    verticalAlignment: Text.AlignTop
                    horizontalAlignment: Text.AlignRight
                    wrapMode: Text.WordWrap
                    anchors.leftMargin: 16
                    anchors.right: parent.right
                    anchors.top: parent.top
                    anchors.rightMargin: 0
                    renderType: Text.NativeRendering;
                    font.hintingPreference: Font.PreferVerticalHinting
                }

                ThemeText {
                    id: ensemble
                    text : pluginContext.RadioController.ensemble.trim()
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                    anchors.topMargin: 8
                    anchors.right: parent.right
                    anchors.top: dabMode.bottom
                    anchors.rightMargin: 0
                    anchors.left: parent.left
                    anchors.leftMargin: 0
                    renderType: Text.NativeRendering;
                    font.hintingPreference: Font.PreferVerticalHinting
                }


                ThemeHeaderText {
                    id: title
                    level: 3
                    text : pluginContext.RadioController.title.trim()
                    anchors.verticalCenter: parent.verticalCenter
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                    anchors.right: parent.right
                    anchors.rightMargin: 0
                    anchors.left: parent.left
                    anchors.leftMargin: 0
                    renderType: Text.NativeRendering;
                    font.hintingPreference: Font.PreferVerticalHinting
                }


                ThemeHeaderText {
                    id: radioText
                    level: 1
                    text:pluginContext.RadioController.text.trim()
                    verticalAlignment: Text.AlignVCenter
                    wrapMode: Text.WordWrap
                    anchors.bottomMargin: 8
                    horizontalAlignment: Text.AlignHCenter
                    anchors.right: parent.right
                    anchors.top: title.bottom
                    anchors.bottom: stationType.top
                    anchors.rightMargin: 0
                    anchors.left: parent.left
                    anchors.leftMargin: 0
                    renderType: Text.NativeRendering;
                    font.hintingPreference: Font.PreferVerticalHinting
                }


                ThemeText {
                    id: stationType
                    text: pluginContext.RadioController.stationType
                    anchors.bottom: parent.bottom
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignTop
                    wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                    anchors.horizontalCenter: parent.horizontalCenter
                    renderType: Text.NativeRendering
                    font.hintingPreference: Font.PreferVerticalHinting
                }


            }

            RowLayout {
                id: buttons
                width: height * 5
                height: parent.height*0.15
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 16

                ImageButton {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    imageSource: pluginContext.RadioController.isPlaying ? "qrc:/qml/icons/pause.png" : "qrc:/qml/icons/play.png"
                    id:playButton
                    onClicked: {
                        if(pluginContext.RadioController.isPlaying){
                            pluginContext.RadioController.stop();
                        } else {
                            pluginContext.playLastStation();
                        }
                    }
                }
            }
        }

    }
    StationDrawer {
        id: stationDrawer
        width: parent.width * 0.3
        anchors.bottom: parent.bottom
        anchors.topMargin: 0
        anchors.bottomMargin: 0
        anchors.top: parent.top
    }
    Connections{
        target: pluginContext.RadioController

        onShowErrorMessage:{
        }

        onShowInfoMessage:{
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;formeditorColor:"#808080";height:480;width:640}
}
##^##*/
