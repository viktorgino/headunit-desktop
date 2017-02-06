#include "mediadb.h"

MediaDB::MediaDB(QObject *parent) : QObject(parent)
{
    QString location_create = "CREATE TABLE \"locations\" ("
                              "`id`	INTEGER UNIQUE,"
                              "`name`	TEXT,"
                              "`relative_path`	TEXT,"
                              "`volume_unique_id`	TEXT,"
                              "`volume_path`	TEXT,"
                              "`is_present`	INTEGER,"
                              "PRIMARY KEY(id)"
                              ");";
    QString media_files_create = "CREATE TABLE \"media_files\" ("
                                 "`filename`	TEXT,"
                                 "`folder_id`	INTEGER,"
                                 "`media_type`	INTEGER"
                                 "PRIMARY KEY(filename,folder_id)"
                                 ")";
    QString scanned_folders_create = "CREATE TABLE \"scanned_folders\" ("
                                     "`id`	INTEGER UNIQUE,"
                                     "`location_id`	INTEGER,"
                                     "`relative_path`	TEXT,"
                                     "`last_modified`	INTEGER,"
                                     "`has_audio`	INTEGER,"
                                     "`has_video`	INTEGER,"
                                     "`thumbnail`	TEXT,"
                                     "PRIMARY KEY(id)"
                                     ")";
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("media_database");

    if (!db.open()){
        qDebug()<< db.lastError().text();
    } else {

        QStringList tables = db.tables();
        QSqlQuery q;
        if (!tables.contains("locations", Qt::CaseInsensitive)){
            if (!q.exec(location_create))
                qDebug()<< qPrintable(q.lastError().text());
        }

        if (!tables.contains("media_files", Qt::CaseInsensitive)){
            if (!q.exec(media_files_create))
                qDebug()<< qPrintable(q.lastError().text());

        }

        if (!tables.contains("scanned_folders", Qt::CaseInsensitive)){
            if (!q.exec(scanned_folders_create))
                qDebug()<< qPrintable(q.lastError().text());
        }
    }
}

int MediaDB::addLocation(QString name, QString v_unique_id, QString v_path, QString relative_path){
    QSqlQuery q;
    if (!q.prepare(QLatin1String("SELECT COUNT(*) AS 'count'  FROM  `locations` WHERE `volume_unique_id` =  ? AND `relative_path` = ?"))){
        qDebug() << q.lastError().text();
        return -1;
    }
    q.addBindValue(v_unique_id);
    q.addBindValue(relative_path);
    if(!q.exec()){
        qDebug()<<q.lastError().text();
        return -1;
    }
    q.first();
    if(q.value("count") == 0){
        //if(true){
        if (!q.prepare(QLatin1String("INSERT INTO `locations`(`name`,`relative_path`,`volume_unique_id`,`volume_path`,`is_present`) VALUES (?,?,?,?,1);"))){
            qDebug() << q.lastError().text();
            return -1;
        }
        q.addBindValue(name);
        q.addBindValue(relative_path);
        q.addBindValue(v_unique_id);
        q.addBindValue(v_path);
        if(!q.exec()){
            qDebug()<<q.lastError().text();
            return -1;
        }
        return q.lastInsertId().toLongLong();
    } else {
        return -2;
    }
}

int MediaDB::addScannedFolder(int location_id, QString relative_path, qint64 last_modified, QString thumbnail){
    QSqlQuery q;
    if (!q.prepare(QLatin1String("SELECT id FROM  `scanned_folders` WHERE `location_id` =  ? AND `relative_path` = ?"))){
        qDebug() << q.lastError().text();
        return -1;
    }
    q.addBindValue(location_id);
    q.addBindValue(relative_path);
    if(!q.exec()){
        qDebug()<<q.lastError().text();
        return -1;
    }
    q.first();
    int id = q.value("id").toInt();
    if(id == 0){
        if (!q.prepare(QLatin1String("INSERT INTO `scanned_folders`(`location_id`,`relative_path`,`last_modified`,`thumbnail`) VALUES (?,?,?,?);"))){
            qDebug() << q.lastError().text();
            return -1;
        }
        q.addBindValue(location_id);
        q.addBindValue(relative_path);
        q.addBindValue(last_modified);
        q.addBindValue(thumbnail);
        if(!q.exec()){
            qDebug()<<q.lastError().text();
            return -1;
        }
        return q.lastInsertId().toLongLong();
    } else {
        return id;
    }
}

int MediaDB::updateFolderInfo(int folder_id, bool hasAudio, bool hasVideo, QString thumbnail){
    QSqlQuery q;
    if (!q.prepare(QLatin1String("UPDATE `scanned_folders` SET `has_audio`=?,`has_video`=?,`thumbnail`=? WHERE `id`=?;"))){
        qDebug() << q.lastError().text();
        return -1;
    }
    q.addBindValue((int)hasAudio);
    q.addBindValue((int)hasVideo);
    q.addBindValue(thumbnail);
    q.addBindValue(folder_id);
    if(!q.exec()){
        qDebug()<<q.lastError().text();
        return -1;
    }

    return 0;
}

