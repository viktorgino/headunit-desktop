#ifndef MEDIASCANNER_H
#define MEDIASCANNER_H

#include <QObject>
#include <QString>
#include <QRunnable>
#include <QDir>
#include <QDebug>
#include <QDateTime>
#include <QtSql>
#include "mediadb.h"
#include <QStorageInfo>
#include <QVariant>
#include <taglib/taglib.h>
#include <taglib/tag.h>
#include <taglib/fileref.h>
#include <taglib/taglib_config.h>
#include <taglib/tpropertymap.h>

class MediaScanner: public QObject,public QRunnable
{
Q_OBJECT
public:
    MediaScanner();
    virtual void run() override;
    QVariantList getVolumes();
    int addLocation(QString name, QString v_unique_id, QString v_path, QString relative_path);
    void scanLocation(int location_id);
    void updateLocationsAvailability();
    QVariantMap getMusicInfo(QString file);
    MediaDB *mediadb;
signals:
    void scanningStarted();
    void scanningFinished();
private:
    QStringList audioFileTypes;
    QStringList videoFileTypes;
    QStringList playlistFileTypes;
    QStringList imageFileTypes;
    QStringList mediaFileTypes;
    QList<QMap<QString, QVariant>> pathsToScan;
    QString scanForThumbnail(QString path, bool tryParent, QString absPosition);
    void scanForMediaFiles(QString path, int folder_id);
    void scanForFolders(QString path, bool is_root, int location_id, QString current_dir, qint64 last_modified);
    bool isRunning;
    QString getStorageUUID(QString device);
};

#endif // MEDIASCANNER_H
