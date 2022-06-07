#ifndef MEDIADBMANAGER_H
#define MEDIADBMANAGER_H

#include <QObject>
#include <QStorageInfo>
#include <QVariant>
#include <QStandardPaths>

#include "mediadb.h"
#include "mediascanner.h"

class MediaDBManager : public QObject
{
    Q_OBJECT
public:
    explicit MediaDBManager(QObject *parent = nullptr);

    void init();

    Q_INVOKABLE QVariantList getLocations();
    Q_INVOKABLE void addLocation(QString path);

    void updateLocations();

    QList<QVariantMap> getMediaContainers(MediaDB::ListType listType);
    QList<QVariantMap> getFolders();
    QList<QVariantMap> getMediaFiles(MediaDB::MediaTypes mediaType = MediaDB::AUDIO, MediaDB::ListType listType = MediaDB::none, QString key = "");
    QList<QVariantMap> getFolderContent(QString folder_id, int mediaType);

public slots:
    void scanningFinishedHandler();
signals:
    void scanningFinished();
private:
    QList<MediaDB *> m_mediaDBs;

    MediaDB *getMediaDB(QString mountPoint);
};

#endif // MEDIADBMANAGER_H
