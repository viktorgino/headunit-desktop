import QtQuick 2.11
import QtQuick.Controls 2.4

import HUDTheme 1.0

Item {
    id: __root
    clip: true
    property alias model : listView.model

    signal itemClicked(var channelName, var stationName, var stationSId)
    signal setFavourite(var favourite, var channelName, var stationName, var stationSId)

    Rectangle {
        id: rectangle1
        color: "#212121"
        anchors.fill: parent
    }

    ListView {
        id: listView
        boundsBehavior: Flickable.DragOverBounds
        clip: true
        anchors.fill: parent

        delegate: StationListItem {
            onItemClicked: {
                __root.itemClicked(channelName, stationName, stationSId)
            }
            onSetFavourite: {
                __root.setFavourite(favorit, channelName, stationName, stationSId)
            }
        }
        ScrollBar.vertical: ThemeScrollBar {

        }
    }


}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
