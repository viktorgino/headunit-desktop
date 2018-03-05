/*
    Copyright 2014-2015 Harald Sitter <sitter@kde.org>
    Copyright 2016 David Rosca <nowrep@gmail.com>

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
import QtQuick.Controls 1.3

ComboBox {
    onModelChanged: updateIndex()
    onCountChanged: updateIndex()

    onActivated: {
        if (index === -1) {
            return;
        }

        DeviceIndex = modelIndexToDeviceIndex(index);
    }

    function updateIndex() {
        currentIndex = deviceIndexToModelIndex(DeviceIndex);
    }

    function deviceIndexToModelIndex(deviceIndex) {
        textRole = 'Index';
        var searchString = '';
        if (deviceIndex !== 0) {
            searchString = '' + deviceIndex;
        }
        var modelIndex = find(searchString);
        textRole = 'Description';
        return modelIndex;
    }

    function modelIndexToDeviceIndex(modelIndex) {
        textRole = 'Index';
        var deviceIndex = Number(textAt(modelIndex));
        textRole = 'Description';
        return deviceIndex;
    }

    Connections {
        target: PulseObject
        onDeviceIndexChanged: updateIndex();
    }

    Component.onCompleted: updateIndex();
}
