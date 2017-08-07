/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

import QtQuick 2.2
import QtQuick.Controls 2.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.1
import Qt.labs.settings 1.0

// Import custom styles
import "qrc:/src/gui/QML"
import "qrc:/src/gui/QML/style"

Item {
    id: mainWindow
    visible: true
    property bool isLandscape: true
    function getWidth() {
        if(Screen.desktopAvailableWidth < Units.dp(700)
                || Screen.desktopAvailableHeight < Units.dp(500)
                || Qt.platform.os == "android") // Always full screen on Android
            return Screen.desktopAvailableWidth
        else
            return Units.dp(700)
    }

    function getHeight() {
        if(Screen.desktopAvailableHeight < Units.dp(500)
                || Screen.desktopAvailableWidth < Units.dp(700)
                || Qt.platform.os == "android")  // Always full screen on Android
            return Screen.desktopAvailableHeight
        else
            return Units.dp(500)
    }

    width: getWidth()
    height: getHeight()

    Component.onCompleted: {
        console.debug("os: " + Qt.platform.os)
        console.debug("desktopAvailableWidth: " + Screen.desktopAvailableWidth)
        console.debug("desktopAvailableHeight: " + Screen.desktopAvailableHeight)
        console.debug("orientation: " + Screen.orientation)
        console.debug("devicePixelRatio: " + Screen.devicePixelRatio)
        console.debug("pixelDensity: " + Screen.pixelDensity)
    }

    property int stackViewDepth
    signal stackViewPush(Item item)
    signal stackViewPop()
    signal stackViewComplete()
    signal stationClicked()
    property alias isExpertView: settingsPage.enableExpertModeState

    Settings {
        property alias width : mainWindow.width
        property alias height : mainWindow.height
    }

    onIsExpertViewChanged: {
        if(stackViewDepth > 1)
        {
            if(isExpertView == true)
                infoMessagePopup.text = qsTr("Expert mode is enabled")
            else
                infoMessagePopup.text = qsTr("Expert mode is disabled")
            infoMessagePopup.open()
        }
    }


    SettingsPage{
        id:settingsPage
        visible: false
    }




    Item {
        id: __toolBar
        //border.bottom: Units.dp(10)
        //source: "images/toolbar.png"
        width: parent.width
        height: Units.dp(40);

        Rectangle {
            id: backButton
            width: Units.dp(60)
            anchors.left: parent.left
            anchors.leftMargin: Units.dp(20)
            anchors.verticalCenter: parent.verticalCenter
            antialiasing: true
            radius: Units.dp(4)
            color: backmouse.pressed ? "#222" : "transparent"
            property bool isSettings: false
            Behavior on opacity { NumberAnimation{} }
            Image {
                anchors.verticalCenter: parent.verticalCenter
                source: parent.isSettings ? "qrc:/src/gui/QML/images/navigation_previous_item.png" : "qrc:/src/gui/QML/images/icon-settings.png"
                height: parent.isSettings? Units.dp(20) : Units.dp(23)
                fillMode: Image.PreserveAspectFit
            }
            MouseArea {
                id: backmouse
                scale: 1
                anchors.fill: parent
                anchors.margins: Units.dp(-20)
                onClicked: {
                    backButton.isSettings = !backButton.isSettings
                }
            }
        }

        TextTitle {
            x: backButton.x + backButton.width + Units.dp(20)
            anchors.verticalCenter: parent.verticalCenter
            text: "welle.io"
        }

        TextStandart {
            x: mainWindow.width - width - Units.dp(5) - infoButton.width
            anchors.verticalCenter: parent.verticalCenter
            anchors.rightMargin: Units.dp(5)
            text: "01.01.2016 00:00"
            id: dateTimeDisplay
        }

        Rectangle {
            id: infoButton
            width: backButton.isSettings ? Units.dp(40) : 0
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            antialiasing: true
            radius: Units.dp(4)
            color: backmouse.pressed ? "#222" : "transparent"
            property bool isInfoPage: false
            Image {
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                source: backButton.isSettings ? "qrc:/src/gui/QML/images/icon-info.png" : ""
                anchors.rightMargin: Units.dp(20)
                height: Units.dp(23)
                fillMode: Image.PreserveAspectFit
            }
            MouseArea {
                id: infomouse
                scale: 1
                anchors.fill: parent
                anchors.margins: Units.dp(-20)
                onClicked: {
                    infoButton.isInfoPage = !infoButton.isInfoPage
                }
            }
        }
    }



    Loader {
        id:mainViewLoader
        anchors.top: __toolBar.bottom
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.topMargin: 0
        Layout.margins: Units.dp(10);
        sourceComponent: {
            mainWindow.isLandscape = true;
            if(isExpertView)
                return landscapeViewExpert3D
            else
                return landscapeView3D
        }
    }

    Component {
        id:channelBrowser
        ChannelBrowser{
            Connections {
                target: backmouse
                onClicked: {
                    if(infoPageVisible){
                        infoPageVisible = false
                        backButton.isSettings = true
                    } else {
                        settingsVisible = backButton.isSettings
                    }
                }
            }
            Connections {
                target: infomouse
                onClicked: {
                    infoPageVisible = infoButton.isInfoPage
                }
            }
            Component.onCompleted: {
                settingsVisible = backButton.isSettings
                enableFullScreenState = settingsPage.enableFullScreenState
                enableExpertModeState = settingsPage.enableExpertModeState
                enableAGCState = settingsPage.enableAGCState
                manualGainState = settingsPage.manualGainState
                is3D = settingsPage.is3D
            }
            onEnableAGCStateChanged: {
                settingsPage.enableAGCState = enableAGCState
            }
            onEnableExpertModeStateChanged: {
                settingsPage.enableExpertModeState = enableExpertModeState

            }
            onEnableFullScreenStateChanged: {
                settingsPage.enableFullScreenState = enableFullScreenState
            }
            onManualGainStateChanged: {
                settingsPage.manualGainState = manualGainState
            }
            onIs3DChanged: {
                settingsPage.is3D = is3D
            }
        }
    }
    Component {
        id: landscapeView3D

        Loader {
            id: stationView
            Layout.minimumWidth: Units.dp(350)
            Layout.margins: Units.dp(10)
            sourceComponent: channelBrowser
        }
    }
    Component {
        id: landscapeViewExpert3D

        SplitView {
            id: splitView
            anchors.fill: parent
            orientation: Qt.Horizontal

            Loader {
                id: stationView
                Layout.minimumWidth: Units.dp(350)
                Layout.margins: Units.dp(10)
                sourceComponent: channelBrowser
            }
            Loader {
                id: expertViewLoader
                Layout.margins: Units.dp(10)
                Layout.fillWidth: true
                sourceComponent: expertView
            }

            Settings {
                property alias expertStationViewWidth: stationView.width
                property alias expertViewWidth: expertViewLoader.width
            }
        }
    }

    // expertView
    Component {
        id: expertView

        ExpertView{
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.preferredWidth: Units.dp(400)
            width: Units.dp(400)
        }
    }

    MessagePopup {
        id: errorMessagePopup
        x: mainWindow.width/2 - width/2
        y: mainWindow.height  - __toolBar.height - height
        revealedY: mainWindow.height - __toolBar.height - height
        hiddenY: mainWindow.height
        color: "#8b0000"
    }

    MessagePopup {
        id: infoMessagePopup
        x: mainWindow.width/2 - width/2
        y: mainWindow.height  - __toolBar.height - height
        revealedY: mainWindow.height - __toolBar.height - height
        hiddenY: mainWindow.height
        color:  "#468bb7"
        onOpened: closeTimer.running = true;
        Timer {
            id: closeTimer
            interval: 1 * 5000 // 5 s
            repeat: false
            onTriggered: {
              infoMessagePopup.close()
            }
        }
    }

    MessageDialog {
        id: androidRTLSDRDialog
        icon: StandardIcon.Warning
        standardButtons: StandardButton.Ok | StandardButton.Cancel
        onAccepted: {
            Qt.openUrlExternally("https://play.google.com/store/apps/details?id=marto.rtl_tcp_andro")
            Qt.quit()
        }
    }

    Connections{
        target: cppGUI

        onSetGUIData:{
            dateTimeDisplay.text = GUIData.DateTime
        }
    }

    Connections{
        target: cppRadioController

        onShowErrorMessage:{
            errorMessagePopup.text = Text;
            errorMessagePopup.open();
        }

        onShowInfoMessage:{
            infoMessagePopup.text = Text;
            infoMessagePopup.open();
        }

        onShowAndroidInstallDialog:{
            androidRTLSDRDialog.title = Title
            androidRTLSDRDialog.text = Text;
            androidRTLSDRDialog.open();
        }
    }
}
