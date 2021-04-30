#include "mediaplayer.h"

MediaPlayerPlugin::MediaPlayerPlugin(QObject *parent) : QObject(parent), mediaLibrary(this)
{
    connect(&mediaLibrary, &MediaLibrary::mediaNotification, this, &MediaPlayerPlugin::message);
}

QObject *MediaPlayerPlugin::getContextProperty(){
    return &mediaLibrary;
}

QQuickImageProvider *MediaPlayerPlugin::getImageProvider() {
    return &m_imageProvider;
}

void MediaPlayerPlugin::init(){

}

MediaPlayerPlugin::~MediaPlayerPlugin(){
}
