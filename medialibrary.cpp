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
        p_playlists = getPlaylists();
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
    QVariantMap playlists = mediaScanner->mediadb->getPlaylists();
    QVariantList playlist_counted;
    foreach (QVariant item, playlists["data"].toList()) {
        QVariantMap tmp_item = item.toMap();
        QFile file(tmp_item["path"].toString() + "/" + tmp_item["name"].toString());
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return playlists;
        QTextStream in(&file);
        int i = 0;
        while (!in.atEnd()) {
            in.readLine();
            i++;
        }
        tmp_item.insert("count",i);
        playlist_counted.append(tmp_item);
    }
    playlists["data"] = playlist_counted;
    return playlists;
}
QVariantMap MediaLibrary::getSongs() {
    return mediaScanner->mediadb->getAudioFiles();
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
QVariantList MediaLibrary::getPlaylistContent(QString path, QString name) {
    QVariantList ret;
    QString filepath = path + "/" + name;
    QFile file(filepath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return ret;
    QTextStream in(&file);
    int i = 0;
    while (!in.atEnd()) {
        QString line  = in.readLine();
        QVariantMap tmp;
        TagLib::FileRef f(QString(path + "/" + line).toUtf8().data());
        TagLib::Tag *tag = f.tag();
        tmp.insert("name", tag->title().toCString());
        tmp.insert("path", path + "/" + line);
        tmp.insert("artist", tag->artist().toCString());
        tmp.insert("album", tag->album().toCString());
        ret.append(tmp);
        i++;
    }
    return ret;
}
