#include "medialibrary.h"

MediaLibrary::MediaLibrary(QObject *parent) : QObject(parent)
{
    mediaScanner = new MediaScanner();
}
const QVariantMap MediaLibrary::audioFolders() {
    if(p_audioFolders.isEmpty())
        p_audioFolders = mediaScanner->mediadb->getMediaFolders(MediaDB::AUDIO);
    return p_audioFolders;
}
const QVariantMap MediaLibrary::playlists() {
    if(p_playlists.isEmpty())
        p_playlists = mediaScanner->mediadb->getPlaylists();
    return p_playlists;
}
QVariantMap MediaLibrary::audioFolderContent(int folder_id) {
    return mediaScanner->mediadb->getFolderContent(folder_id, MediaDB::AUDIO);
}
QVariantMap MediaLibrary::videoFolderContent(int folder_id) {
    return mediaScanner->mediadb->getFolderContent(folder_id, MediaDB::VIDEO);
}
QVariantMap MediaLibrary::albumContent(QString album) {
    return mediaScanner->mediadb->getAlbumContent(album);
}
QVariantMap MediaLibrary::getAlbums() {
    return mediaScanner->mediadb->getList(mediaScanner->mediadb->albums);
}
QVariantMap MediaLibrary::getArtists() {
    return mediaScanner->mediadb->getList(mediaScanner->mediadb->artists);
}
QVariantMap MediaLibrary::getGenres() {
    return mediaScanner->mediadb->getList(mediaScanner->mediadb->genres);
}
QVariantMap MediaLibrary::getPlaylists() {
    return mediaScanner->mediadb->getList(mediaScanner->mediadb->playlists);
}
QVariantMap MediaLibrary::getSongs() {
    return mediaScanner->mediadb->getList(mediaScanner->mediadb->songs);
}
QVariantMap MediaLibrary::getAlbumContent(QString key) {
    return mediaScanner->mediadb->getListContent(mediaScanner->mediadb->albums, key);
}
QVariantMap MediaLibrary::getArtistContent(QString key) {
    return mediaScanner->mediadb->getListContent(mediaScanner->mediadb->artists, key);
}
QVariantMap MediaLibrary::getGenreContent(QString key) {
    return mediaScanner->mediadb->getListContent(mediaScanner->mediadb->genres, key);
}
QVariantMap MediaLibrary::getPlaylistContent(QString key) {
    return mediaScanner->mediadb->getListContent(mediaScanner->mediadb->playlists, key);
}
QVariantMap MediaLibrary::getSongContent(QString key) {
    return mediaScanner->mediadb->getListContent(mediaScanner->mediadb->songs, key);
}
