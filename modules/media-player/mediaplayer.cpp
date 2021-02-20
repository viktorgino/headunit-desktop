#include "mediaplayer.h"

MediaPlayerPlugin::MediaPlayerPlugin(QObject *parent) : QObject(parent)
{
    connect(&mediaLibrary, &MediaLibrary::mediaNotification, this, &MediaPlayerPlugin::message);
}

QObject *MediaPlayerPlugin::getContextProperty(){
    return &mediaLibrary;
}

void MediaPlayerPlugin::init(){

}

MediaPlayerPlugin::~MediaPlayerPlugin(){
    delete &mediaLibrary;
}
