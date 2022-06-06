#include "mediaplayerplaylistmodel.h"

MediaPlayerPlaylistModel::MediaPlayerPlaylistModel( QObject *parent) : QAbstractListModel(parent)
{
}

void MediaPlayerPlaylistModel::init(){
    m_settings.beginGroup("MediaPlayer");
    bool hasNowPlaying = m_settings.contains("nowPlaying");
    QVariantList items = m_settings.value("nowPlaying").toList();
    m_settings.endGroup();
    if(hasNowPlaying){
        if(items.count() > 0){
            setItems(items);
        }
    }
}

QHash<int, QByteArray> MediaPlayerPlaylistModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[TitleRole] = "title";
    roles[ArtistRole] = "artist";
    roles[PathRole] = "path";
    return roles;
}

int MediaPlayerPlaylistModel::rowCount(const QModelIndex &parent) const{
    Q_UNUSED(parent)
    return m_playlistContent.size();
}

QVariant MediaPlayerPlaylistModel::data(const QModelIndex &index, int role) const{

    switch (role) {
    case TitleRole:
        return m_playlistContent[index.row()].toMap()["title"];
        break;
    case ArtistRole:
        return m_playlistContent[index.row()].toMap()["artist"];
        break;
    case PathRole:
        return "file://" + m_playlistContent[index.row()].toMap()["path"].toString();
        break;
    default:
        return "";
        break;
    }
}
void MediaPlayerPlaylistModel::setItems(QVariantList items){
    beginResetModel();
    m_playlistContent.clear();
    for(const QVariant &item : items){
        QVariantMap map = item.toMap();
        if(map.contains("path") && QFile::exists(map["path"].toString())){
            m_playlistContent.append(map);
        }
    }
    endResetModel();

    m_settings.beginGroup("MediaPlayer");
    m_settings.setValue("nowPlaying",m_playlistContent);
    m_settings.endGroup();
    emit sourcesChanged();
}

QVariantList MediaPlayerPlaylistModel::getSources(){
    QVariantList ret;
    for(QVariant item : qAsConst(m_playlistContent)){
        ret.append("file://" + item.toMap()["path"].toString());
    }
    return ret;
}
