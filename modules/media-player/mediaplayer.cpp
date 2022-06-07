#include "mediaplayer.h"

MediaPlayerPlugin::MediaPlayerPlugin(QObject *parent) : QObject(parent)
{
    m_pluginSettings.eventListeners = QStringList() << "UsbConnectionListenerPlugin::DriveAdded" << "UsbConnectionListenerPlugin::UsbDeviceRemoved";
    m_interfaceSettings.mediaStream = true;

    m_mediaDbManager = new MediaDBManager(this);
    m_mediaLibraryContainerModel = new MediaLibraryContainerModel(m_mediaDbManager, this);
    m_mediaLibraryMediaListModel = new MediaLibraryMediaListModel(m_mediaDbManager, this);
    m_mediaLibraryPlaylistModel = new MediaPlayerPlaylistModel(this);

    connect(m_mediaDbManager,SIGNAL(scanningFinished()),this,SLOT(scanningFinished()));

    m_mediaDbManager->init();
}

MediaPlayerPlugin::~MediaPlayerPlugin(){
}

QObject *MediaPlayerPlugin::getContextProperty(){
    return this;
}
void MediaPlayerPlugin::eventMessage(QString id, QVariant message){
    if(id == "UsbConnectionListenerPlugin::DriveAdded"){
        m_mediaDbManager->updateLocations();
    } else if(id == "UsbConnectionListenerPlugin::UsbDeviceRemoved"){
        m_mediaDbManager->updateLocations();
    }
}
QQuickImageProvider *MediaPlayerPlugin::getImageProvider() {
    return &m_imageProvider;
}

void MediaPlayerPlugin::init(){
    m_mediaLibraryContainerModel->init();
    m_mediaLibraryMediaListModel->init();
    m_mediaLibraryPlaylistModel->init();
}

QVariantList MediaPlayerPlugin::getLocations() {
    return m_mediaDbManager->getLocations();
}

qreal MediaPlayerPlugin::getVolume(){
    return m_volume;
}

void MediaPlayerPlugin::addLocation(QString path){
    m_mediaDbManager->addLocation(path.remove("file://",Qt::CaseInsensitive));
    emit locationsUpdated();
}

void MediaPlayerPlugin::scanningFinished(){
    emit libraryUpdated();
    emit message("GUI::Notification","{\"image\":\"qrc:/qml/icons/android-search.png\", \"title\":\"Media Scanning finished\", \"description\":\"\"}");
}

void MediaPlayerPlugin::setMediaVolume(uint8_t volume) {
    m_volume = volume / 100.00;
    emit volumeUpdated();
}
