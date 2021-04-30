#include "medialibrary.h"

MediaLibrary::MediaLibrary(QObject *parent) : QObject(parent), m_mediaDb(this)
{
    mediaScanner = new MediaScanner(&m_mediaDb, this);

    connect(mediaScanner,SIGNAL(scanningFinished()),this,SLOT(scanningFinished()));
}
const QVariantMap MediaLibrary::audioFolders() {
    if(p_audioFolders.isEmpty())
        p_audioFolders = m_mediaDb.getMediaFolders(MediaDB::AUDIO);
    return p_audioFolders;
}
const QVariantMap MediaLibrary::playlists() {
    if(p_playlists.isEmpty())
        p_playlists = getPlaylists();
    return p_playlists;
}
QVariantMap MediaLibrary::audioFolderContent(int folder_id) {
    return m_mediaDb.getFolderContent(folder_id, MediaDB::AUDIO);
}
QVariantMap MediaLibrary::videoFolderContent(int folder_id) {
    return m_mediaDb.getFolderContent(folder_id, MediaDB::VIDEO);
}
QVariantMap MediaLibrary::albumContent(QString album) {
    return m_mediaDb.getAlbumContent(album);
}
QVariantList MediaLibrary::getLocations() {
    return m_mediaDb.getLocations(false);
}
QVariantMap MediaLibrary::getAlbums() {
    return m_mediaDb.getList(m_mediaDb.albums);
}
QVariantMap MediaLibrary::getArtists() {
    return m_mediaDb.getList(m_mediaDb.artists);
}
QVariantMap MediaLibrary::getGenres() {
    return m_mediaDb.getList(m_mediaDb.genres);
}
QVariantMap MediaLibrary::getPlaylists() {
    QVariantMap playlists = m_mediaDb.getPlaylists();
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
    return m_mediaDb.getAudioFiles();
}
QVariantMap MediaLibrary::getAlbumContent(QString key) {
    QVariantMap list = m_mediaDb.getListContent(m_mediaDb.albums, key);
    return list;
}
QVariantMap MediaLibrary::getArtistContent(QString key) {
    return m_mediaDb.getListContent(m_mediaDb.artists, key);
}
QVariantMap MediaLibrary::getGenreContent(QString key) {
    return m_mediaDb.getListContent(m_mediaDb.genres, key);
}
QVariantMap MediaLibrary::getPlaylistContent(QString key) {
    return m_mediaDb.getListContent(m_mediaDb.playlists, key);
}
QVariantMap MediaLibrary::getSongContent(QString key) {
    return m_mediaDb.getListContent(m_mediaDb.songs, key);
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
        tmp.insert("title", tag->title().toCString());
        tmp.insert("path", path + "/" + line);
        tmp.insert("artist", tag->artist().toCString());
        tmp.insert("album", tag->album().toCString());
        ret.append(tmp);
        i++;
    }
    return ret;
}
QVariantList MediaLibrary::getMountedVolumes(){
    QVariantList ret;
    foreach (const QStorageInfo &storage, QStorageInfo::mountedVolumes()) {
        if (storage.isValid() && storage.isReady()) {
            if (!storage.isReadOnly() && storage.fileSystemType() != "tmpfs" && storage.fileSystemType() != "squashfs") {
                QVariantMap volume;
                qDebug()<<storage.displayName();
                volume.insert("name", storage.displayName());
                volume.insert("path",storage.rootPath());
                ret.append(volume);
            }
        }
    }
    return ret;
}
void MediaLibrary::addLocation(QString path){
    mediaScanner->addLocation(&m_mediaDb, path.remove("file://",Qt::CaseInsensitive));
}
void MediaLibrary::scanningFinished(){
    p_audioFolders = m_mediaDb.getMediaFolders(MediaDB::AUDIO);
    p_playlists = getPlaylists();
    emit libraryUpdated();
    emit mediaNotification("GUI::Notification","{\"image\":\"qrc:/qml/icons/android-search.png\", \"title\":\"Media Scanning finished\", \"description\":\"\"}");
}

