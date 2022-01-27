#include "sampleplugin.h"

SamplePlugin::SamplePlugin(QObject *parent) : QObject (parent)
{

}

void SamplePlugin::init() {

}

void SamplePlugin::testNotification(){
    emit message("GUI::Notification", "{\"image\":\"qrc:/qml/icons/alert.png\", \"title\":\"Test notification\", \"description\":\"This is a test notification\"}");
}


void SamplePlugin::onSettingsPageDestroyed() {
    qDebug() << "Sample plugin destroyed";
}
