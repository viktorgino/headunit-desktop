import QtQuick 2.0

import HUDTheme 1.0
Item {
    id : __root

    signal pop()
    FileBrowser {
        id: fileBrowser
        anchors.fill : parent
        folderSelectable:true
        showFiles:false
        onAccepted: {
            MediaPlayerPlugin.addLocation(fileBrowser.folder)
            __root.pop()
        }
        onRejected: {
            __root.pop()
        }
    }
}
