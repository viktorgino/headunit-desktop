#include "audioprocessorplugin.h"

AudioProcessorPlugin::AudioProcessorPlugin(QObject *parent) : QObject(parent), m_overlayTimeout(this)
{
}

void AudioProcessorPlugin::init()
{
    m_audioBackend = new TDA7418();
    connect(&m_settings, &QQmlPropertyMap::valueChanged, this, &AudioProcessorPlugin::settingsChanged);
    connect(&m_overlayTimeout, &QTimer::timeout, this, &AudioProcessorPlugin::closeOverlay);
    m_overlayTimeout.setSingleShot(true);

    m_pluginSettings.actions = QStringList() << "Sound" << "TuneUp" << "TuneDown" << "VolumeUp" << "VolumeDown";
    QStringList audioParameters = QStringList() << "volume" << "sub" << "bass" << "middle" << "treble" << "balance";
    for(QString audioParameter : audioParameters){
        setAudioParameter(audioParameter, m_settings[audioParameter].toInt());
    }
}

void AudioProcessorPlugin::actionMessage(QString id, QVariant message){
    if(id == "Sound"){
        if(m_soundSettingsState == DefaultState){
            openOverlay();
        }
        
        m_overlayTimeout.start(5000);
        
        if(m_soundSettingsState < Treble){
            m_soundSettingsState = m_soundSettingsState + 1;
        } else {
            m_soundSettingsState = Balance;
        }
        
        emit settingsStateChanged();
    } else if(id == "TuneUp" || id == "TuneDown") {
        QString key;
        int value;
        switch(m_soundSettingsState){
            case Balance:
                key = "balance";
                break;
            case Sub:
                key = "sub";
                break;
            case Bass:
                key = "bass";
                break;
            case Middle:
                key = "middle";
                break;
            case Treble:
                key = "treble";
                break;
            default:
                return;
        }
        if(id == "TuneUp") {
            value = m_settings[key].toInt() + 1;
        } else {
            value = m_settings[key].toInt() - 1;
        }
        m_settings[key] = value;
        emit m_settings.valueChanged(key, value);
    } else if(id == "VolumeUp" || id == "VolumeDown") {
        int value;
        if(id == "VolumeUp") {
            value = m_settings["volume"].toInt() + 1;
        } else {
            value = m_settings["volume"].toInt() - 1;
        }
        if(value > 50){
            value = 50;
        }
        if(value < 0){
            value = 0;
        }
        m_settings["volume"] = value;
        emit m_settings.valueChanged("volume", value);
        QVariantMap map;
        map["source"] = "qrc:/AudioProcessor/Volume.qml";

        emit action("GUI::OpenOverlay", map);
    }
}

void AudioProcessorPlugin::settingsChanged(const QString &key, const QVariant &value){
    // if(key == "volume"){
    //     m_audioBackend->setVolume(value.toInt());
    // } else {
    //     qDebug() << "Settings changed" << key << value;
    // }
    setAudioParameter(key, value.toInt());
}

void AudioProcessorPlugin::openOverlay()
{
    QVariantMap map;
    map["source"] = "qrc:/AudioProcessor/SoundSettings.qml";

    emit action("GUI::OpenOverlay", map);
}

void AudioProcessorPlugin::setAudioParameter(QString parameter, int value)
{
    if(!m_audioBackend){
        qWarning() << "Audio processor doesn't exist";
        return;
    }

    if(parameter == "volume"){
        if(value > 50 || value < 0){
            return;
        }
        m_audioBackend->setVolume(50 + value);
    } else if(parameter == "balance"){
        int leftLevel = 100, rightLevel = 100;
        if (value < -15 || value > 15) {
            return;
        }
        if (value < 0){
            leftLevel = 100 + value;
        } else if (value > 0) {
            rightLevel = 100 - value;
        }
        m_audioBackend->setOutputChannelLevel(AudioProcessorInterface::Front_Left, leftLevel);
        m_audioBackend->setOutputChannelLevel(AudioProcessorInterface::Front_Right, rightLevel);
    } else if(parameter == "sub"){
        if(value > 15 || value < -100){
            return;
        }
        m_audioBackend->setOutputChannelLevel(AudioProcessorInterface::Subwoofer, 100 + value);        
    } else if(parameter == "bass"){
        if (value < -15 || value > 15) {
            return;
        }
        m_audioBackend->setEqBandLevel(AudioProcessorInterface::EQ_Band_Bass, value);
    } else if(parameter == "middle"){
        if (value < -15 || value > 15) {
            return;
        }
        m_audioBackend->setEqBandLevel(AudioProcessorInterface::EQ_Band_Middle, value);
    } else if(parameter == "treble"){
        if (value < -15 || value > 15) {
            return;
        }
        m_audioBackend->setEqBandLevel(AudioProcessorInterface::EQ_Band_Treble, value);
    } else {
        return;
    }
    qDebug() << "Set audio parameter" << parameter << value;
    
    m_overlayTimeout.start(5000);
    
    // m_settings[parameter] = value;
    // emit m_settings.valueChanged(parameter, value);
}
void AudioProcessorPlugin::closeOverlay()
{
    m_overlayTimeout.stop();
    m_soundSettingsState = DefaultState;
    emit settingsStateChanged();
    emit action("GUI::CloseOverlay", QVariant());
}

void AudioProcessorPlugin::onSettingsPageDestroyed()
{
    qDebug() << "Audio Processor destroyed";
}

QObject *AudioProcessorPlugin::getContextProperty()
{
    return this;
}
