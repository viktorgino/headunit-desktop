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

void UsbConnectionListenerPlugin::usbDeviceAdded(QVariantMap deviceDetails){
    QJsonDocument json = QJsonDocument::fromVariant(deviceDetails);
    emit onMessage("UsbDeviceAdded", json.toJson(QJsonDocument::Compact));

    QVariantMap notificationDetails;
    QString notificationText = QString("%1 %2").arg(deviceDetails.value("manufacturer").toString())
                                               .arg(deviceDetails.value("product").toString());
    notificationDetails.insert("description", notificationText);
    notificationDetails.insert("title", "New USB device connected");
    notificationDetails.insert("image", "qrc:/qml/icons/usb.png");

    json = QJsonDocument::fromVariant(notificationDetails);

    emit onMessage("GUI::Notification", json.toJson(QJsonDocument::Compact));
}

void UsbConnectionListenerPlugin::usbDeviceRemoved(QVariantMap deviceDetails){
    QJsonDocument json = QJsonDocument::fromVariant(deviceDetails);
    emit onMessage("UsbDeviceRemoved", json.toJson(QJsonDocument::Compact));
}

