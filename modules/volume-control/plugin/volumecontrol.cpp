#include "volumecontrol.h"

VolumeControl::VolumeControl(QObject *parent) : QObject (parent)
{

}

QObject *VolumeControl::getContextProperty(){
    return nullptr;
}

QQuickImageProvider *VolumeControl::getImageProvider() {
    return nullptr;
}

QStringList VolumeControl::eventListeners(){
    return QStringList();
}

QStringList VolumeControl::events(){
    return QStringList();
}

QStringList VolumeControl::actions() {
    return QStringList();
}

void VolumeControl::eventMessage(QString id, QString message){

}

void VolumeControl::actionMessage(QString id, QString message){

}
