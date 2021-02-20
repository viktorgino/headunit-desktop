import QtQuick 2.11
import QtQml 2.11

Loader {
    source: HVACPlugin.hvacSettings.readOnly ? "qrc:/HVAC/ClimateControl/HVACBottomBarDisplay.qml" : "qrc:/HVAC/ClimateControl/HVACBottomBarControl.qml"
}
