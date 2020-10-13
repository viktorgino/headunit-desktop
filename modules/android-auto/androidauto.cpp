#include "androidauto.h"

AndroidAuto::AndroidAuto(QObject *parent) : QObject (parent)
{
    headunit =  new Headunit();
//    headunit->startHU();
}

QObject *AndroidAuto::getContextProperty(){
    return qobject_cast<QObject *>(headunit);
}

QQuickImageProvider *AndroidAuto::getImageProvider() {
    return nullptr;
}

QStringList AndroidAuto::eventListeners(){
    return QStringList() << "UsbConnectionListenerPlugin::UsbDeviceAdded";
}

QStringList AndroidAuto::events(){
    return QStringList();
}

QStringList AndroidAuto::actions() {
    return QStringList();
}

void AndroidAuto::eventMessage(QString id, QString message){
    if(id == "UsbConnectionListenerPlugin::UsbDeviceAdded"){
        qDebug () << "AndroidAuto event UsbConnectionListenerPlugin::UsbDeviceAdded : " << message;
        headunit->startHU();
    }
}

void AndroidAuto::actionMessage(QString id, QString message){

}
AndroidAuto::~AndroidAuto() {
    delete(headunit);
    //delete(&surface);
}
