import QtQuick 2.0
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
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: tabsWrapper.top
        anchors.bottomMargin: 8
        initialItem: callHistory
    }
    RowLayout {
        id: tabsWrapper
        height: 35
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        spacing: 8
        anchors.bottomMargin: 8
        anchors.leftMargin: 8
        anchors.rightMargin: 8

        Rectangle {
            id: rectangle
            color: HUDStyle.colors.formBox
            Layout.fillHeight: true
            Layout.fillWidth: true
            ImageButton {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.bottomMargin: parent.height * 0.2
                anchors.topMargin: parent.height * 0.2
                imageSource: "icons/history.svg"
                onClicked: {
                    contactView.pop()
                    contactView.push(callHistory)
                }
            }
        }

        Rectangle {
            id: rectangle1
            color: HUDStyle.colors.formBox
            Layout.fillWidth: true
            Layout.fillHeight: true
            ImageButton {
                id: imageButton
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.leftMargin: 0
                anchors.topMargin: parent.height * 0.2
                anchors.bottomMargin: parent.height * 0.2
                imageSource: "icons/contacts.svg"

                onClicked: {
                    contactView.pop()
                    contactView.push(browsingArea)
                }
            }
        }
    }

    Component {
        id:browsingArea
        ContactList{
            height: contactView.height
            width: contactView.width
            onContactClicked: {
                contactView.push(contactInfo,{"name":name, "numbers" : numbers, "image" : image})
            }
        }
    }

    Component {
        id: contactInfo
        ContactDetails{

        }
    }

    Component {
        id: callHistory
        CallHistoryList{

        }
    }

}

/*##^##
Designer {
    D{i:0;autoSize:true;formeditorColor:"#808080";height:480;width:640}D{i:6}
}
##^##*/
