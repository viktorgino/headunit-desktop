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

}
QObject *VolumeControl::getContextProperty(){
    return this;
}

