#include "mediadb.h"

static int dbId = 0;

Q_LOGGING_CATEGORY(MEDIAPLAYER, "MediaPlayer")

MediaDB::MediaDB(QObject *parent) : QObject(parent)
{
    QString location_create = "CREATE TABLE \"locations\" ("
                              "`id` INTEGER UNIQUE,"
                              "`name` TEXT,"
                              "`relative_path` TEXT,"
                              "`volume_unique_id` TEXT,"
                              "`volume_path` TEXT,"
                              "`is_present` INTEGER,"
                              "PRIMARY KEY(id)"
                              ");";
    QString media_files_create = "CREATE TABLE \"media_files\" ( "
                                 "`filename` TEXT,"
                                 "`folder_id` INTEGER,"
                                 "`media_type` INTEGER,"
                                 "`artist` TEXT,"
                                 "`title` TEXT,"
                                 "`album` TEXT,"
                                 "`genre` TEXT,"
                                 "PRIMARY KEY(filename,folder_id)"
                                 ")";
    QString scanned_folders_create = "CREATE TABLE \"scanned_folders\" ("
                                     "`id` INTEGER UNIQUE,"
                                     "`name` TEXT,"
                                     "`location_id` INTEGER,"
                                     "`relative_path` TEXT,"
                                     "`last_modified` INTEGER,"
                                     "`has_audio` INTEGER,"
                                     "`thumbnail` TEXT,"
                                     "PRIMARY KEY(id)"
                                     ")";
    QString media_files_view_create = "CREATE VIEW media_files_view "
                                      "AS "
                                      "SELECT locations.volume_path || locations.relative_path || scanned_folders.relative_path || '/' || media_files.filename AS 'path', "
                                      "media_files.artist, "
                                      "media_files.title, "
                                      "media_files.album, "
                                      "media_files.media_type,"
                                      "media_files.genre,"
                                      "CASE "
                                      "WHEN scanned_folders.thumbnail != '' "
                                      "THEN locations.volume_path || locations.relative_path || scanned_folders.relative_path || '/' || scanned_folders.thumbnail "
                                      "ELSE '' "
                                      "END AS 'thumbnail', "
                                      "scanned_folders.id AS 'folder_id' "
                                      "FROM  media_files "
                                      "LEFT JOIN scanned_folders ON media_files.folder_id=scanned_folders.id "
                                      "LEFT JOIN locations ON scanned_folders.location_id = locations.id";
    QString dbName = QString("media_database%1").arg(dbId++);
    db = QSqlDatabase::addDatabase("QSQLITE", dbName);
    db.setDatabaseName("media_database");

    if (!db.open()){
        qCWarning(MEDIAPLAYER) << "Error opening database : " << db.lastError().text();
        return;
    }
    QSqlQuery q(db);

    QStringList tables = db.tables(QSql::AllTables);

    if (!tables.contains("locations", Qt::CaseInsensitive)){
        if (!q.exec(location_create))
            qCWarning(MEDIAPLAYER)<< qPrintable("Error when creating locations table : " + q.lastError().text());
    }

    if (!tables.contains("media_files", Qt::CaseInsensitive)){
        if (!q.exec(media_files_create))
            qCWarning(MEDIAPLAYER)<< qPrintable("Error when creating media_files table : " + q.lastError().text());

    }

    if (!tables.contains("scanned_folders", Qt::CaseInsensitive)){
        if (!q.exec(scanned_folders_create))
            qCWarning(MEDIAPLAYER)<< qPrintable("Error when creating scanned_folders table : " + q.lastError().text());
    }

    if (!tables.contains("media_files_view", Qt::CaseInsensitive)){
        if (!q.exec(media_files_view_create))
            qCWarning(MEDIAPLAYER)<< qPrintable("Error when creating media_files_view table : " + q.lastError().text());
    }
    db.close();
}

QList<QVariantMap> MediaDB::executeQuery(QString query, QVariantMap values){
    QList<QVariantMap> ret;

    if (!db.open()){
        qCWarning(MEDIAPLAYER) << Q_FUNC_INFO << ": db open error : " <<  db.lastError().text();
        return ret;
    }
    QSqlQuery q(db);

    if (q.prepare(query)){
        QStringList keys = values.keys();
        for(QString key : keys){
            q.bindValue(key, values.value(key));
        }

        if(q.exec()){
            QSqlRecord record = q.record();

            while (q.next()){
                QVariantMap row;
                for(int i = 0; i<record.count(); i++){
                    row.insert(record.fieldName(i), q.value(i).toString());
                }
                ret.append(row);
            }
        } else {
            qCWarning(MEDIAPLAYER) << Q_FUNC_INFO << ":" <<  q.lastError().text();
            qCWarning(MEDIAPLAYER) << query;
        }
    } else {
        qCWarning(MEDIAPLAYER) << Q_FUNC_INFO << ":" <<  q.lastError().text();
        qCWarning(MEDIAPLAYER) << query;
    }

    db.close();
    return ret;
}

