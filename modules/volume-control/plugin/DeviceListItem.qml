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
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0

import org.viktorgino.headunit.volume 0.1

ColumnLayout {
    id: delegate
    width: parent.width

    RowLayout {

        Text {
            width: height
            height: inputText.height
            text: IconName || "audio-card"
        }

        Label {
            id: inputText
            Layout.fillWidth: true
            elide: Text.ElideRight
            text: Description
        }

        DefaultDeviceButton {
            visible: delegate.ListView.view.count > 1
            isDefault: Default
            onCheckedChanged: {
                if (!checked) {
                    // Cannot unset default device
                    checked = isDefault;
                } else {
                    Default = true;
                }
            }
        }

        MuteButton {
            muted: Muted
            onCheckedChanged: Muted = checked
        }
    }

    ColumnLayout {
        width: parent.width

        RowLayout {
            visible: portbox.count > 1

            Label {
                text: "Port"
            }

            ComboBox {
                id: portbox
                readonly property var ports: Ports
                Layout.fillWidth: true
                onModelChanged: currentIndex = ActivePortIndex
                currentIndex: ActivePortIndex
                onActivated: ActivePortIndex = index

                onPortsChanged: {
                    var items = [];
                    for (var i = 0; i < ports.length; ++i) {
                        var port = ports[i];
                        var text = port.description;
                        if (port.availability == Port.Unavailable) {
                            if (port.name == "analog-output-speaker" || port.name == "analog-input-microphone-internal") {
                                text += "Port is unavailable (unavailable)";
                            } else {
                                text += "Port is unplugged (unplugged)"
                            }
                        }
                        items.push(text);
                    }
                    model = items;
                }
            }
        }

        VolumeSlider {}
    }

    ListItemSeperator { view: delegate.ListView.view }
}
