#include "mediaplayer.h"

MediaPlayerPlugin::MediaPlayerPlugin(QObject *parent) : QObject(parent)
{
    connect(&mediaLibrary, &MediaLibrary::mediaNotification, this, &MediaPlayerPlugin::onMessage);
}

QObject *MediaPlayerPlugin::getContextProperty(){
    return &mediaLibrary;
}

QQuickImageProvider *MediaPlayerPlugin::getImageProvider() {
    return nullptr;
}

QStringList MediaPlayerPlugin::eventListeners(){
    return QStringList();
}

QStringList MediaPlayerPlugin::events(){
    return QStringList();
}

QStringList MediaPlayerPlugin::actions() {
    return QStringList();
}

void MediaPlayerPlugin::eventMessage(QString id, QString message){

}

void MediaPlayerPlugin::actionMessage(QString id, QString message){

}

MediaPlayerPlugin::~MediaPlayerPlugin(){
    delete &mediaLibrary;
}
