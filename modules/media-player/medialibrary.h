#ifndef MEDIALIBRARY_H
#define MEDIALIBRARY_H

#include <QObject>
#include <QQmlListProperty>
#include <QVariant>
#include <QDebug>
#include <taglib/taglib.h>
#include <taglib/tag.h>
#include <taglib/fileref.h>
#include <taglib/taglib_config.h>
#include <taglib/tpropertymap.h>

#include "mediascanner.h"
#include "mediadb.h"
class MediaLibrary : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantMap audioFolders READ audioFolders CONSTANT)
    Q_PROPERTY(QVariantMap playlists READ playlists CONSTANT)
    Q_PROPERTY(QVariantMap albums READ getAlbums NOTIFY libraryUpdated)
    Q_PROPERTY(QVariantMap artists READ getArtists NOTIFY libraryUpdated)
    Q_PROPERTY(QVariantMap genres READ getGenres NOTIFY libraryUpdated)
    Q_PROPERTY(QVariantMap songs READ getSongs NOTIFY libraryUpdated)
public:
    explicit MediaLibrary(QObject *parent = nullptr);
    const QVariantMap audioFolders();
    const QVariantMap playlists();
    Q_INVOKABLE QVariantMap audioFolderContent(int folder_id);
    Q_INVOKABLE QVariantMap videoFolderContent(int folder_id);
    Q_INVOKABLE QVariantMap albumContent(QString album);
    Q_INVOKABLE QVariantList getLocations();
    Q_INVOKABLE QVariantMap getAlbums();
    Q_INVOKABLE QVariantMap getArtists();
    Q_INVOKABLE QVariantMap getGenres();
    Q_INVOKABLE QVariantMap getSongs();
    Q_INVOKABLE QVariantMap getAlbumContent(QString key);
    Q_INVOKABLE QVariantMap getArtistContent(QString key);
    Q_INVOKABLE QVariantMap getGenreContent(QString key);
    Q_INVOKABLE QVariantMap getPlaylistContent(QString key);
    Q_INVOKABLE QVariantMap getSongContent(QString key);
    Q_INVOKABLE QVariantList getPlaylistContent(QString path, QString name);
    Q_INVOKABLE QVariantList getMountedVolumes();
    Q_INVOKABLE void addLocation(QString path);
signals:
    void libraryUpdated();
    void mediaNotification(QString id, QString message);
public slots:
    void scanningFinished();
private:
    //TODO: Consider only creating mediaScanner when needed
    MediaScanner *mediaScanner;
    QVariantMap p_audioFolders;
    QVariantMap p_playlists;
    QVariantMap getPlaylists();
};

#endif // MEDIALIBRARY_H
