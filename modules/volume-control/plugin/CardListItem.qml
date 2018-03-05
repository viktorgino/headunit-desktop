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
            id: clientIcon
            Layout.alignment: Qt.AlignHCenter
            width: height
            height: nameLabel.contentHeight
            text: IconName || "audio-card"
        }

        Label {
            id: nameLabel
            Layout.fillWidth: true
            text: Properties["device.description"] || Name
            elide: Text.ElideRight
        }
    }

    RowLayout {
        Label {
            id: profileLabel
            text: "Profile:"
            font.bold: true
        }
        ComboBox {
            Layout.fillWidth: true
            model: Profiles.filter(function (profile) {
                return profile.availability === Profile.Available;
            })
            // NOTE: model resets (i.e. profiles property changes) will reset
            // the currentIndex, so force it to be set on model changes, otherwise
            // it would eventually become 0 when it shouldn't be.
            onModelChanged: currentIndex = model.indexOf(Profiles[ActiveProfileIndex])
            textRole: "description"
            onActivated: ActiveProfileIndex = Profiles.indexOf(model[index])
        }
    }

    ListItemSeperator { view: delegate.ListView.view }
}
