import QtQuick 2.11
import QtQml 2.11
import HUDTheme 1.0

Loader {
    property QtObject pluginContext : HUDPlugins.getPlugin("HVACPlugin").contextProperty

    source: pluginContext.hvacSettings.readOnly ? "qrc:/HVAC/ClimateControl/HVACBottomBarDisplay.qml" : "qrc:/HVAC/ClimateControl/HVACBottomBarControl.qml"
}
