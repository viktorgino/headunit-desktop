#include "androidauto.h"


AndroidAuto::AndroidAuto(QObject *parent) : QObject (parent)
{
    m_pluginSettings.eventListeners = QStringList() << "UsbConnectionListenerPlugin::UsbDeviceAdded";
    gst_init(NULL, NULL);
    headunit = new Headunit();
}

QObject *AndroidAuto::getContextProperty(){
    return qobject_cast<QObject *>(headunit);
}

void AndroidAuto::eventMessage(QString id, QVariant message){
    if(id == "UsbConnectionListenerPlugin::UsbDeviceAdded"){
        qDebug () << "AndroidAuto event UsbConnectionListenerPlugin::UsbDeviceAdded : " << message;
        headunit->startHU();
    } else if(id == "UsbConnectionListenerPlugin::UsbDeviceRemoved"){
        qDebug () << "AndroidAuto event UsbConnectionListenerPlugin::UsbDeviceRemoved : " << message;
    }
}
void AndroidAuto::init(){
    headunit->init();
    headunit->startHU();
}
