#include "mediascanner.h"

MediaScanner::MediaScanner()
{
    mediadb = new MediaDB();
    mediaDirs.append("/media/gino/Gino HDD/music/Trap");
    audioFileTypes << "*.mp3" << "*.flac" << "*.wav" << "*.ogg" << "*.aac" << "*.aiff";
    videoFileTypes << "*.avi" << "*.mkv" << "*.webm" << "*.flv" << "*.mov" << "*.mp4" << "*.mpg" << "*.mpeg";
    playlistFileTypes << "*.m3u";
    mediaFileTypes << audioFileTypes << videoFileTypes << playlistFileTypes;
    audioFileTypes.replaceInStrings(QRegExp("\\*\\."), "");
    videoFileTypes.replaceInStrings(QRegExp("\\*\\."), "");
    playlistFileTypes.replaceInStrings(QRegExp("\\*\\."), "");
}
void MediaScanner::run(){
    isRunning = true;
    for (int i = 0; i < pathsToScan.size(); ++i) {
        scanForFolders(pathsToScan.at(i)["path"].toString(),true,pathsToScan.at(i)["location_id"].toInt(), "", 0);
    }
    //getUsbBlockSerial();
    //QVariantList volumes = getVolumes();
    isRunning = false;
}
void MediaScanner::scanForFolders(QString path, bool is_root, int location_id, QString current_dir, qint64 last_modified){
    QDir currentDir(path);
    currentDir.setFilter(QDir::NoSymLinks | QDir::NoDotAndDotDot | QDir::Dirs);
    if(!is_root)
        current_dir = current_dir + "/" + currentDir.dirName();

    QFileInfoList list = currentDir.entryInfoList();
    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);
        //qDebug() << qPrintable(fileInfo.fileName());
        scanForFolders(fileInfo.filePath(),false,location_id,current_dir,fileInfo.lastModified().toSecsSinceEpoch());
    }
    int folder_id = mediadb->addScannedFolder(location_id,current_dir,last_modified,"");
    if(folder_id < 0)
        return;

    scanForMediaFiles(path, folder_id);
}

void MediaScanner::scanForMediaFiles(QString path, int folder_id){
    QDir currentDir(path);
    currentDir.setFilter(QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot);

    currentDir.setNameFilters(mediaFileTypes);
    bool hasAudio = false;
    bool hasVideo = false;
    QVariantList filenames,media_types,folder_ids;
    QFileInfoList list = currentDir.entryInfoList();
    if(0 >= list.size())
        return;

    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);
        int media_type = 0;
        if(audioFileTypes.contains(fileInfo.suffix())){
            media_type = MediaDB::AUDIO;
            if(!hasAudio)
                hasAudio = true;
        }
        if(videoFileTypes.contains(fileInfo.suffix())){
            media_type = MediaDB::VIDEO;
            if(!hasAudio)
                hasVideo = true;
        }
        if(playlistFileTypes.contains(fileInfo.suffix())){
            media_type = MediaDB::PLAYLIST;
        }
        filenames<<fileInfo.fileName();
        media_types<<media_type;
        folder_ids<<folder_id;
    }
    mediadb->addMediaFiles(filenames,folder_ids,media_types);
    mediadb->updateFolderMediaType(folder_id,hasAudio,hasVideo);
}
/*
 * Get a list of volumes except the root partition
 */
QVariantList MediaScanner::getVolumes(){
    QVariantList volumes;
    foreach (const QStorageInfo &storage, QStorageInfo::mountedVolumes()) {
        if (storage.isValid() && storage.isReady()) {
            if (!storage.isReadOnly() && !storage.isRoot() && storage.fileSystemType() != "tmpfs") {
                QVariantMap info;
                info.insert("name",storage.name());
                info.insert("uuid",getStorageUUID(storage.device()));
                info.insert("path",storage.rootPath());
                volumes.append(info);
                qDebug()<<qPrintable("Name: "+ storage.name() + " | UUID :"+ getStorageUUID(storage.device()) + " | RootPath : "+ storage.rootPath());
            }
        }
    }
    return volumes;
}
/*
 * Get the UUID for the given device
 */
QString MediaScanner::getStorageUUID(QString device){
    QDir currentDir("/dev/disk/by-uuid");
    currentDir.setFilter(QDir::NoDotAndDotDot);

    QFileInfoList list = currentDir.entryInfoList();
    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);
        if(fileInfo.isSymLink() && fileInfo.symLinkTarget() == device){
            return fileInfo.fileName();
        }
    }
}
/*
 * Add new location to database and start scanning it
 */
void MediaScanner::addLocation(QString name, QString v_unique_id, QString v_path, QString relative_path){
    int location_id = mediadb->addLocation(name,v_unique_id,v_path,relative_path);
    if(location_id < 0)
        return;
    scanLocation(location_id);
}
/*
 * Scan location
 */
void MediaScanner::scanLocation(int location_id){
    QVariantMap locationInfo = mediadb->getLocationInfo(location_id);
    QString path(locationInfo["volume_path"].toString() + locationInfo["relative_path"].toString());
    QMap<QString,QVariant> info;

    info.insert("location_id",location_id);
    info.insert("path",path);
    pathsToScan.append(info);
    if(!isRunning)
        QThreadPool::globalInstance()->start(this);
}
