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

import QtQuick 2.7
import QtQuick.Controls 1.4
import QtQuick.Window 2.2

import org.viktorgino.headunit.volume 0.1

Item {
    visible: true
    Component.onCompleted: {
        console.log("asd");
    }

    MouseArea {
        property int wheelDelta: 0
        property QtObject sinkModel: SinkModel { }
        property QtObject sourceModel: SourceModel { }

        acceptedButtons: Qt.NoButton
        anchors.fill: parent

        //ConfigModule.quickHelp: "This module allows to set up the Pulseaudio sound subsystem."

        onWheel: {
            if (tabView.childAt(wheel.x, wheel.y).objectName != "tabbar") {
                wheel.accepted = false;
                return;
            }
            var delta = wheel.angleDelta.y || wheel.angleDelta.x;
            wheelDelta += delta;
            // Magic number 120 for common "one click"
            // See: http://qt-project.org/doc/qt-5/qml-qtquick-wheelevent.html#angleDelta-prop
            while (wheelDelta >= 120) {
                wheelDelta -= 120;
                tabView.currentIndex = Math.max(0, tabView.currentIndex - 1);
            }
            while (wheelDelta <= -120) {
                wheelDelta += 120;
                tabView.currentIndex = Math.min(tabView.count - 1, tabView.currentIndex + 1);
            }
        }

        TabView {
            id: tabView
            anchors.fill: parent

            Tab {
                title: "Applications"
                Volumes {}
            }
            Tab {
                title: "Advanced"
                Advanced {}
            }
        }
    }
}
