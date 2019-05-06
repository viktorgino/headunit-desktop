#include "usbconnectionlistenerplugin.h"

UsbConnectionListenerPlugin::UsbConnectionListenerPlugin(QObject *parent) : QObject (parent), connectionListener()
{
    connect(&connectionListener, &UsbConnectionListener::usbDeviceAdded, this, &UsbConnectionListenerPlugin::usbDeviceAdded);
    connect(&connectionListener, &UsbConnectionListener::usbDeviceRemoved, this, &UsbConnectionListenerPlugin::usbDeviceRemoved);

    QThreadPool::globalInstance()->start(&connectionListener);
}

QObject *UsbConnectionListenerPlugin::getContextProperty(){
    return new QObject();
}

QQuickImageProvider *UsbConnectionListenerPlugin::getImageProvider() {
    return nullptr;
}

QStringList UsbConnectionListenerPlugin::eventListeners(){
    return QStringList();
}

QStringList UsbConnectionListenerPlugin::events(){
    return QStringList() << "UsbDeviceAdded" << "UsbDeviceRemoved";
}

QStringList UsbConnectionListenerPlugin::actions() {
    return QStringList();
}

void UsbConnectionListenerPlugin::eventMessage(QString id, QString message){

}

void UsbConnectionListenerPlugin::actionMessage(QString id, QString message){

}

void UsbConnectionListenerPlugin::usbDeviceAdded(QString deviceDetails){
    emit onMessage("UsbDeviceAdded", deviceDetails);
    emit onMessage("GUI::Notification", deviceDetails);
}

void UsbConnectionListenerPlugin::usbDeviceRemoved(QString deviceDetails){
    emit onMessage("UsbDeviceRemoved", deviceDetails);
    emit onMessage("GUI::Notification", deviceDetails);
}

