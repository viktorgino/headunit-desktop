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
        //qDebug("Location succesfully added to database");
        return q.lastInsertId().toLongLong();
    } else {
        //qDebug("Location is already in database");
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

        //qDebug("Folder succesfully added to database");
        return q.lastInsertId().toLongLong();
    } else {
        //qDebug("Folder is already in database");
        return id;
    }
}

int MediaDB::updateFolderMediaType(int folder_id, bool hasAudio, bool hasVideo){
    QSqlQuery q;
    if (!q.prepare(QLatin1String("UPDATE `scanned_folders` SET `has_audio`=?,`has_video`=? WHERE `id`=?;"))){
        qDebug() << q.lastError().text();
        return -1;
    }
    q.addBindValue((int)hasAudio);
    q.addBindValue((int)hasVideo);
    q.addBindValue(folder_id);
    if(!q.exec()){
        qDebug()<<q.lastError().text();
        return -1;
    }

    //qDebug("Media succesfully added to database");
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

    //qDebug("Media succesfully added to database");
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
