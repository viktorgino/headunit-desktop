#include "mediadbmanager.h"

Q_LOGGING_CATEGORY(MEDIAPLAYER_DBMANAGER, "MediaPlayer::MediaDBManager")

MediaDBManager::MediaDBManager(QObject *parent) : QObject(parent)
{

}

void MediaDBManager::init(){
    updateLocations();
}

void MediaDBManager::updateLocations() {
    QMutableListIterator<MediaDB *> i(m_mediaDBs);

    while(i.hasNext()){
        MediaDB *mediaDb = i.next();
        QStorageInfo volume(mediaDb->getPath());
        if(!volume.isValid()){
            qCDebug(MEDIAPLAYER_DBMANAGER) << "Removing db: " << mediaDb->getPath();
            i.remove();
            mediaDb->deleteLater();
        }
    }
    for (const QStorageInfo &storage : QStorageInfo::mountedVolumes()) {
        if (storage.isValid() && storage.isReady()) {
            if (/*!storage.isReadOnly() && */storage.fileSystemType() != "tmpfs" && storage.fileSystemType() != "squashfs") {
                QDir path;
                if(storage.isRoot()){
                    path.setPath(QDir::homePath());
                } else {
                    path.setPath(storage.rootPath());
                }
                if(path.entryList().contains("media_database") && !getMediaDB(storage.rootPath())){
                    qCDebug(MEDIAPLAYER_DBMANAGER)<< "Adding : " << path.path();
                    MediaDB * mediaDb = new MediaDB(storage.rootPath(), this);
                    m_mediaDBs.append(mediaDb);
                }
            }
        }
    }
}

QVariantList MediaDBManager::getLocations() {
    QVariantList ret;
    for(MediaDB *mediaDb : qAsConst(m_mediaDBs)){
        ret.append(mediaDb->getLocations());
    }
    return ret;
}

MediaDB * MediaDBManager::getMediaDB(QString mountPoint) {
    for(MediaDB *p_mediaDb : qAsConst(m_mediaDBs)){
        if(p_mediaDb->getPath() == mountPoint){
            return p_mediaDb;
        }
    }
    return nullptr;
}

void MediaDBManager::addLocation(QString path) {
    QStorageInfo volume(path);
    MediaDB * mediaDb = getMediaDB(volume.rootPath());

    if(!mediaDb){
        qCDebug(MEDIAPLAYER_DBMANAGER) << "Creating db: " << volume.rootPath();
        mediaDb = new MediaDB(volume.rootPath(), this);
        m_mediaDBs.append(mediaDb);
    }

    if(mediaDb){
        MediaScanner *scanner = new MediaScanner(mediaDb,this);

        connect(scanner,SIGNAL(scanningFinished()),this,SLOT(scanningFinishedHandler()));
        scanner->scanLocation(path);
    }
}
bool sortAsc(const QVariantMap &a, const QVariantMap &b)
{
    return a["title"].toString() < b["title"].toString();
}

bool titleEqual(QVariantMap &a, QVariantMap &b) {
    bool isEqual = a["title"].toString() == b["title"].toString();
    if(isEqual){
        a["count"] = a["count"].toInt() + b["count"].toInt();
    }
    return isEqual;
}
QList<QVariantMap> MediaDBManager::getMediaContainers(MediaDB::ListType listType){
    QList<QVariantMap> ret;
    for(MediaDB *mediaDb : m_mediaDBs){
        QList<QVariantMap> containers = mediaDb->getMediaContainers(listType);
        ret += containers;
    }
    std::sort(ret.begin(), ret.end(), sortAsc);
    ret.erase(std::unique(ret.begin(), ret.end(), titleEqual), ret.end());

    return ret;
}

QList<QVariantMap> MediaDBManager::getFolders(){
    QList<QVariantMap> ret;

    for(MediaDB *mediaDb : m_mediaDBs){
        ret.append(mediaDb->getFolders());
    }
    std::sort(ret.begin(), ret.end(), sortAsc);

    return ret;
}

QList<QVariantMap> MediaDBManager::getMediaFiles(MediaDB::MediaTypes mediaType, MediaDB::ListType listType, QString key){
    QList<QVariantMap> ret;

    for(MediaDB *mediaDb : m_mediaDBs){
        ret.append(mediaDb->getMediaFiles(mediaType, listType, key));
    }
    std::sort(ret.begin(), ret.end(), sortAsc);

    return ret;
}
QList<QVariantMap> MediaDBManager::getFolderContent(QString folder_id, int mediaType){
    QList<QVariantMap> ret;

    for(MediaDB *mediaDb : m_mediaDBs){
        ret.append(mediaDb->getFolderContent(folder_id, mediaType));
    }
    std::sort(ret.begin(), ret.end(), sortAsc);

    return ret;
}

void MediaDBManager::scanningFinishedHandler(){
    QObject *obj = QObject::sender();
    disconnect(obj,SIGNAL(scanningFinished()),this,SLOT(scanningFinishedHandler()));
    delete (obj);
    emit scanningFinished();
}
