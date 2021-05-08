#ifndef MEDIAPLAYER_H
#define MEDIAPLAYER_H

#include <QObject>
#include <plugininterface.h>

#include "mediadb.h"
#include "mediascanner.h"
#include "medialibrarycontainermodel.h"
#include "medialibrarymedialistmodel.h"
#include "mediaplayerplaylistmodel.h"
#include "mediaplayercoverimageprovider.h"

class MediaPlayerPlugin : public QObject, PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.viktorgino.headunit.mediaplayer" FILE "config.json")
    Q_INTERFACES(PluginInterface)

    Q_PROPERTY(MediaLibraryContainerModel *ContainerModel MEMBER m_mediaLibraryContainerModel CONSTANT)
    Q_PROPERTY(MediaLibraryMediaListModel *MediaListModel MEMBER m_mediaLibraryMediaListModel CONSTANT)
    Q_PROPERTY(MediaPlayerPlaylistModel *PlaylistModel MEMBER m_mediaLibraryPlaylistModel CONSTANT)
    Q_PROPERTY(QVariantList MediaLocations READ getLocations NOTIFY locationsUpdated)
public:
    explicit MediaPlayerPlugin(QObject *parent = nullptr);
    ~MediaPlayerPlugin() override;
    QObject *getContextProperty() override;
    QQuickImageProvider *getImageProvider() override;

    void init() override;

    Q_INVOKABLE QVariantList getLocations();
    Q_INVOKABLE void addLocation(QString path);

signals:
    void message(QString id, QVariant message);
    void libraryUpdated();
    void mediaNotification(QString id, QString message);
    void locationsUpdated();

public slots:
    void scanningFinished();

private:
    MediaPlayerCoverImageProvider m_imageProvider;
    //TODO: Consider only creating mediaScanner when needed

    MediaLibraryContainerModel *m_mediaLibraryContainerModel = nullptr;
    MediaLibraryMediaListModel  *m_mediaLibraryMediaListModel = nullptr;
    MediaPlayerPlaylistModel *m_mediaLibraryPlaylistModel = nullptr;
    MediaDBManager *m_mediaDbManager = nullptr;

};

#endif // MEDIAPLAYER_H

