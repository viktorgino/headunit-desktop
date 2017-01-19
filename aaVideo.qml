/*
    Copyright (C) 2012-2013 Collabora Ltd. <info@collabora.com>
      @author George Kiagiadakis <george.kiagiadakis@collabora.com>
    Copyright (C) 2013 basysKom GmbH <info@basyskom.com>
      @author Benjamin Federau <benjamin.federau@basyskom.com>

    This library is free software; you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
import QtQuick 2.0
import QtGStreamer 1.0


VideoItem {
    id: aaVideo
    width: 800
    height: 480
    surface: videoSurface1

    MouseArea {
        id: mouseArea1
        anchors.fill: parent
        onPressed:{
            headunit.mouseDown(Qt.point(mouse.x, mouse.y));
        }
        onPositionChanged:{
            headunit.mouseMove(Qt.point(mouse.x, mouse.y));
        }
        onReleased:{
            headunit.mouseUp(Qt.point(mouse.x, mouse.y));
        }
    }

    Keys.onPressed: {
        switch(event.key){
        case Qt.Key_Left:
            console.log("Key Left");
            break;
        }
    }
}
