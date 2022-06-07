import QtQuick 2.11
import QtQuick.Controls 2.4

import HUDTheme 1.0

Item {
    id: __now_playing_list
    clip: true
    property alias model : listView.model
    property url icon
    property string name
    property string item_type
    property int currentPlaying : 0
    signal itemClicked(int index)
    onCurrentPlayingChanged: {
        listView.positionViewAtIndex(currentPlaying,ListView.Beginning);
    }

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

        delegate: NowPlayingListItem {
            width: listView.width
            currentPlaying: __now_playing_list.currentPlaying
            onItemClicked: __now_playing_list.itemClicked(index)
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
