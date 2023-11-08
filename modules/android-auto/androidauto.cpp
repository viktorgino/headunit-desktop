#include "androidauto.h"

AndroidAutoPlugin::AndroidAutoPlugin(QObject* parent)
    : QObject(parent)
    , m_headunit(this)
    , m_bluetoothServer(this)
    , m_bluetoothService(this)
{
    m_pluginSettings.eventListeners = QStringList() << "UsbConnectionListenerPlugin::UsbDeviceAdded" << "SYSTEM::SetNightMode";
    m_pluginSettings.events = QStringList() << "connected";
    gst_init(NULL, NULL);

    m_interfaceSettings.mediaStream = true;
    m_interfaceSettings.voiceStream = true;

    connect(&m_headunit, &HeadunitVideoSource::playbackStarted, this, &AndroidAutoPlugin::playbackStarted);
    connect(&m_headunit, &HeadunitVideoSource::statusChanged, this, &AndroidAutoPlugin::huStatusChanged);
}

QObject *AndroidAutoPlugin::getContextProperty(){
    return qobject_cast<QObject*>(&m_headunit);
}

void AndroidAutoPlugin::eventMessage(QString id, QVariant message){
    if(id == "UsbConnectionListenerPlugin::UsbDeviceAdded"){
        if (m_headunit.status() != HeadunitVideoSource::RUNNING) {
            m_headunit.startHU();
        }
    } else if(id == "UsbConnectionListenerPlugin::UsbDeviceRemoved"){
    } else if(id == "SYSTEM::SetNightMode"){
        m_headunit.setNigthmode(message.toBool());
    }
}
void AndroidAutoPlugin::init(){
    m_headunit.init();
    m_headunit.startHU();
}

void AndroidAutoPlugin::start() {
    m_headunit.startMedia();
}
void AndroidAutoPlugin::stop() {
    m_headunit.stopMedia();
}
void AndroidAutoPlugin::prevTrack() {
    m_headunit.prevTrack();
}
void AndroidAutoPlugin::nextTrack() {
    m_headunit.nextTrack();
}
void AndroidAutoPlugin::setMediaVolume(uint8_t volume) {
    m_headunit.setMediaVolume(volume);
}
void AndroidAutoPlugin::setVoiceVolume(uint8_t volume) {
    m_headunit.setVoiceVolume(volume);
}

void AndroidAutoPlugin::huStatusChanged(){
    emit message("connected", (m_headunit.status() > HeadunitVideoSource::NO_CONNECTION));
}
