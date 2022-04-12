#include "volumecontrol.h"

VolumeControl::VolumeControl(QObject *parent) : QObject (parent)
{
    qmlRegisterType<PulseAudioQt::CardModel>("HUDVolume", 1, 0, "CardModel");
    qmlRegisterType<PulseAudioQt::SinkModel>("HUDVolume", 1, 0, "SinkModel");
    qmlRegisterType<PulseAudioQt::SinkInputModel>("HUDVolume", 1, 0, "SinkInputModel");
    qmlRegisterType<PulseAudioQt::SourceModel>("HUDVolume", 1, 0, "SourceModel");
    qmlRegisterType<PulseAudioQt::SourceOutputModel>("HUDVolume", 1, 0, "SourceOutputModel");
    qmlRegisterType<PulseAudioQt::StreamRestoreModel>("HUDVolume", 1, 0, "StreamRestoreModel");
    qmlRegisterType<PulseAudioQt::ModuleModel>("HUDVolume", 1, 0, "ModuleModel");
}

void VolumeControl::init(){
    defaultSinkChanged();
    m_pluginSettings.actions = QStringList() << "VolumeUp" << "VolumeDown";
    connect(&m_sinkModel, &PulseAudioQt::SinkModel::defaultSinkChanged, this, &VolumeControl::defaultSinkChanged);
}

void VolumeControl::setDefaultVolume(int volume){
    m_settings["volume"] = volume;

    emit m_settings.valueChanged("volume", volume);
}

void VolumeControl::defaultSinkChanged() {
    PulseAudioQt::Sink * defaultSink = m_sinkModel.defaultSink();
    if( defaultSink != nullptr) {
        defaultSink->setVolume(m_settings["volume"].toInt());
        qDebug () << "Setting volume to : " << m_settings["volume"].toInt();
    }
}
QObject *VolumeControl::getContextProperty(){
    return this;
}

void VolumeControl::actionMessage(QString id, QVariant message){
    PulseAudioQt::Sink * defaultSink = m_sinkModel.defaultSink();
    if( defaultSink != nullptr) {
        int volume = 0;
        if(id == "VolumeUp"){
            volume = defaultSink->volume() + (655 * 4);
        } else if(id == "VolumeDown") {
            volume = defaultSink->volume() - (655 * 4);
        }
        if(volume > PulseAudioQt::normalVolume()){
            volume = PulseAudioQt::normalVolume();
        } else if (volume < PulseAudioQt::minimumVolume()) {
            volume = PulseAudioQt::minimumVolume();
        } else {
        }
        defaultSink->setVolume(volume);
        m_settings["volume"] = volume;

        emit m_settings.valueChanged("volume", volume);
    }
}
