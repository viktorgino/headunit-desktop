#include "androidauto.h"

AndroidAuto::AndroidAuto(QObject *parent) : QObject (parent)
{
    headunit =  new Headunit();
    m_pluginSettings.eventListeners = QStringList() << "UsbConnectionListenerPlugin::UsbDeviceAdded";
//    headunit->startHU();
}

QObject *AndroidAuto::getContextProperty(){
    return qobject_cast<QObject *>(headunit);
}

void AndroidAuto::eventMessage(QString id, QVariant message){
    if(id == "UsbConnectionListenerPlugin::UsbDeviceAdded"){
        qDebug () << "AndroidAuto event UsbConnectionListenerPlugin::UsbDeviceAdded : " << message;
        headunit->startHU();
    }
}
void AndroidAuto::init(){

}
AndroidAuto::~AndroidAuto() {
    delete(headunit);
    //delete(&surface);
}
