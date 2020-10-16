import QtQuick 2.11
import QtQuick.Controls 2.4
import QtGraphicalEffects 1.0
import Qt.labs.folderlistmodel 2.1
import Qt.labs.platform 1.0

Item {
    id: root
    property alias folder: folderModel.folder
    function reset (){
        folderModel.folder = "file://"+StandardPaths.standardLocations(StandardPaths.HomeLocation)
    }

    Item {
        id: standardLocations
        width: parent.width*0.3
        anchors.top: parent.top
        anchors.topMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0

        ListView {
            id: listView1
            anchors.bottom: divider.top
            anchors.bottomMargin: 20
            clip: true
            anchors.top: parent.top
            anchors.topMargin: 8
            anchors.left: parent.left
            anchors.leftMargin: 8
            anchors.right: parent.right
            anchors.rightMargin: 8
            delegate: Item {
                width: parent.width
                height: 40
                Row {
                    Image {
                        width: 20
                        height: 20
                        source: iconImage
                        mipmap:true
                        ColorOverlay {
                            color:"#424242"
                            anchors.fill: parent
                            enabled: true
                            source: parent
                        }
                    }
                    Text {
                        text: StandardPaths.displayName(path_key)
                        anchors.verticalCenter: parent.verticalCenter
                        font.bold: true
                    }
                    spacing: 10
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: folderModel.folder = "file://"+StandardPaths.standardLocations(path_key)
                }
            }
            model: ListModel {
                ListElement {
                    path_key: 8 //HomeLocation
                    iconImage: "qrc:/qml/icons/home.png"
                }
                ListElement {
                    path_key: 0 //DesktopLocation
                    iconImage: "qrc:/qml/icons/folder.png"
                }
                ListElement {
                    path_key: 1 //DocumentsLocation
                    iconImage: "qrc:/qml/icons/document.png"
                }
                ListElement {
                    path_key: 14 //DownloadLocation
                    iconImage: "qrc:/qml/icons/android-download.png"
                }
                ListElement {
                    path_key: 4 //MusicLocation
                    iconImage: "qrc:/qml/icons/music-note.png"
                }
                ListElement {
                    path_key: 5 //MoviesLocation
                    iconImage: "qrc:/qml/icons/videocamera.png"
                }
            }

            ScrollBar.vertical: ScrollBar {
            }
        }
        Rectangle {
            id: divider
            height: 1
            color: "#424242"
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.right: parent.right
        }

        ListView {
            id: listView2
            clip: true
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 8
            anchors.top: divider.bottom
            anchors.topMargin: 20
            anchors.left: parent.left
            anchors.leftMargin: 8
            anchors.right: parent.right
            anchors.rightMargin: 8
            delegate:Item {
                height:40
                anchors.left: parent.left
                anchors.right: parent.right
                Text {
                    text: modelData.name
                    elide: Text.ElideLeft
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                    font.bold: true
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: folderModel.folder = "file://"+modelData.path
                }
            }
            model: MediaPlayerPlugin.getMountedVolumes()

            ScrollBar.vertical: ScrollBar {
            }
        }


        Rectangle {
            id: rectangle
            width: 1
            color: "#424242"
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.right: parent.right
        }


    }

    Item {
        id: item1
        anchors.rightMargin: 8
        anchors.bottomMargin: 8
        anchors.topMargin: 8
        anchors.left: standardLocations.right
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.top: parent.top
        anchors.leftMargin: 8

        Item {
            id: dir_up
            height: 80
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            Item {
                id: item2
                height: 40
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.right: parent.right
                Text {
                    text: folderModel.folder.toString().replace(/^(file:\/{2})|(qrc:\/{1})/, "")
                    elide: Text.ElideLeft
                    anchors.left: parent.left
                    anchors.leftMargin: 0
                    anchors.right: parent.right
                    anchors.rightMargin: 0
                    anchors.verticalCenter: parent.verticalCenter
                    font.bold: true
                }
            }
            Item {
                height: 40
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                Image {
                    x:5
                    width: 20
                    height: 20
                    source: "qrc:/qml/icons/chevron-up.png"
                    anchors.verticalCenter: parent.verticalCenter
                    mipmap: true
                    ColorOverlay {
                        color:"#424242"
                        anchors.fill: parent
                        enabled: true
                        source: parent
                    }
                }
                Text {
                    x:35
                    text: "Up"
                    anchors.verticalCenter: parent.verticalCenter
                    font.bold: true
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: folderModel.parentFolder != folderModel.rootFolder?folderModel.folder = folderModel.parentFolder:null
                }
            }
        }

        ListView {
            id: listView
            clip: true
            anchors.top: dir_up.bottom
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.topMargin: 0
            delegate: Item {
                height: 40
                width:parent.width
                Image {
                    x:5
                    width: 20
                    height: 20
                    id: folder_image
                    source: "qrc:/qml/icons/android-folder.png"
                    anchors.verticalCenter: parent.verticalCenter
                    mipmap: true
                    ColorOverlay {
                        color:"#424242"
                        anchors.fill: parent
                        enabled: true
                        source: parent
                    }
                }
                Text {
                    x:35
                    text: fileName
                    anchors.verticalCenter: parent.verticalCenter
                    font.bold: true
                }
                MouseArea {
                    id: mouseArea
                    anchors.fill: parent
                    onClicked: folderModel.folder = fileURL
                }
            }
            FolderListModel {
                id: folderModel
                showFiles: false
                showHidden: false
                showDotAndDotDot: false
                showOnlyReadable: true
                folder:"file://"+StandardPaths.standardLocations(StandardPaths.HomeLocation)
            }
            ScrollBar.vertical: ScrollBar {
            }
            model: folderModel
        }

    }


}
