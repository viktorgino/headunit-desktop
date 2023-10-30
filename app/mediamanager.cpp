#include "mediamanager.h"

MediaManager::MediaManager(QObject *parent) : QObject(parent), m_mediaVolumes(new QQmlPropertyMap()), m_voiceVolumes(new QQmlPropertyMap())
{
    QSettings settings;
    m_activeMediaPlayer = settings.value("MediaManager/ActiveMediaPlayer").toString();

    connect(m_mediaVolumes, &QQmlPropertyMap::valueChanged, this, &MediaManager::setMediaVolume);
    connect(m_voiceVolumes, &QQmlPropertyMap::valueChanged, this, &MediaManager::setVoiceVolume);
}

void MediaManager::init () {
    for(QString interface : m_mediaVolumes->keys()){
        if(m_mediaInterfaces.contains(interface)){
            m_mediaInterfaces[interface]->setMediaVolume(m_mediaVolumes->value(interface).toInt());
        }
    }

    for(QString interface : m_voiceVolumes->keys()){
        if(m_mediaInterfaces.contains(interface)){
            m_mediaInterfaces[interface]->setVoiceVolume(m_voiceVolumes->value(interface).toInt());
        }
    }
}

void MediaManager::addInterface(QString name, QObject *object){
    MediaInterface * mediaInterface = dynamic_cast<MediaInterface *>(object);
    if(mediaInterface){
        m_mediaInterfaces[name] = mediaInterface;
        emit intefacesChanged();

        QSettings settings;

        if(mediaInterface->getSettings().mediaStream) {
            m_mediaVolumes->insert(name, settings.value("MediaManager/MediaVolumes" + name, 100).toInt());
        }
        if(mediaInterface->getSettings().voiceStream) {
            m_voiceVolumes->insert(name, settings.value("MediaManager/VoiceVolumes" + name, 100).toInt());
        }

        const QMetaObject *pluginMeta = object->metaObject();

        for(int i = pluginMeta->methodOffset(); i < pluginMeta->methodCount(); ++i){
            if(pluginMeta->method(i).methodSignature() == "playbackStarted()"){
                connect(object, SIGNAL(playbackStarted()), this, SLOT(playbackStartedHandler()));
            } else if(pluginMeta->method(i).methodSignature() == "trackChanged(QVariantMap)") {
                connect(object, SIGNAL(trackChanged(QVariantMap)), this, SLOT(onTrackChanged(QVariantMap)));
            } else if(pluginMeta->method(i).methodSignature() == "mediaPositionChanged(uint)") {
                connect(object, SIGNAL(mediaPositionChanged(uint)), this, SLOT(onMediaPositionChanged(uint)));
            }
        }
    }
}

void MediaManager::onTrackChanged(QVariantMap track) {
    emit message("MediaInput", "track", track);
}

void MediaManager::onMediaPositionChanged(quint32 position) {
    emit message("MediaInput", "position", position);
}

void MediaManager::playbackStartedHandler(){
    MediaInterface * sender = dynamic_cast<MediaInterface *>(QObject::sender());
    if(sender){
        QString senderName = m_mediaInterfaces.key(sender);
        setActiveMediaPlayer(senderName);
    }
}

void MediaManager::setActiveMediaPlayer(QString name){
    if(m_activeMediaPlayer != name) {
        m_activeMediaPlayer = name;

        QSettings settings;
        settings.setValue("MediaManager/ActiveMediaPlayer", m_activeMediaPlayer);

        for(QString interfaceName : m_mediaInterfaces.keys()) {
            if(name != interfaceName){
                m_mediaInterfaces[interfaceName]->stop();
            }
        }

        emit activeMediaPlayerChanged();
    }
}

void MediaManager::setVoiceVolume(QString interface, QVariant value){
    if(m_mediaInterfaces.contains(interface)){
        m_mediaInterfaces[interface]->setVoiceVolume(value.toInt());
        QSettings settings;
        settings.setValue("MediaManager/VoiceVolumes" + interface, value.toInt());
    }
}
void MediaManager::setMediaVolume(QString interface, QVariant value){
    if(m_mediaInterfaces.contains(interface)){
        m_mediaInterfaces[interface]->setMediaVolume(value.toInt());
        QSettings settings;
        settings.setValue("MediaManager/MediaVolumes" + interface, value.toInt());
    }
}

void MediaManager::mediaInput(QString input){
    if(input == "Next"){
        nextTrack();
    } else if(input == "Previous"){
        prevTrack();
    }
}
void MediaManager::start(){
    if(m_mediaInterfaces.contains(m_activeMediaPlayer)){
        m_mediaInterfaces[m_activeMediaPlayer]->start();
    }
}
void MediaManager::stop(){
    if(m_mediaInterfaces.contains(m_activeMediaPlayer)){
        m_mediaInterfaces[m_activeMediaPlayer]->stop();
    }
}
void MediaManager::prevTrack(){
    if(m_mediaInterfaces.contains(m_activeMediaPlayer)){
        m_mediaInterfaces[m_activeMediaPlayer]->prevTrack();
    }
}
void MediaManager::nextTrack(){
    if(m_mediaInterfaces.contains(m_activeMediaPlayer)){
        m_mediaInterfaces[m_activeMediaPlayer]->nextTrack();
    }
}
void MediaManager::setVolume(uint8_t volume){
    if(m_mediaInterfaces.contains(m_activeMediaPlayer)){
        m_mediaInterfaces[m_activeMediaPlayer]->setMediaVolume(volume);
    }
}

QStringList MediaManager::getInterfaces(){
    return m_mediaInterfaces.keys();
}

QString MediaManager::getActiveMediaPlayer(){
    return m_activeMediaPlayer;
}
