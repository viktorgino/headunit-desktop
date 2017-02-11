#include "mediascanner.h"

MediaScanner::MediaScanner()
{
    mediadb = new MediaDB();

    audioFileTypes << "*.mp3" << "*.flac" << "*.wav" << "*.ogg" << "*.aac" << "*.aiff";
    videoFileTypes << "*.avi" << "*.mkv" << "*.webm" << "*.flv" << "*.mov" << "*.mp4" << "*.mpg" << "*.mpeg";
    playlistFileTypes << "*.m3u";
    imageFileTypes << "*.png"<< "*.jpg";
    mediaFileTypes << audioFileTypes << videoFileTypes << playlistFileTypes;
    audioFileTypes.replaceInStrings("*.", "");
    videoFileTypes.replaceInStrings("*.", "");
    playlistFileTypes.replaceInStrings("*.", "");

    updateLocationsAvailability();
}
//TODO: Add functionality that not just adds to the database, but compares folder contetents and structure with DB and removes non-existent files/folders from DB
void MediaScanner::run(){
    isRunning = true;
    emit scanningStarted();
    for (int i = 0; i < pathsToScan.size(); ++i) {
        scanForFolders(pathsToScan.at(i)["path"].toString(),true,pathsToScan.at(i)["location_id"].toInt(), "", 0);
    }
    //getUsbBlockSerial();
    //QVariantList volumes = getVolumes();
    isRunning = false;
    emit scanningFinished();
}
void MediaScanner::scanForFolders(QString path, bool is_root, int location_id, QString current_dir, qint64 last_modified){
    QDir currentDir(path);
    currentDir.setFilter(QDir::NoSymLinks | QDir::NoDotAndDotDot | QDir::Dirs);
    QString abs_path = current_dir;
    if(!is_root)
        current_dir = current_dir + "/" + currentDir.dirName();

    QFileInfoList list = currentDir.entryInfoList();
    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);
        //qDebug() << qPrintable(fileInfo.fileName());
        scanForFolders(fileInfo.filePath(),false,location_id,current_dir,fileInfo.lastModified().toMSecsSinceEpoch());
    }
    int folder_id = mediadb->addScannedFolder(location_id,currentDir.dirName(),abs_path,last_modified,"");
    if(folder_id < 0)
        return;

    scanForMediaFiles(path, folder_id);
}
//TODO:have a look at how to use UTF-8
void MediaScanner::scanForMediaFiles(QString path, int folder_id){
    QDir currentDir(path);
    currentDir.setFilter(QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot);

    currentDir.setNameFilters(mediaFileTypes);
    bool hasAudio = false;
    bool hasVideo = false;
    QVariantList filenames,media_types,folder_ids,artist,title,album,genre;
    QFileInfoList list = currentDir.entryInfoList();
    if(0 >= list.size())
        return;

    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);
        int media_type = 0;
        TagLib::FileRef f(fileInfo.absoluteFilePath().toUtf8().data());
        TagLib::Tag *tag = f.tag();

        if(audioFileTypes.contains(fileInfo.suffix())){
            media_type = MediaDB::AUDIO;
            if(!hasAudio)
                hasAudio = true;
            artist<<tag->artist().toCString();
            title<<tag->title().toCString();
            album<<tag->album().toCString();
            genre<<tag->genre().toCString();
        } else {
            if(videoFileTypes.contains(fileInfo.suffix())){
                media_type = MediaDB::VIDEO;
                if(!hasAudio)
                    hasVideo = true;
            } else if(playlistFileTypes.contains(fileInfo.suffix())){
                media_type = MediaDB::PLAYLIST;
            }
            artist<<"";
            title<<"";
            album<<"";
            genre<<"";
        }

        filenames<<fileInfo.fileName();
        media_types<<media_type;
        folder_ids<<folder_id;
    }
    mediadb->addMediaFiles(filenames,folder_ids,media_types,artist,title,album,genre);
    mediadb->updateFolderInfo(folder_id,hasAudio,hasVideo,scanForThumbnail(path, true, ""));
}
QString MediaScanner::scanForThumbnail(QString path, bool tryParent, QString absPosition){
    QDir currentDir(path);
    currentDir.setFilter(QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot);

    currentDir.setNameFilters(imageFileTypes);
    QFileInfoList list = currentDir.entryInfoList();
    if(0 == list.size()){
        if(tryParent){
            currentDir.cdUp();
            QString tmp = scanForThumbnail(currentDir.path(), false, "../");
            return tmp;
        } else {
            return "";
        }
    }
    QString tmp = absPosition + list.at(0).fileName();
    return tmp;
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
int MediaScanner::addLocation(QString name, QString v_unique_id, QString v_path, QString relative_path){
    int location_id = mediadb->addLocation(name,v_unique_id,v_path,relative_path);
    if(location_id < 0)
        return location_id;
    scanLocation(location_id);
    return 1;
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
void MediaScanner::updateLocationsAvailability(){
    QVariantList locations = mediadb->getLocations(false);
    for(int i = 0; i < locations.size(); i++){
        QVariantMap location = locations.at(i).toMap();
        QString path(location["volume_path"].toString() + location["relative_path"].toString());
        QDir currentDir(path);
        if(currentDir.isReadable())
            mediadb->setLocationAvailability(location["id"].toInt(),1);
        else
            mediadb->setLocationAvailability(location["id"].toInt(),0);
    }
}
QVariantMap MediaScanner::getMusicInfo(QString file){
}
