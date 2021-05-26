#include "androidauto.h"


AndroidAuto::AndroidAuto(QObject *parent) : QObject (parent)
{
    m_pluginSettings.eventListeners = QStringList() << "UsbConnectionListenerPlugin::UsbDeviceAdded";
    m_pluginSettings.events = QStringList() << "connected";
    gst_init(NULL, NULL);
    headunit = new Headunit();
    m_interfaceSettings.mediaStream = true;
    m_interfaceSettings.voiceStream = true;

    connect(headunit, &Headunit::playbackStarted, this, &AndroidAuto::playbackStarted);
    connect(headunit, &Headunit::statusChanged, this, &AndroidAuto::huStatusChanged);
}

QObject *AndroidAuto::getContextProperty(){
    return qobject_cast<QObject *>(headunit);
}

void AndroidAuto::eventMessage(QString id, QVariant message){
    if(id == "UsbConnectionListenerPlugin::UsbDeviceAdded"){
        if(headunit->status() != Headunit::RUNNING){
            headunit->startHU();
        }
    } else if(id == "UsbConnectionListenerPlugin::UsbDeviceRemoved"){
    }
}
void AndroidAuto::init(){
    headunit->init();
    headunit->startHU();
}

void AndroidAuto::start() {
    headunit->startMedia();
}
void AndroidAuto::stop() {
    headunit->stopMedia();
}
void AndroidAuto::prevTrack() {
    headunit->prevTrack();
}
void AndroidAuto::nextTrack() {
    headunit->nextTrack();
}
void AndroidAuto::setMediaVolume(uint8_t volume) {
    headunit->setMediaVolume(volume);
}
void AndroidAuto::setVoiceVolume(uint8_t volume) {
    headunit->setVoiceVolume(volume);
}

void AndroidAuto::huStatusChanged(){
    emit message("connected", (headunit->status() > Headunit::NO_CONNECTION));
}
