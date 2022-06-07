#include "medialibrarymedialistmodel.h"

MediaLibraryMediaListModel::MediaLibraryMediaListModel(MediaDBManager *mediaDBManager, QObject *parent) : QAbstractListModel(parent), m_mediaDBManager(mediaDBManager)
{
}

void MediaLibraryMediaListModel::init(){
    setFilter("","");
}

QHash<int, QByteArray> MediaLibraryMediaListModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[TitleRole] = "title";
    roles[ArtistRole] = "artist";
    roles[AlbumRole] = "album";
    roles[GenreRole] = "genre";
    roles[PathRole] = "path";
    roles[ThumbnailRole] = "thumbnail";
    roles[FolderRole] = "folder";
    return roles;
}


int MediaLibraryMediaListModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent)
    return m_containerContent.size();
}

QVariant MediaLibraryMediaListModel::data(const QModelIndex &index, int role) const {
    switch (role) {
    case TitleRole:
        return m_containerContent[index.row()]["title"];
        break;
    case ArtistRole:
        return m_containerContent[index.row()]["artist"];
        break;
    case AlbumRole:
        return m_containerContent[index.row()]["album"];
        break;
    case GenreRole:
        return m_containerContent[index.row()]["genre"];
        break;
    case PathRole:
        return m_containerContent[index.row()]["path"];
        break;
    case ThumbnailRole:
        return m_containerContent[index.row()]["thumbnail"];
        break;
    case FolderRole:
        return m_containerContent[index.row()]["folder"];
        break;
    default:
        return "";
        break;
    }
}

QList<QVariantMap> MediaLibraryMediaListModel::getPlaylistContent(QString path) {
    QList<QVariantMap> ret;
    QFile file(path);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return ret;
    QFileInfo fileInfo(path);
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line  = in.readLine();
        QVariantMap tmp;
        QString filePath = fileInfo.absoluteDir().absolutePath() + "/" + line;
        TagLib::FileRef f(filePath.toUtf8().data());
        TagLib::Tag *tag = f.tag();
        if(tag) {
            tmp.insert("title", tag->title().toCString());
            tmp.insert("artist", tag->artist().toCString());
            tmp.insert("album", tag->album().toCString());
            tmp.insert("genre", tag->genre().toCString());
            tmp.insert("path", filePath);
            ret.append(tmp);
        }
    }
    return ret;
}

void MediaLibraryMediaListModel::setFilter(QString type, QString key){
    beginResetModel();
    m_type = MediaDB::stringToListType(type);

    switch(m_type){
    case MediaDB::folders:
        m_containerContent = m_mediaDBManager->getFolderContent(key, MediaDB::AUDIO);
        break;
    case MediaDB::playlists:
        m_containerContent = getPlaylistContent(key);
        break;
    case MediaDB::albums:
    case MediaDB::artists:
    case MediaDB::genres:
    default:
        m_containerContent = m_mediaDBManager->getMediaFiles(MediaDB::AUDIO, m_type, key);
        break;
    }
    endResetModel();
}

QVariant MediaLibraryMediaListModel::getItem(int index){
    if(0 <= index && index < m_containerContent.size()){
        return m_containerContent[index];
    }
    return QVariant();
}

QVariantList MediaLibraryMediaListModel::getItems(){
    QVariantList ret;
    for(const QVariantMap &item : qAsConst(m_containerContent)){
        ret.append(item);
    }
    return ret;
}
