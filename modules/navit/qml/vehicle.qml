import QtQuick 2.0
import com.navit.gui 1.0

Item {
    Text {
        x: 8
        y: 8
        color: "#ffffff"
        text: NavitGUI.currentVehicle.name
        font.pixelSize: 32
    }
}
