#include "bluetooth.h"

PhoneBluetooth::PhoneBluetooth(QObject *parent)
{
    telephonyManager = new TelephonyManager();
}

QObject *PhoneBluetooth::getContextProperty(){
    return qobject_cast<QObject *>(telephonyManager);
}

QQuickImageProvider *PhoneBluetooth::getImageProvider() {
    return nullptr;
}

QStringList PhoneBluetooth::eventListeners(){
    return QStringList();
}

QStringList PhoneBluetooth::events(){
    return QStringList();
}

QStringList PhoneBluetooth::actions() {
    return QStringList();
}

void PhoneBluetooth::eventMessage(QString id, QString message){

}

void PhoneBluetooth::actionMessage(QString id, QString message){

}
