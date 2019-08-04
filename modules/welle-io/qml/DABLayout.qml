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

import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.3
import QtQuick.Controls.Material 2.1
import QtQuick.Controls.Universal 2.1
import QtQuick.Window 2.2
import Qt.labs.settings 1.0

import "qrc:/WelleIoPluginRes/QML"
import "qrc:/WelleIoPluginRes/QML/texts"
import "qrc:/WelleIoPluginRes/QML/settingpages"
import "qrc:/WelleIoPluginRes/QML/components"

Item {
    id: __root

    property bool isExpertView: false
    property bool isFullScreen: false
    property bool isLoaded: false

    visible: true

    StationListModel { id: stationList }
    StationListModel { id: favoritsList }

    readonly property bool inPortrait: __root.width < __root.height

    Component.onCompleted: {
        // Show error message if one occured during startup
        if(errorMessagePopup.text != "")
            errorMessagePopup.open();

        isLoaded = true
    }

    Settings {
//        property alias width : __root.width
//        property alias height : __root.height
        property alias stationListSerialize: stationList.serialized
        property alias favoritsListSerialize: favoritsList.serialized
        property alias stationListBoxIndex: stationListBox.currentIndex
    }

    ToolBar {
        id: overlayHeader
        Material.foreground: "white"
        width: __root.width
        height: __root.height * 0.05
        RowLayout {
            spacing: 20
            anchors.fill: parent

            ToolButton {
                text: qsTr("‹")
                onClicked: {
                    if (stationDrawer.isOpen)
                    {
                        stationDrawer.close()
                    }
                    else
                    {

                        // Open drawer
                        stationDrawer.open()
                    }
                }
            }

            Label {
                id: titleLabel
                text: "welle.io"
                font.pixelSize: Units.dp(20)
                elide: Label.ElideRight
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                Layout.fillWidth: true
            }

            ToolButton {
                text: qsTr("⋮")
                onClicked: optionsMenu.open()

                Menu {
                    id: optionsMenu
                    x: parent.width - width
                    transformOrigin: Menu.TopRight

                    MenuItem {
                        text: qsTr("Settings")
                        font.pixelSize: TextStyle.textStandartSize
                        onTriggered: {
                            globalSettingsDialog.title = text
                            globalSettingsDialog.open()
                        }
                    }
                    MenuItem {
                        text: qsTr("Expert Settings")
                        font.pixelSize: TextStyle.textStandartSize
                        onTriggered: {
                            expertSettingsDialog.title = text
                            expertSettingsDialog.open()
                        }
                    }
                    MenuItem {
                        text: qsTr("About")
                        font.pixelSize: TextStyle.textStandartSize
                        onTriggered: {
                            aboutDialog.title = text
                            aboutDialog.open()
                        }
                    }
                }
            }
        }
    }

    Item {
        id: stationDrawer

        y: overlayHeader.height
        width: __root.width < 1.5 * implicitWidth ? __root.width : Units.dp(320)
        height: __root.height - overlayHeader.height
        property bool isOpen: true

        function open () {
            x = 0
            isOpen = true
        }
        function close () {
            x = - width
            isOpen = false
        }

        Behavior on x {
            NumberAnimation { duration: 500 }
        }

        visible: !inPortrait

        parent: __root

        // Workaround for touch displays. (Discovered with Windows 10)
        // For some reason the dawer will be closed before it is openend
        // Enable closing again

        Rectangle {
            anchors.fill: parent
            color: "#ffffff"
        }

        ColumnLayout {
            anchors.fill: parent
            anchors.topMargin: Units.dp(5)

            RowLayout {
                WComboBox {
                    id: stationListBox
                    Layout.preferredWidth: Units.dp(200)
                    background: Rectangle { color: "white" }

                    model:  [qsTr("All stations"), qsTr("Favorites")]

                    onCurrentIndexChanged: {
                        switch(currentIndex) {
                        case 0: stationChannelView.model = stationList; break;
                        case 1: stationChannelView.model = favoritsList; break;
                        }
                    }
                }

                Item { // Dummy element for filling
                    Layout.fillWidth: true
                }

                Button {
                    id: menuButton
                    text: qsTr("⋮")
                    icon.height: Units.dp(15)
                    icon.width: Units.dp(15)
                    background: Rectangle {
                        color: menuButton.pressed ? "lightgrey" : "white"
                        opacity: menuButton.pressed ? 100 : 0
                    }
                    onClicked: stationMenu.open()
                    implicitWidth: contentItem.implicitWidth + Units.dp(15)

                    Menu {
                        id: stationMenu

                        MenuItem {
                            id: startStationScanItem
                            text: qsTr("Start station scan")
                            font.pixelSize: TextStyle.textStandartSize
                            font.family: TextStyle.textFont
                            onTriggered:  {
                                startStationScanItem.enabled = false
                                stopStationScanItem.enabled = true
                                WelleIoPlugin.RadioController.startScan()
                            }
                        }

                        MenuItem {
                            id: stopStationScanItem
                            text: qsTr("Stop station scan")
                            font.pixelSize: TextStyle.textStandartSize
                            font.family: TextStyle.textFont
                            enabled: false
                            onTriggered:  {
                                startStationScanItem.enabled = true
                                stopStationScanItem.enabled = false
                                WelleIoPlugin.RadioController.stopScan()
                            }
                        }

                        MenuItem {
                            text: qsTr("Clear station list")
                            font.pixelSize: TextStyle.textStandartSize
                            font.family: TextStyle.textFont
                            onTriggered: stationList.clearStations()
                        }

                        MenuItem {
                            id: stationSettingsItem
                            text: qsTr("Station settings")
                            font.pixelSize: TextStyle.textStandartSize
                            font.family: TextStyle.textFont
                            onTriggered: {
                                stationSettingsDialog.title = text
                                stationSettingsDialog.open()
                            }
                        }
                    }
                }
            }

            TextStandart {
                text: qsTr("No stations in list")
                visible: stationChannelView.count ? false : true
                Layout.margins: Units.dp(10)
            }

            ListView {
                id: stationChannelView
                model: stationList
                Layout.fillWidth: true
                Layout.fillHeight: true
                clip: true
                delegate: StationDelegate {
                    stationNameText: stationName
                    stationSIdValue: stationSId
                    channelNameText: channelName
                    isFavorit: favorit
                    isExpert: isExpertView
                    onClicked: WelleIoPlugin.RadioController.play(channelName, stationName, stationSId)
                    onFavoritClicked: {
                        var favoritInvert = !favorit
                        stationList.setFavorit(stationSId, favoritInvert) // Invert favorit

                        if(favoritInvert)
                            favoritsList.addStation(stationName, stationSId, channelName, true)
                        else
                            favoritsList.removeStation(stationSId);
                    }
                }

                ScrollIndicator.vertical: ScrollIndicator { }
            }

            RowLayout {
                Layout.margins: Units.dp(10)
                visible: isExpertView ? true : false

                TextStandart {
                    text: qsTr("Manual channel")
                    Layout.fillWidth: true
                }

                WComboBox {
                    id: manualChannelBox
                    model: ["5A", "5B", "5C", "5D",
                        "6A", "6B", "6C", "6D",
                        "7A", "7B", "7C", "7D",
                        "8A", "8B", "8C", "8D",
                        "9A", "9B", "9C", "9D",
                        "10A", "10B", "10C", "10D",
                        "11A", "11B", "11C", "11D",
                        "12A", "12B", "12C", "12D",
                        "13A", "13B", "13C", "13D", "13E", "13F",
                        "LA", "LB", "LC", "LD",
                        "LE", "LF", "LG", "LH",
                        "LI", "LJ", "LK", "LL",
                        "LM", "LN", "LO", "LP"]

                    Layout.preferredHeight: Units.dp(25)
                    Layout.preferredWidth: Units.dp(130)
                    onActivated: {
                        WelleIoPlugin.RadioController.setManualChannel(model[index])
                    }
                }
            }
        }
    }

    Flickable {
        id: flickable
        anchors.leftMargin: (!inPortrait && stationDrawer.opened) ? stationDrawer.width: undefined
        anchors.topMargin: overlayHeader.height
        anchors.left : stationDrawer.right
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom

        GeneralView {
            id: generalView
            isExpert: isExpertView
            isPortrait: inPortrait
        }
    }

    RoundButton {
        text: "\u002b" // Unicode character '+'
        onClicked: viewMenu.open()
        x: parent.width - width - 50
        y: parent.height - height - 50
        visible: isExpertView

        Menu {
            id: viewMenu
            transformOrigin: Menu.TopRight

            MenuItem {
                text: qsTr("Service Overview")
                font.pixelSize: TextStyle.textStandartSize
                onTriggered: generalView.addComponent("qrc:/WelleIoPluginRes/QML/RadioView.qml", -1 ,-1)
            }

            MenuItem {
                text: qsTr("Service Details")
                font.pixelSize: TextStyle.textStandartSize
                onTriggered: generalView.addComponent("qrc:/WelleIoPluginRes/QML/expertviews/ServiceDetails.qml", -1 ,-1)
            }

            MenuItem {
                text: qsTr("MOT Slide Show")
                font.pixelSize: TextStyle.textStandartSize
                onTriggered: generalView.addComponent("qrc:/WelleIoPluginRes/QML/MotView.qml", -1 ,-1)
            }

            MenuItem {
                text: qsTr("Spectrum")
                font.pixelSize: TextStyle.textStandartSize
                onTriggered: generalView.addComponent("qrc:/WelleIoPluginRes/QML/expertviews/SpectrumGraph.qml", -1 ,-1)
            }

            MenuItem {
                text: qsTr("Impulse Response")
                font.pixelSize: TextStyle.textStandartSize
                onTriggered: generalView.addComponent("qrc:/WelleIoPluginRes/QML/expertviews/ImpulseResponseGraph.qml", -1 ,-1)
            }

            MenuItem {
                text: qsTr("Constellation Diagram")
                font.pixelSize: TextStyle.textStandartSize
                onTriggered: generalView.addComponent("qrc:/WelleIoPluginRes/QML/expertviews/ConstellationGraph.qml", -1 ,-1)
            }

            MenuItem {
                text: qsTr("Null Symbol")
                font.pixelSize: TextStyle.textStandartSize
                onTriggered: generalView.addComponent("qrc:/WelleIoPluginRes/QML/expertviews/NullSymbolGraph.qml", -1 ,-1)
            }

            MenuItem {
                text: qsTr("Console Output")
                font.pixelSize: TextStyle.textStandartSize
                onTriggered: generalView.addComponent("qrc:/WelleIoPluginRes/QML/expertviews/TextOutputView.qml", -1 ,-1)
            }

            MenuItem {
                text: qsTr("RAW Recorder")
                font.pixelSize: TextStyle.textStandartSize
                onTriggered: generalView.addComponent("qrc:/WelleIoPluginRes/QML/expertviews/RawRecorder.qml", -1 ,-1)
            }
        }
    }

    WDialog {
        id: aboutDialog

        contentItem: InfoPage{
            id: infoPage
        }
    }

    WDialog {
        id: stationSettingsDialog
        content: Loader {
            anchors.right: parent.right
            anchors.left: parent.left
            height: parent.implicitHeight
            source:  "qrc:/WelleIoPluginRes/QML/settingpages/ChannelSettings.qml"
        }
    }

    WDialog {
        id: globalSettingsDialog

        content: Loader {
            id: globalSettingsLoader
            anchors.right: parent.right
            anchors.left: parent.left
            height: parent.implicitHeight
            source:  "qrc:/WelleIoPluginRes/QML/settingpages/GlobalSettings.qml"
            onLoaded : isFullScreen = globalSettingsLoader.item.enableFullScreenState
        }

        Connections {
            target: globalSettingsLoader.item
            onEnableFullScreenStateChanged : isFullScreen = globalSettingsLoader.item.enableFullScreenState
        }
    }

    WDialog {
        id: expertSettingsDialog

        content: Loader {
            id: expertSettingsLoader
            anchors.right: parent.right
            anchors.left: parent.left
            height: parent.implicitHeight
            source:  "qrc:/WelleIoPluginRes/QML/settingpages/ExpertSettings.qml"
            onLoaded: isExpertView = expertSettingsLoader.item.enableExpertModeState
        }

        Connections {
            target: expertSettingsLoader.item
            onEnableExpertModeStateChanged : isExpertView = expertSettingsLoader.item.enableExpertModeState
        }
    }

    MessagePopup {
        id: errorMessagePopup
        x: __root.width/2 - width/2
        y: __root.height  - overlayHeader.height - height
        revealedY: __root.height - overlayHeader.height - height
        hiddenY: __root.height
        color: "#8b0000"
    }

    MessagePopup {
        id: infoMessagePopup
        x: __root.width/2 - width/2
        y: __root.height  - overlayHeader.height - height
        revealedY: __root.height - overlayHeader.height - height
        hiddenY: __root.height
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

    Connections{
        target: WelleIoPlugin.RadioController

        onShowErrorMessage:{
            errorMessagePopup.text = Text;

            if(__root.isLoaded)
                errorMessagePopup.open();
        }

        onShowInfoMessage:{
            infoMessagePopup.text = Text;
            infoMessagePopup.open();
        }

        onScanStopped:{
            startStationScanItem.enabled = true
            stopStationScanItem.enabled = false
        }

        onScanProgress:{
            startStationScanItem.enabled = false
            stopStationScanItem.enabled = true
        }

        onNewStationNameReceived: stationList.addStation(station, sId, channel, false)
    }
}
