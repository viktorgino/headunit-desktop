import QtQuick 2.0
import QtQuick.Layouts 1.0

GridLayout {
    id: gridLayout
    anchors.fill: parent
    columnSpacing: 4
    rowSpacing: 4
    rows: 3
    columns: 1

    MainButton {
        id: townSearchButton
        height: 100
        text: NavitGUI.currentTown
        Layout.fillWidth: true
        onClicked: {
            NavitGUI.setSearchContext('town')
            menucontent.source = "search.qml"
        }
    }

    MainButton {
        id: streetSearchButton
        height: 100
        text: NavitGUI.currentStreet
        Layout.fillWidth: true
        onClicked: {
            NavitGUI.setSearchContext('street')
            menucontent.source = "search.qml"
        }
    }

    MainButton {
        id: countrySearchButton
        height: 100
        text: NavitGUI.currentCountry
        icon: NavitGUI.get_icon_path() + 'country_' + NavitGUI.currentCountryIso2 + ".svg"
        Layout.fillWidth: true
        onClicked: {
            NavitGUI.setSearchContext('country')
            menucontent.source = "search.qml"
        }
    }
}
