#include "hvacplugin.h"

HVACPlugin::HVACPlugin(QObject *parent) : QObject (parent)
{
    loadHVAC("modules/hvac/cars/sample/config.ini");
}

void HVACPlugin::loadHVAC(QString settingsFile){
    QFileInfo settingsFileInfo(settingsFile);
    if(settingsFileInfo.exists()){
        carPlatformSettings = new QSettings(settingsFileInfo.absoluteFilePath(), QSettings::IniFormat);
        QStringList settingsGroups = carPlatformSettings->childGroups();

        QMap<QString,QMap<QString,QVariant>> settings;
        foreach (QString group, settingsGroups){
            carPlatformSettings->beginGroup(group);
            QMap<QString,QVariant> keys;
            foreach (QString key, carPlatformSettings->childKeys()){
                keys.insert(key,carPlatformSettings->value(key));
            }
            settings.insert(group, keys);
        }
        m_hvac = new HVACController(settingsFileInfo.dir(), settings.value("HVAC").toStdMap(), this);
    } else {
        qDebug() << "Car settings file doesn't exists!" << settingsFileInfo.absoluteFilePath();
    }
}

QObject *HVACPlugin::getContextProperty(){
    return m_hvac;
}

QQuickImageProvider *HVACPlugin::getImageProvider() {
    return nullptr;
}

QStringList HVACPlugin::eventListeners(){
    return QStringList();
}

QStringList HVACPlugin::events(){
    return QStringList();
}

QStringList HVACPlugin::actions() {
    return QStringList();
}

void HVACPlugin::eventMessage(QString /*id*/, QString /*message*/){

}

void HVACPlugin::actionMessage(QString /*id*/, QString /*message*/){

}

HVACPlugin::~HVACPlugin(){
    qDebug() << "Deleting HVACPlugin";
}