int MediaDB::addLocation(QString name, QString v_unique_id, QString v_path, QString relative_path){
    int ret = 0;

    if (!db.open()){
        qCWarning(MEDIAPLAYER) << Q_FUNC_INFO << ": db open error : " <<  db.lastError().text();
        return -2;
    }
    QSqlQuery q(db);


    if (!q.prepare(QLatin1String("SELECT COUNT(*) AS 'count'  FROM  `locations` WHERE `volume_unique_id` =  :v_unique_id AND `relative_path` = :relative_path"))){
        qCWarning(MEDIAPLAYER) << Q_FUNC_INFO << ":" <<  q.lastError().text();
        ret = -1;
        goto ret;
    }
    q.bindValue("v_unique_id",v_unique_id);
    q.bindValue("relative_path",relative_path);
    if(!q.exec()){
        qCWarning(MEDIAPLAYER) << Q_FUNC_INFO << ":" <<  q.lastError().text();
        ret = -1;
        goto ret;
    }
    q.first();
    if(q.value("count") == 0){

        if (!q.prepare(QLatin1String("INSERT INTO `locations`(`name`,`relative_path`,`volume_unique_id`,`volume_path`,`is_present`)"
                                     "VALUES (:name,:relative_path,:v_unique_id,:v_path,1);"))){
            qCWarning(MEDIAPLAYER) << Q_FUNC_INFO << ":" <<  q.lastError().text();
            ret = -1;
            goto ret;
        }
        q.bindValue(":name",name);
        q.bindValue(":relative_path",relative_path);
        q.bindValue(":v_unique_id",v_unique_id);
        q.bindValue(":v_path",v_path);
        if(!q.exec()){
            qCWarning(MEDIAPLAYER) << Q_FUNC_INFO << ":" <<  q.lastError().text();
            ret = -1;
            goto ret;
        }
        ret = q.lastInsertId().toLongLong();
    } else {
        ret = -2;
        goto ret;
    }
ret:
    db.close();
    return ret;
}

int MediaDB::addScannedFolder(int location_id, QString name, QString relative_path, qint64 last_modified, QString thumbnail){
    int ret = 0;

    if (!db.open()){
        qCWarning(MEDIAPLAYER) << Q_FUNC_INFO << ": db open error : " <<  db.lastError().text();
        return -2;
    }
    QSqlQuery q(db);

    if (!q.prepare(QLatin1String("SELECT id FROM  `scanned_folders` WHERE `location_id` =  :location_id AND `name` = :name AND `relative_path` = :relative_path"))){
        qCWarning(MEDIAPLAYER) << Q_FUNC_INFO << ":" <<  q.lastError().text();
        ret = -1;
        goto ret;
    }
    q.bindValue(":location_id",location_id);
    q.bindValue(":name",name);
    q.bindValue(":relative_path",relative_path);
    if(!q.exec()){
        qCWarning(MEDIAPLAYER) << Q_FUNC_INFO << ":" <<  q.lastError().text();
        ret = -1;
        goto ret;
    }
    if(q.size() <= 0){
        if (!q.prepare(QLatin1String("INSERT INTO `scanned_folders`(`location_id`,`name`,`relative_path`,`last_modified`,`thumbnail`) "
                                     "VALUES (:location_id,:name,:relative_path,:last_modified,:thumbnail);"))){
            qCWarning(MEDIAPLAYER) << Q_FUNC_INFO << ":" <<  q.lastError().text();
            ret = -1;
            goto ret;
        }
        q.bindValue(":location_id",location_id);
        q.bindValue(":name",name);
        q.bindValue(":relative_path",relative_path);
        q.bindValue(":last_modified",last_modified);
        q.bindValue(":thumbnail",thumbnail);
        if(!q.exec()){
            qCWarning(MEDIAPLAYER) << Q_FUNC_INFO << ":" <<  q.lastError().text();
            ret = -1;
            goto ret;
        }
        ret = q.lastInsertId().toLongLong();
    } else {
        q.first();
        ret = q.value("id").toInt();
    }
ret:
    db.close();
    return ret;
}

