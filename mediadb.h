#ifndef MEDIADB_H
#define MEDIADB_H

#include <QObject>
#include <QtSql>
#include <QString>
#include <QStringList>
#include <QVariant>

class MediaDB : public QObject
{
    Q_OBJECT
public:
    explicit MediaDB(QObject *parent = 0);
    int initDatabase();
    int addLocation(QString name, QString v_unique_id, QString v_path, QString relative_path);
    int addScannedFolder(int location_id, QString relative_path, qint64 last_modified, QString thumbnail);
    int addMediaFiles(QVariantList filenames, QVariantList folder_id, QVariantList media_types);
    QVariantMap getLocationInfo(int location_id);
    int updateFolderMediaType(int folder_id, bool hasAudio, bool hasVideo);
    int setLocationAvailability(int32_t location_id, bool isAvailable);
    QStringList getAvailableLocations();
    QStringList getPlaylists();
    QStringList getAudioFolders();
    QStringList getVideoFolders();

    static const int AUDIO = 1;
    static const int VIDEO = 2;
    static const int PLAYLIST = 3;
private:
    QSqlDatabase db;
signals:

public slots:
};

#endif // MEDIADB_H
