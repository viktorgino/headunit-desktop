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

class MediaScanner: public QObject,public QRunnable
{
Q_OBJECT
public:
    MediaScanner();
    virtual void run() override;
    QVariantList getVolumes();
    void addLocation(QString name, QString v_unique_id, QString v_path, QString relative_path);
    void scanLocation(int location_id);
private:
    QList<QString> mediaDirs;
    QStringList audioFileTypes;
    QStringList videoFileTypes;
    QStringList playlistFileTypes;
    QStringList mediaFileTypes;
    QList<QMap<QString, QVariant>> pathsToScan;
    void scanForMediaFiles(QString path, int folder_id);
    void scanForFolders(QString path, bool is_root, int location_id, QString current_dir, qint64 last_modified);
    bool isRunning;
    QString getStorageUUID(QString device);
    MediaDB *mediadb;
};

#endif // MEDIASCANNER_H