int MediaDB::addMediaFiles(QList<MediaFile> mediaFiles){
    int ret = 0;

    if (!db.open()){
        qCWarning(MEDIAPLAYER) << Q_FUNC_INFO << ": db open error : " <<  db.lastError().text();
        return -2;
    }
    QSqlQuery q(db);

    for(MediaFile mediaFile : mediaFiles){
        if (!q.prepare(QLatin1String("INSERT INTO `media_files`(`filename`,`folder_id`,`media_type`,`artist`,`title`,`album`,`genre`) "
                                     "VALUES (:filenames,:folder_id,:media_types,:artist,:title,:album,:genre);"))){
            qCWarning(MEDIAPLAYER) << Q_FUNC_INFO << ":" <<  q.lastError().text();
            ret = -1;
            goto ret;
        }
        q.bindValue(":filenames",mediaFile.filename);
        q.bindValue(":folder_id",mediaFile.folder_id);
        q.bindValue(":media_types",mediaFile.media_type);
        q.bindValue(":artist",mediaFile.artist);
        q.bindValue(":title",mediaFile.title);
        q.bindValue(":album",mediaFile.album);
        q.bindValue(":genre",mediaFile.genre);
        if(!q.exec()){
            qCWarning(MEDIAPLAYER) << Q_FUNC_INFO << ":" <<  q.lastError().text();
            ret = -1;
            goto ret;
        }
    }

ret:
    db.close();
    return ret;
}
int MediaDB::updateFolderInfo(int folder_id, bool hasAudio, QString thumbnail){
    int ret = 0;

    if (!db.open()){
        qCWarning(MEDIAPLAYER) << Q_FUNC_INFO << ": db open error : " <<  db.lastError().text();
        return -2;
    }
    QSqlQuery q(db);

    if (!q.prepare(QLatin1String("UPDATE `scanned_folders` SET `has_audio`=:hasAudio,`thumbnail`=:thumbnail WHERE `id`=:folder_id;"))){
        qCWarning(MEDIAPLAYER) << Q_FUNC_INFO << ":" <<  q.lastError().text();
        ret =  -1;
        goto ret;
    }
    q.bindValue(":hasAudio",(int)hasAudio);
    q.bindValue(":thumbnail",thumbnail);
    q.bindValue(":folder_id",folder_id);
    if(!q.exec()){
        qCWarning(MEDIAPLAYER) << Q_FUNC_INFO << ":" <<  q.lastError().text();
        ret = -1;
        goto ret;
    }

ret:
    db.close();
    return ret;
}

int MediaDB::setLocationAvailability(int location_id, bool isAvailable){
    int ret = 0;

    if (!db.open()){
        qCWarning(MEDIAPLAYER) << Q_FUNC_INFO << ": db open error : " <<  db.lastError().text();
        return -1;
    }
    QSqlQuery q(db);

    if (q.prepare(QLatin1String("UPDATE `locations` SET `is_present`=:isAvailable WHERE `id`=:location_id;"))){
        q.bindValue(":isAvailable", isAvailable);
        q.bindValue(":location_id", location_id);
        if(!q.exec()){
            qCWarning(MEDIAPLAYER) << Q_FUNC_INFO << ":" <<  q.lastError().text();
            ret = -1;
        }
    } else {
        qCWarning(MEDIAPLAYER) << Q_FUNC_INFO << ":" <<  q.lastError().text();
        ret = -1;
    }

    db.close();
    return ret;

}

QVariantMap MediaDB::getLocationInfo(int location_id){
    QVariantMap ret;

    if (!db.open()){
        qCWarning(MEDIAPLAYER) << Q_FUNC_INFO << ": db open error : " <<  db.lastError().text();
        return ret;
    }
    QSqlQuery q(db);

    if (!q.prepare(QLatin1String("SELECT name,relative_path,volume_path FROM `locations` WHERE `id` =  :location_id;"))){
        qCWarning(MEDIAPLAYER) << Q_FUNC_INFO << ":" <<  q.lastError().text();
        goto ret;
    }
    q.bindValue(":location_id",location_id);
    if(!q.exec()){
        qCWarning(MEDIAPLAYER) << Q_FUNC_INFO << ":" <<  q.lastError().text();
        goto ret;
    }
    q.first();
    ret.insert("name",q.value("name"));
    ret.insert("relative_path",q.value("relative_path"));
    ret.insert("volume_path",q.value("volume_path"));

ret:
    db.close();
    return ret;
}

