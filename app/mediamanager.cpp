#include "mediamanager.h"

MediaManager::MediaManager(QObject *parent) : QObject(parent), m_mediaVolumes(new QQmlPropertyMap()), m_voiceVolumes(new QQmlPropertyMap())
{
    QSettings settings;
    m_activeMediaPlayer = settings.value("MediaManager/ActiveMediaPlayer").toString();

    connect(m_mediaVolumes, &QQmlPropertyMap::valueChanged, this, &MediaManager::setMediaVolume);
    connect(m_voiceVolumes, &QQmlPropertyMap::valueChanged, this, &MediaManager::setVoiceVolume);
}

void MediaManager::addInterface(QString name, MediaInterface * object){
    if(object){
        m_mediaInterfaces[name] = object;
        emit intefacesChanged();

        if(object->getSettings().mediaStream) {
            m_mediaVolumes->insert(name,  100);
        }
        if(object->getSettings().voiceStream) {
            m_voiceVolumes->insert(name,  100);
        }
    }
}

void MediaManager::setActiveMediaPlayer(QString name){
    m_activeMediaPlayer = name;

    QSettings settings;
    settings.setValue("MediaManager/ActiveMediaPlayer", m_activeMediaPlayer);

    emit activeMediaPlayerChanged();
}

void MediaManager::setVoiceVolume(QString interface, QVariant value){
    qDebug () << interface << value;
    if(m_mediaInterfaces.contains(interface)){
        m_mediaInterfaces[interface]->setVolume(value.toInt());
    }
}
void MediaManager::setMediaVolume(QString interface, QVariant value){
    qDebug () << interface << value;
    if(m_mediaInterfaces.contains(interface)){
        m_mediaInterfaces[interface]->setVolume(value.toInt());
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
        m_mediaInterfaces[m_activeMediaPlayer]->setVolume(volume);
    }
}

QStringList MediaManager::getInterfaces(){
    return m_mediaInterfaces.keys();
}

QString MediaManager::getActiveMediaPlayer(){
    return m_activeMediaPlayer;
}
