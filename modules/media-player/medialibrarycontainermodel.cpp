#include "medialibrarycontainermodel.h"

MediaLibraryContainerModel::MediaLibraryContainerModel(MediaDBManager *mediaDBManager, QObject *parent) : QAbstractListModel(parent), m_mediaDBManager(mediaDBManager)
{

}

void MediaLibraryContainerModel::init(){
}

QHash<int, QByteArray> MediaLibraryContainerModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[TitleRole] = "title";
    roles[SubtitleRole] = "subtitle";
    roles[ThumbnailRole] = "thumbnail";
    roles[FolderRole] = "folder";
    roles[PathRole] = "path";
    return roles;
}


int MediaLibraryContainerModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent)
    return m_containerContent.size();
}

QVariant MediaLibraryContainerModel::data(const QModelIndex &index, int role) const {
    switch (role) {
    case TitleRole: {
        return m_containerContent[index.row()]["title"];
    }
        break;
    case SubtitleRole: {
        switch(m_type){
        case MediaDB::albums:
        case MediaDB::artists:
        case MediaDB::genres:{
            int count = m_containerContent[index.row()]["count"].toInt();
            return QString("%1 File%2").arg(count).arg(count>1?"s":"");
        }
            break;
        case MediaDB::playlists:
        case MediaDB::folders:
            return m_containerContent[index.row()]["path"];
        default:
            return "";
            break;
        }
        break;
    }
        break;
    case ThumbnailRole:{
        return m_containerContent[index.row()]["thumbnail"];
    }
    case FolderRole:{
        return m_containerContent[index.row()]["folder_id"];
    }
        break;
    case PathRole:{
        return m_containerContent[index.row()]["path"];
    }
        break;
    }
    return "";
}

void MediaLibraryContainerModel::setFilter(QString type){
    beginResetModel();
    m_type = MediaDB::stringToListType(type);

    switch(m_type){
    case MediaDB::albums:
    case MediaDB::artists:
    case MediaDB::genres:
        m_containerContent = m_mediaDBManager->getMediaContainers(m_type);
        break;
    case MediaDB::folders:
        m_containerContent = m_mediaDBManager->getFolders();
        break;
    case MediaDB::playlists:
        m_containerContent = m_mediaDBManager->getMediaFiles(MediaDB::PLAYLIST);
        break;
    default:
        m_containerContent.clear();
        break;
    }
    endResetModel();
}

QVariant MediaLibraryContainerModel::getItem(int index){
    if(0 <= index && index < m_containerContent.size()){
        return m_containerContent[index];
    }
    return QVariant();
}
