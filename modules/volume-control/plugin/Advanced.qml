/*
    Copyright 2014-2015 Harald Sitter <sitter@kde.org>

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of
    the License or (at your option) version 3 or any later version
    accepted by the membership of KDE e.V. (or its successor approved
    by the membership of KDE e.V.), which shall act as a proxy
    defined in Section 14 of version 3 of the license.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

import QtQuick 2.0
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.3

import org.viktorgino.headunit.volume 0.1

ScrollView {
    id: scrollView

    ColumnLayout {
        Component.onCompleted: {
            // Normal binding causes binding loops
            width = Qt.binding(function() {
                return scrollView.viewport.width;
            });
        }

        Header {
            Layout.fillWidth: true
            enabled: view.count > 0
            text: "Device Profiles"
            disabledText: "No Device Profiles Available"
        }

        ListView {
            id: view
            Layout.fillWidth: true
            Layout.preferredHeight: contentHeight
            Layout.margins: 8
            interactive: false
            spacing: 8
            model: CardModel {}
            delegate: CardListItem {}
        }

        Header {
            Layout.fillWidth: true
            text: "Advanced Output Configuration"
        }

        ModuleManager {
            id: moduleManager
        }

        CheckBox {
            Layout.fillWidth: true
            Layout.topMargin: 8
            Layout.leftMargin: 8
            Layout.rightMargin: 8
            text: "Add virtual output device for simultaneous output on all local sound cards"
            checked: moduleManager.combineSinks
            onCheckedChanged: moduleManager.combineSinks = checked;
            enabled: moduleManager.loadedModules.indexOf("module-gconf") != -1
        }

        CheckBox {
            Layout.fillWidth: true
            Layout.leftMargin: 8
            Layout.rightMargin: 8
            text: "Automatically switch all running streams when a new output becomes available"
            checked: moduleManager.switchOnConnect
            onCheckedChanged: moduleManager.switchOnConnect = checked;
            enabled: moduleManager.loadedModules.indexOf("module-gconf") != -1
        }

        Label {
            Layout.alignment: Qt.AlignHCenter
            enabled: false
            font.italic: true
            text: "Requires 'module-gconf' PulseAudio module"
            visible: moduleManager.loadedModules.indexOf("module-gconf") == -1
        }
    }
}
