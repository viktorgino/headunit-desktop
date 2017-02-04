#include "medialibrary.h"

MediaLibrary::MediaLibrary(QObject *parent) : QObject(parent)
{
    mediaScanner = new MediaScanner();
}
const QVariantList MediaLibrary::audioFolders() {
    if(p_audioFolders.isEmpty())
        p_audioFolders = mediaScanner->mediadb->getMediaFolders(MediaDB::AUDIO);
    return p_audioFolders;
}
const QVariantList MediaLibrary::playlists() {
    if(p_playlists.isEmpty())
        p_playlists = mediaScanner->mediadb->getPlaylists();
    return p_playlists;
}
QVariantList MediaLibrary::audioFolderContent(int folder_id) {
    return mediaScanner->mediadb->getFolderContent(folder_id, MediaDB::AUDIO);
}
QVariantList MediaLibrary::videoFolderContent(int folder_id) {
    return mediaScanner->mediadb->getFolderContent(folder_id, MediaDB::VIDEO);
}
