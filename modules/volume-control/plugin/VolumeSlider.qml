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

import QtQuick 2.4
import QtQuick.Layouts 1.0
import QtQuick.Controls 1.0

import org.viktorgino.headunit.volume 0.1

RowLayout {
    Layout.bottomMargin: hundredPercentLabel.height

    Slider {
        id: slider

        // Helper properties to allow async slider updates.
        // While we are sliding we must not react to value updates
        // as otherwise we can easily end up in a loop where value
        // changes trigger volume changes trigger value changes.
        property int volume: Volume
        property bool ignoreValueChange: true

        Layout.fillWidth: true
        minimumValue: PulseAudio.MinimalVolume
        maximumValue: PulseAudio.MaximalVolume
        visible: HasVolume
        enabled: VolumeWritable
        opacity: Muted ? 0.5 : 1

        Component.onCompleted: {
            ignoreValueChange = false;
        }

        onVolumeChanged: {
            var oldIgnoreValueChange = ignoreValueChange;
            ignoreValueChange = true;
            value = Volume;
            ignoreValueChange = oldIgnoreValueChange;
        }

        onValueChanged: {
            if (!ignoreValueChange) {
                Volume = value;
                Muted = value == 0;

                if (!pressed) {
                    updateTimer.restart();
                }
            }
        }

        onPressedChanged: {
            if (!pressed) {
                // Make sure to sync the volume once the button was
                // released.
                // Otherwise it might be that the slider is at v10
                // whereas PA rejected the volume change and is
                // still at v15 (e.g.).
                updateTimer.restart();
            }
        }

        Label {
            id: hundredPercentLabel
            readonly property real hundredPos: (slider.width / slider.maximumValue) * PulseAudio.NormalVolume
            z: slider.z - 1
            x: (Qt.application.layoutDirection == Qt.RightToLeft ? slider.width - hundredPos : hundredPos) - width / 2
            y: slider.height / 1.2
            opacity: 0.5
            font.pixelSize: slider.height / 2.2
            text: "100%"
        }

        Timer {
            id: updateTimer
            interval: 200
            onTriggered: slider.value = Volume
        }
    }

    Label {
        id: percentText
        readonly property real value: PulseObject.volume > slider.maximumValue ? PulseObject.volume : slider.value
        Layout.alignment: Qt.AlignHCenter
        Layout.minimumWidth: percentMetrics.advanceWidth
        horizontalAlignment: Qt.AlignRight
        text: Math.round(value / PulseAudio.NormalVolume * 100.0) + "%"
    }

    TextMetrics {
        id: percentMetrics
        font: percentText.font
        text: "only used for sizing, should be widest possible string 100%"
    }
}
