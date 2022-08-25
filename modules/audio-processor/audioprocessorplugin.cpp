#include "audioprocessorplugin.h"

AudioProcessorPlugin::AudioProcessorPlugin(QObject *parent) : QObject (parent)
{

}

void AudioProcessorPlugin::init() {

}

void AudioProcessorPlugin::testNotification(){
    QVariantMap map;
    map["source"] = "qrc:/AudioProcessor/SoundSettings.qml";

    emit action("GUI::OpenOverlay", map);
}

void AudioProcessorPlugin::setAudioParameter(QString parameter, int value){
    qDebug() << parameter << value;
}
void AudioProcessorPlugin::closeOverlay(){
    emit action("GUI::CloseOverlay", "");
}

void AudioProcessorPlugin::onSettingsPageDestroyed() {
    qDebug() << "Sample plugin destroyed";
}

QObject *AudioProcessorPlugin::getContextProperty(){
    return this;
}
