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
            enabled: sinks.count > 0
            text: "Outputs"
            disabledText: "No Output Devices Available"
        }

        ListView {
            id: sinks
            Layout.fillWidth: true
            Layout.preferredHeight: contentHeight
            Layout.margins: 6
            interactive: false
            spacing: 16
            model:  SinkModel { }
            delegate: DeviceListItem {}
        }

        Header {
            Layout.fillWidth: true
            enabled: sources.count > 0
            text: "Inputs"
            disabledText: "No Input Devices Available"
        }

        ListView {
            id: sources
            Layout.fillWidth: true
            Layout.preferredHeight: contentHeight
            Layout.margins: 6
            interactive: false
            model: SourceModel { }
            delegate: DeviceListItem {}
        }

        Header {
            Layout.fillWidth: true
            enabled: eventStreamView.count || sinkInputView.count
            text: "Playback"
            disabledText: "No Applications Playing Audio"
        }

        ListView {
            id: eventStreamView
            Layout.fillWidth: true
            Layout.preferredHeight: contentHeight
            Layout.margins: 20
            interactive: false
            spacing: 16
            model: StreamRestoreModel {}
            delegate: StreamListItem {
                deviceModel: SinkModel{}
            }
        }

        ListView {
            id: sinkInputView
            Layout.fillWidth: true
            Layout.preferredHeight: contentHeight
            Layout.margins: 20
            interactive: false
            spacing: 16
            model: SinkInputModel {}
            delegate: StreamListItem {
                deviceModel: SinkModel{}
            }
        }

        Header {
            Layout.fillWidth: true
            enabled: sourceOutputView.count > 0
            text: "Capture"
            disabledText: "No Applications Recording Audio"
        }

        ListView {
            id: sourceOutputView
            Layout.fillWidth: true
            Layout.preferredHeight: contentHeight
            Layout.margins: 8
            interactive: false
            spacing: 16
            model: SourceOutputModel {}

            delegate: StreamListItem {
                deviceModel: SourceModel{}
            }
        }
    }
}
