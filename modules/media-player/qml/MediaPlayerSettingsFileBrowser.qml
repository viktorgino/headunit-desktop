import QtQuick 2.0

import HUDTheme 1.0
ThemeRoot {
    id : __root

    signal pop()
    FileBrowser {
        id: fileBrowser
        anchors.fill : parent
        folderSelectable:true
        showFiles:false
        onAccepted: {
            pluginContext.addLocation(fileBrowser.folder)
            __root.pop()
        }
        onRejected: {
            __root.pop()
        }
    }
}
