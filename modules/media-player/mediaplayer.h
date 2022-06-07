#ifndef MEDIAPLAYER_H
#define MEDIAPLAYER_H

#include <QObject>
#include <plugininterface.h>
#include <mediainterface.h>

#include "mediadb.h"
#include "mediascanner.h"
#include "medialibrarycontainermodel.h"
#include "medialibrarymedialistmodel.h"
#include "mediaplayerplaylistmodel.h"
#include "mediaplayercoverimageprovider.h"

class MediaPlayerPlugin : public QObject, PluginInterface, public MediaInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.viktorgino.headunit.mediaplayer" FILE "config.json")
    Q_INTERFACES(PluginInterface)

    Q_PROPERTY(MediaLibraryContainerModel *ContainerModel MEMBER m_mediaLibraryContainerModel CONSTANT)
    Q_PROPERTY(MediaLibraryMediaListModel *MediaListModel MEMBER m_mediaLibraryMediaListModel CONSTANT)
    Q_PROPERTY(MediaPlayerPlaylistModel *PlaylistModel MEMBER m_mediaLibraryPlaylistModel CONSTANT)
    Q_PROPERTY(QVariantList MediaLocations READ getLocations NOTIFY locationsUpdated)

    Q_PROPERTY(qreal Volume READ getVolume NOTIFY volumeUpdated)
public:
    explicit MediaPlayerPlugin(QObject *parent = nullptr);
    ~MediaPlayerPlugin() override;
    QObject *getContextProperty() override;
    QQuickImageProvider *getImageProvider() override;

    void init() override;

    Q_INVOKABLE QVariantList getLocations();
    Q_INVOKABLE void addLocation(QString path);

    void setMediaVolume(uint8_t volume) override;

    qreal getVolume();
signals:
    void message(QString id, QVariant message);
    void libraryUpdated();
    void mediaNotification(QString id, QString message);
    void locationsUpdated();
    void volumeUpdated();

    void start() override;
    void stop() override;
    void prevTrack() override;
    void nextTrack() override;

    void playbackStarted() override;

public slots:
    void scanningFinished();
    void eventMessage(QString id, QVariant message) override;

private:
    MediaPlayerCoverImageProvider m_imageProvider;
    //TODO: Consider only creating mediaScanner when needed

    MediaLibraryContainerModel *m_mediaLibraryContainerModel = nullptr;
    MediaLibraryMediaListModel  *m_mediaLibraryMediaListModel = nullptr;
    MediaPlayerPlaylistModel *m_mediaLibraryPlaylistModel = nullptr;
    MediaDBManager *m_mediaDbManager = nullptr;

    qreal m_volume;
};

#endif // MEDIAPLAYER_H

