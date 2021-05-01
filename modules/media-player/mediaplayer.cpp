#include "mediaplayer.h"

MediaPlayerPlugin::MediaPlayerPlugin(QObject *parent) : QObject(parent)
{
    m_mediaScanner = new MediaScanner(&m_mediaDb, this);
    m_mediaLibraryContainerModel = new MediaLibraryContainerModel(&m_mediaDb, this);
    m_mediaLibraryMediaListModel = new MediaLibraryMediaListModel(&m_mediaDb, this);
    m_mediaLibraryPlaylistModel = new MediaPlayerPlaylistModel(this);
    connect(m_mediaScanner,SIGNAL(scanningFinished()),this,SLOT(scanningFinished()));
}

MediaPlayerPlugin::~MediaPlayerPlugin(){
}

QObject *MediaPlayerPlugin::getContextProperty(){
    return this;
}

QQuickImageProvider *MediaPlayerPlugin::getImageProvider() {
    return &m_imageProvider;
}

void MediaPlayerPlugin::init(){
    m_mediaScanner->init();
    m_mediaLibraryContainerModel->init();
    m_mediaLibraryMediaListModel->init();
    m_mediaLibraryPlaylistModel->init();
}

QVariantList MediaPlayerPlugin::getLocations() {
    return m_mediaDb.getLocations(false);
}

void MediaPlayerPlugin::addLocation(QString path){
    m_mediaScanner->addLocation(path.remove("file://",Qt::CaseInsensitive));
}

void MediaPlayerPlugin::scanningFinished(){
    emit libraryUpdated();
    emit message("GUI::Notification","{\"image\":\"qrc:/qml/icons/android-search.png\", \"title\":\"Media Scanning finished\", \"description\":\"\"}");
}
