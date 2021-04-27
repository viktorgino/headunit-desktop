import QtQuick 2.0
import QtContacts 5.0
import QtGraphicalEffects 1.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

import HUDTheme 1.0

Item {
    id: __root
    property int contactCardHeight: 50
    property string dialed_num: ""
    property bool isSearching: (dialed_num.length > 0)
    signal dial(string number)
    clip: true

    StackView{
        id:contactView
        initialItem: browsingAreaLoader
        anchors.fill: parent
    }
    Timer {
        id: contactsTimer
        interval:2000; running: false; repeat: false
        onTriggered: browsingAreaLoader.active = true;
    }
    Connections{
        target: PhoneBluetooth
        onPhonebookChanged:{
            browsingAreaLoader.active = false;
            contactsTimer.restart();
        }
    }
    Loader{
        id:browsingAreaLoader
        sourceComponent: browsingArea
        active: true
    }

    Component {
        id:browsingArea
        ListView {
            id: contactsView
            model: ContactModel {
                id:contactsModel
                autoUpdate: true
                manager: "memory"
                filter:DetailFilter {
                    id:numberFilter
                    value:dialed_num
                    detail: ContactDetail.PhoneNumber
                    field:PhoneNumber.Number
                    matchFlags: Filter.MatchContains
                }
                Component.onCompleted: {
                    importContacts("file:"+PhoneBluetooth.contactsFolder+"/contacts.vcf")
                }

                sortOrders: [
                    SortOrder {
                        detail: ContactDetail.Name
                        field: Name.FirstName
                        direction: Qt.AscendingOrder
                    }
                ]
            }
            focus: true
            clip: true
            delegate: Item {
                id: delRect
                width: parent.width
                height: __root.contactCardHeight

                MouseArea {
                    id: mouseArea
                    anchors.fill: parent
                    onClicked: {
                        //__root.dial(contact)
                        if(__root.isSearching){
                            __root.dial(number.num)
                        } else {
                            contactView.push(contactInfo,{"contact":contact})
                        }
                    }
                }

                ThemeText {
                    id: name
                    anchors {
                        verticalCenter: parent.verticalCenter
                        left: imageWrapper.right
                        leftMargin: 6
                    }
                    text: contact.name.firstName + " " + contact.name.lastName
                    elide: Text.ElideRight
                    font.bold: true
                }
                Rectangle{
                    id: imageWrapper
                    width: height
                    radius: height/2
                    clip: true
                    anchors.top: parent.top
                    anchors.topMargin: 6
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 6
                    anchors.left: parent.left
                    anchors.leftMargin: 6
                    color:"#00BCD4"


                    Item {
                        anchors.fill: parent
                        layer.enabled: true
                        layer.effect: OpacityMask {
                            maskSource: imageWrapper
                        }

                        Text {
                            x: -height/5
                            color: "#66ffffff"
                            text: contact.name.firstName.charAt(0).toUpperCase()
                            font.pixelSize: parent.height * 1.4
                            anchors.bottomMargin: 0
                            anchors.top: parent.top
                            anchors.bottom: parent.bottom
                            anchors.topMargin: 0
                            font.bold: true
                            clip: true
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignLeft
                        }

                        Text {
                            x: height/4
                            color: "#4cffffff"
                            text: contact.name.lastName.charAt(0).toUpperCase()
                            font.pixelSize: parent.height * 1.4
                            anchors.bottom: parent.bottom
                            anchors.top: parent.top
                            font.bold: true
                            clip: true
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignRight
                        }

                        /*Image {
                            id: image
                            anchors.fill: parent
                            source: contact.avatar.imageUrl
                            fillMode: Image.PreserveAspectCrop
                        }*/
                    }

                }

                Text {
                    id: number
                    color: "#ffffff"
                    property string num : {
                        if(__root.isSearching){
                            for(var i = 0; contact.phoneNumbers.length > i; i++){
                                if(String(contact.phoneNumbers[i].number).includes(__root.dialed_num)){
                                    return contact.phoneNumbers[i].number;
                                }
                            }
                        }
                        return "";
                    }
                    text: {
                        var numIndex = num.indexOf(__root.dialed_num);
                        return num.substring(0, numIndex) + "<b>" + __root.dialed_num + "</b>" + num.substring(numIndex+__root.dialed_num.length, num.length);
                    }

                    anchors.top: name.bottom
                    anchors.topMargin: 0
                    anchors.right: parent.right
                    anchors.rightMargin: 8
                    anchors.left: imageWrapper.right
                    anchors.leftMargin: 8
                    font.pointSize: parent.height * 0.15
                }

            }
        }
    }

    Component {
        id: contactInfo
        Item {
            id: contactInfoItem
            property Contact contact: Contact {

            }
            Rectangle {
                color:"#ffffff"
                anchors.fill: parent
            }

            Rectangle {
                id: contactDetails
                color:"#00BCD4"
                clip: true
                height: parent.height*0.5
                anchors.top: parent.top
                anchors.topMargin: 0
                anchors.right: parent.right
                anchors.rightMargin: 0
                anchors.left: parent.left
                anchors.leftMargin: 0
                Text {
                    x: -height/4
                    color: "#66ffffff"
                    text: contact.name.firstName.charAt(0).toUpperCase()
                    font.pixelSize: parent.height * 2
                    anchors.bottomMargin: 0
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    anchors.topMargin: 0
                    font.bold: true
                    clip: true
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                }

                Text {
                    x: height/2
                    color: "#4cffffff"
                    text: contact.name.lastName.charAt(0).toUpperCase()
                    font.pixelSize: parent.height * 2
                    anchors.bottom: parent.bottom
                    anchors.top: parent.top
                    font.bold: true
                    clip: true
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignRight
                }
                Image {
                    id: image1
                    anchors.fill: parent
                    fillMode: Image.PreserveAspectCrop
                    source: contact.avatar.imageUrl
                }

                Text {
                    id: text1
                    color: "#ffffff"
                    text: contact.name.firstName + " " + contact.name.lastName
                    elide: Text.ElideLeft
                    font.pixelSize: parent.height * 0.1
                    anchors.leftMargin: parent.width * 0.05
                    anchors.bottomMargin: parent.height * 0.1
                    anchors.left: parent.left
                    anchors.bottom: parent.bottom
                }
            }

            ListView {
                clip: true
                anchors.top: contactDetails.bottom
                anchors.topMargin: 0
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 0
                anchors.left: parent.left
                anchors.leftMargin: 0
                anchors.right: parent.right
                anchors.rightMargin: 0
                model: contact.phoneNumbers
                delegate: Item{
                    height: 100
                    width: parent.width
                    Image {
                        id: image
                        width: height
                        anchors.leftMargin: 8
                        anchors.bottomMargin: 8
                        anchors.topMargin: 8
                        anchors.bottom: parent.bottom
                        anchors.left: parent.left
                        anchors.top: parent.top
                        source: "qrc:/qml/icons/svg/android-call.svg"
                    }

                    ColorOverlay {
                        anchors.fill: image
                        source: image
                        color: "#000000"
                    }

                    Item {
                        anchors.right: parent.right
                        anchors.rightMargin: 8
                        anchors.left: image.right
                        anchors.leftMargin: 8
                        anchors.top: parent.top
                        anchors.topMargin: 8
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: 8

                        Text {
                            id: number
                            height: parent.height/2
                            text: modelData.number
                            anchors.right: parent.right
                            anchors.left: parent.left
                            anchors.top: parent.top
                            font.pixelSize: parent.height * 0.4
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignLeft
                        }

                        Text {
                            id: type
                            text: {
                                switch(modelData.subTypes[0]){
                                case PhoneNumber.Mobile :
                                    return qsTr("Mobile")
                                case PhoneNumber.Landline :
                                    return qsTr("Landline")
                                case PhoneNumber.Fax :
                                    return qsTr("Fax")
                                case PhoneNumber.Unknown :
                                default:
                                    return qsTr("Other")
                                }
                            }
                            anchors.topMargin: 0
                            anchors.bottom: parent.bottom
                            anchors.bottomMargin: 0
                            font.pixelSize: parent.height * 0.3
                            horizontalAlignment: Text.AlignLeft
                            anchors.right: parent.right
                            verticalAlignment: Text.AlignVCenter
                            anchors.top: number.bottom
                            anchors.left: parent.left
                        }
                    }

                    MouseArea {
                        id: mouseArea
                        anchors.fill: parent
                        onClicked: {
                            __root.dial(modelData.number)
                        }
                    }
                }
            }

            Button {
                id: button
                text: qsTr("Back")
                anchors.top: parent.top
                anchors.topMargin: 8
                anchors.left: parent.left
                anchors.leftMargin: 8
                onClicked: {
                    contactView.pop()
                }
            }

        }
    }
}
