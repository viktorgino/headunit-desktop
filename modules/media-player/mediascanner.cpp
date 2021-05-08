#include "mediascanner.h"

MediaScanner::MediaScanner(MediaDB *mediadb, QObject *parent) : QThread(parent), m_mediadb(mediadb)
{
    audioFileTypes << "*.mp3" << "*.flac" << "*.wav" << "*.ogg" << "*.aac" << "*.aiff";
    playlistFileTypes << "*.m3u";
    imageFileTypes << "*.png"<< "*.jpg";
    mediaFileTypes << audioFileTypes << playlistFileTypes;
    audioFileTypes.replaceInStrings("*.", "");
    playlistFileTypes.replaceInStrings("*.", "");

}

//TODO: Add functionality that not just adds to the database, but compares folder contetents and structure with DB and removes non-existent files/folders from DB
void MediaScanner::run(){
    isRunning = true;
    emit scanningStarted();
    while (!pathsToScan.isEmpty()){
        QMap<QString, QVariant> path = pathsToScan.dequeue();
        scanForFolders(path["path"].toString(),true,path["location_id"].toInt(), "", 0);
    }
    //getUsbBlockSerial();
    isRunning = false;
    emit scanningFinished();
}

void MediaScanner::scanForFolders(QString path, bool is_root, int location_id, QString current_dir, qint64 last_modified){
    QDir currentDir(path);
    currentDir.setFilter(QDir::NoSymLinks | QDir::NoDotAndDotDot | QDir::Dirs);

    if(!is_root)
        current_dir = current_dir + "/" + currentDir.dirName();

    QFileInfoList list = currentDir.entryInfoList();
    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);

        scanForFolders(fileInfo.filePath(),false,location_id,current_dir,fileInfo.lastModified().toMSecsSinceEpoch());
    }

    int folder_id = m_mediadb->getFolderID(current_dir);

    if(folder_id == 0){
        folder_id = m_mediadb->addScannedFolder(location_id,currentDir.dirName(),current_dir,last_modified,"");
    }

    if(folder_id < 0)
        return;

    scanForMediaFiles(path, folder_id);
}

void MediaScanner::scanForMediaFiles(QString path, int folder_id){
    QDir currentDir(path);

    currentDir.setFilter(QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot);

    currentDir.setNameFilters(mediaFileTypes);
    bool hasAudio = false;

    QList<MediaDB::MediaFile> mediaFiles;
    QFileInfoList list = currentDir.entryInfoList();
    if(0 >= list.size())
        return;

    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);
        MediaDB::MediaFile mediaFile;

        TagLib::FileRef f(fileInfo.absoluteFilePath().toUtf8().data());
        TagLib::Tag *tag = f.tag();
        if(tag) {
            if(audioFileTypes.contains(fileInfo.suffix())){
                hasAudio = true;
                mediaFile.media_type = MediaDB::AUDIO;

                mediaFile.filename = fileInfo.fileName();
                mediaFile.folder_id = folder_id;

                mediaFile.artist = tag->artist().toCString();
                mediaFile.title = tag->title().toCString();
                mediaFile.album = tag->album().toCString();
                mediaFile.genre = tag->genre().toCString();
                mediaFiles << mediaFile;
            }
        } else if(playlistFileTypes.contains(fileInfo.suffix())){
            mediaFile.media_type = MediaDB::PLAYLIST;
            mediaFile.filename = fileInfo.fileName();
            mediaFile.folder_id = folder_id;
            mediaFile.title = fileInfo.baseName();

            mediaFiles << mediaFile;
        }
    }
    m_mediadb->addMediaFiles(mediaFiles);
    m_mediadb->updateFolderInfo(folder_id,hasAudio,scanForThumbnail(path, true, ""));
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
 * Add new location to database and start scanning it
 */

int MediaScanner::scanLocation(QString path){
    QStorageInfo volume(path);

    QDir volume_dir(volume.rootPath());
    QString relative_path = volume_dir.relativeFilePath(path);
    QString rootPath = volume.rootPath();

    int location_id = m_mediadb->getLocationID(relative_path);
    if(location_id == 0){
        location_id = m_mediadb->addLocation(relative_path);
    }

    if(location_id < 0)
        return location_id;

    QMap<QString,QVariant> info;

    info.insert("location_id",location_id);
    info.insert("path",path);

    pathsToScan.enqueue(info);
    if(!isRunning)
        this->start();
    return 1;
}
