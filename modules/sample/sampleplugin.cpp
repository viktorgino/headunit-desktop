#include "sampleplugin.h"

SamplePlugin::SamplePlugin(QObject *parent) : QObject (parent)
{

}

QObject *SamplePlugin::getContextProperty(){
    return this;
}

QQuickImageProvider *SamplePlugin::getImageProvider() {
    return nullptr;
}

QStringList SamplePlugin::eventListeners(){
    return QStringList();
}

QStringList SamplePlugin::events(){
    return QStringList();
}

QStringList SamplePlugin::actions() {
    return QStringList();
}

void SamplePlugin::eventMessage(QString /*id*/, QString /*message*/){

}

void SamplePlugin::actionMessage(QString /*id*/, QString /*message*/){

}

void SamplePlugin::testNotification(){
    emit onMessage("GUI::Notification", "{\"image\":\"qrc:/qml/icons/alert.png\", \"title\":\"Test notification\", \"description\":\"This is a test notification\"}");
}