int MediaDB::addMediaFiles(QVariantList filenames, QVariantList folder_id, QVariantList media_types){
    QSqlQuery q;
    if (!q.prepare(QLatin1String("INSERT INTO `media_files`(`filename`,`folder_id`,`media_type`) VALUES (?,?,?);"))){
        qDebug() << q.lastError().text();
        return -1;
    }
    q.addBindValue(filenames);
    q.addBindValue(folder_id);
    q.addBindValue(media_types);
    if(!q.execBatch()){
        qDebug()<<q.lastError().text();
        return -1;
    }

    return 0;
}
QVariantMap MediaDB::getLocationInfo(int location_id){
    QSqlQuery q;
    QVariantMap ret;
    if (!q.prepare(QLatin1String("SELECT name,relative_path,volume_path FROM `locations` WHERE `id` =  ?;"))){
        qDebug() << q.lastError().text();
        return ret;
    }
    q.addBindValue(location_id);
    if(!q.exec()){
        qDebug()<<q.lastError().text();
        return ret;
    }
    q.first();
    ret.insert("name",q.value("name"));
    ret.insert("relative_path",q.value("relative_path"));
    ret.insert("volume_path",q.value("volume_path"));
    return ret;
}
QVariantList MediaDB::getLocations(bool onlyAvailable){
    QSqlQuery q;
    QString queryString("SELECT * FROM `locations`");
    if(onlyAvailable)
        queryString.append(" WHERE is_present=1");
    queryString.append(";");
    if (!q.prepare(queryString)){
        qDebug() << q.lastError().text();
        return QVariantList();
    }
    if(!q.exec()){
        qDebug()<<q.lastError().text();
        return QVariantList();
    }
    return getListFromQuery(q);
}
//TODO: Create DB view for getMediaFolders
QVariantList MediaDB::getMediaFolders(int mediaType){
    QSqlQuery q;
    QString queryString("SELECT locations.relative_path || scanned_folders.relative_path AS 'path',"
                        "   scanned_folders.id,"
                        "   'folder' AS 'item_type',"
                        "	CASE"
                        "       WHEN scanned_folders.thumbnail != ''"
                        "           THEN locations.volume_path || locations.relative_path || scanned_folders.relative_path || '/' || scanned_folders.thumbnail"
                        "           ELSE ''"
                        "   END AS 'thumbnail' "
                        "FROM  scanned_folders "
                        "LEFT JOIN locations "
                        "ON locations.id=scanned_folders.location_id ");
    switch(mediaType){
    case AUDIO:
        queryString.append("WHERE scanned_folders.has_audio = 1 ");
        break;
    case VIDEO:
        queryString.append("WHERE scanned_folders.has_video = 1 ");
        break;
    }
    queryString.append("AND locations.is_present=1;");

    if (!q.prepare(queryString)){
        qDebug() << q.lastError().text();
        return QVariantList();
    }
    if(!q.exec()){
        qDebug()<<q.lastError().text();
        return QVariantList();
    }
    return getListFromQuery(q);
}
//TODO: Create DB view for getFolderContent
QVariantList MediaDB::getFolderContent(int folder_id, int mediaType){
    QSqlQuery q;
    QString queryString("SELECT locations.volume_path || locations.relative_path || scanned_folders.relative_path  AS 'path', "
                        "	media_files.filename AS 'name' "
                        "FROM  media_files "
                        "LEFT JOIN scanned_folders "
                        "ON media_files.folder_id=scanned_folders.id "
                        "LEFT JOIN locations "
                        "ON scanned_folders.location_id = locations.id "
                        "WHERE scanned_folders.id = ? "
                        "AND media_files.media_type = ? "
                        "ORDER BY media_files.filename ASC");

    if (!q.prepare(queryString)){
        qDebug() << q.lastError().text();
        return QVariantList();
    }
    q.addBindValue(folder_id);
    q.addBindValue(mediaType);
    if(!q.exec()){
        qDebug()<<q.lastError().text();
        return QVariantList();
    }
    return getListFromQuery(q);
}
QVariantList MediaDB::getPlaylists(){
    QSqlQuery q;
    QString queryString("SELECT * FROM media_files WHERE media_type = ?;");

    if (!q.prepare(queryString)){
        qDebug() << q.lastError().text();
        return QVariantList();
    }
    q.addBindValue(PLAYLIST);
    if(!q.exec()){
        qDebug()<<q.lastError().text();
        return QVariantList();
    }
    return getListFromQuery(q);
}
QVariantList MediaDB::getListFromQuery(QSqlQuery q){
    QSqlRecord record = q.record();

    QVariantList rows;
    while (q.next()){
        QVariantMap row;
        for(int i = 0; i<record.count(); i++){
            row.insert(record.fieldName(i),q.value(i).toString());
        }
        rows.append(row);
    }
    return rows;
}
int MediaDB::setLocationAvailability(int location_id, bool isAvailable){
    QSqlQuery q;
    if (!q.prepare(QLatin1String("UPDATE `locations` SET `is_present`=? WHERE `id`=?;"))){
        qDebug() << q.lastError().text();
        return -1;
    }
    q.addBindValue(isAvailable);
    q.addBindValue(location_id);
    if(!q.exec()){
        qDebug()<<q.lastError().text();
        return -1;
    }

    return 0;

}
