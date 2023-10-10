#include "gpsdplugin.h"
#include <math.h>

GPSDPlugin::GPSDPlugin(QObject *parent) : QObject (parent)
{
    //m_pluginSettings.events = QStringList() << "MediaInput::position";
}

void GPSDPlugin::init() {
    m_host = m_settings.value("host").toString();
    m_port = m_settings.value("port").toUInt();
}

QObject *GPSDPlugin::getContextProperty(){
    return this;
}

void GPSDPlugin::eventMessage(QString id, QVariant message) {
}

void GPSDPlugin::settingsChanged(const QString &key, const QVariant &){
    if(key == "host"){
    }
    else if (key == "port") {
    }
}

void GPSDPlugin::PrintString(char *message, int length) {
    qDebug() << "GPSD DEBUG : " << message;
}