QVariantList MediaDB::getLocations(bool onlyAvailable){
    QString queryString("SELECT * FROM `locations`");

    QVariantList ret;

    if (!db.open()){
        qCWarning(MEDIAPLAYER) << Q_FUNC_INFO << ": db open error : " <<  db.lastError().text();
        return ret;
    }

    QSqlQuery q(db);

    if(onlyAvailable){
        queryString.append(" WHERE is_present=1");
    }
    queryString.append(";");

    if(q.exec(queryString)){
        ret = getListFromQuery(q);
    } else {
        qCWarning(MEDIAPLAYER) << Q_FUNC_INFO << ":" <<  q.lastError().text();
    }
    db.close();
    return ret;
}

//TODO: Create DB view for getMediaFolders
QList<QVariantMap> MediaDB::getFolders(){
    QString queryString("SELECT locations.relative_path || scanned_folders.relative_path AS 'path', "
                        "   scanned_folders.id AS 'folder_id', "
                        "   scanned_folders.name AS 'title', "
                        " CASE"
                        "       WHEN scanned_folders.thumbnail != ''"
                        "           THEN locations.volume_path || locations.relative_path || scanned_folders.relative_path || '/' || scanned_folders.thumbnail"
                        "           ELSE ''"
                        "   END AS 'thumbnail' "
                        "FROM  scanned_folders "
                        "LEFT JOIN locations "
                        "ON locations.id=scanned_folders.location_id "
                        "WHERE scanned_folders.has_audio = 1 "
                        "AND locations.is_present=1;");

    QVariantMap values;

    return executeQuery(queryString, values);
}
//TODO: Create DB view for getFolderContent
QList<QVariantMap> MediaDB::getFolderContent(QString folder_id, int mediaType){
    QString queryString("SELECT * FROM media_files_view "
                        "WHERE folder_id = :folder_id AND media_type = :media_type "
                        "ORDER BY cast(path as int) COLLATE NOCASE ASC");
    QVariantMap values;
    values.insert(":folder_id", folder_id);
    values.insert(":media_type", mediaType);
    return executeQuery(queryString, values);
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

QList<QVariantMap> MediaDB::getMediaContainers(ListType listType){
    QString typeString;
    switch (listType){
    case albums:typeString = "album"; break;
    case artists:typeString = "artist"; break;
    case genres:typeString = "genre"; break;
    case playlists:typeString = "playlist"; break;
    case songs:typeString = "songs"; break;
    default: return QList<QVariantMap>();
    }
    QString queryString = QString ("SELECT %1 as 'title', "
                                   "thumbnail, "
                                   "COUNT(%1) as 'count' "
                                   "FROM media_files_view "
                                   "GROUP BY %1 "
                                   "ORDER BY %1 COLLATE NOCASE ASC").arg(typeString);
    QVariantMap values;
    return executeQuery(queryString, values);
}

QList<QVariantMap> MediaDB::getMediaFiles(MediaTypes mediaType, ListType listType, QString key){
    QString typeString;
    switch (listType) {
    case albums: typeString = "album"; break;
    case artists: typeString = "artist"; break;
    case genres: typeString = "genre"; break;
    case playlists: typeString = "playlist"; break;
    case songs: typeString = "song"; break;
    case folders: typeString = "folder"; break;
    default: typeString = "none"; break;
    }

    QString queryString = QString ("SELECT * FROM media_files_view ");
    QVariantMap values;
    QString filters;

    if(typeString != "none" && !key.isEmpty()){
        filters = QString ("WHERE %1 = :key AND media_type = :media_type ").arg(typeString);
        values.insert(":key",key);
    } else {
        filters = QString("WHERE media_type = :media_type ");
    }

    queryString.append(filters);
    values.insert(":media_type", mediaType);

    queryString.append("ORDER BY cast(path as int) COLLATE NOCASE ASC");

    return executeQuery(queryString, values);
}

MediaDB::ListType MediaDB::stringToListType(QString type){
    if(type == "albums"){
        return MediaDB::albums;
    } else if(type == "artists"){
        return MediaDB::artists;
    } else if(type == "genres"){
        return MediaDB::genres;
    } else if(type == "playlists"){
        return MediaDB::playlists;
    } else if(type == "songs"){
        return MediaDB::songs;
    } else if(type == "folders"){
        return MediaDB::folders;
    } else {
        return MediaDB::none;
    }
}
QString MediaDB::listTypeToString(ListType type){
    switch (type) {
    case albums:
        return "albums";
    case artists:
        return "artists";
    case genres:
        return "genres";
    case playlists:
        return "playlists";
    case songs:
        return "songs";
    case folders:
        return "folders";
    default:
        return "none";
    }
}
