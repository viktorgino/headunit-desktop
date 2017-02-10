#ifndef MEDIALIBRARY_H
#define MEDIALIBRARY_H

#include <QObject>
#include <QQmlListProperty>
#include <QVariant>

#include "mediascanner.h"
#include "mediadb.h"
class MediaLibrary : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantList audioFolders READ audioFolders CONSTANT)
    Q_PROPERTY(QVariantList playlists READ playlists CONSTANT)
public:
    explicit MediaLibrary(QObject *parent = 0);
    const QVariantList audioFolders();
    const QVariantList playlists();
    Q_INVOKABLE QVariantList audioFolderContent(int folder_id);
    Q_INVOKABLE QVariantList videoFolderContent(int folder_id);
    Q_INVOKABLE QVariantList albumContent(QString album);
    Q_INVOKABLE QVariantList getAlbums();
    Q_INVOKABLE QVariantList getArtists();
    Q_INVOKABLE QVariantList getGenres();
    Q_INVOKABLE QVariantList getPlaylists();
    Q_INVOKABLE QVariantList getSongs();
private:
    MediaScanner *mediaScanner;
    QVariantList p_audioFolders;
    QVariantList p_playlists;
signals:

public slots:
};

#endif // MEDIALIBRARY_H
