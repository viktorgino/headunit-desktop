#ifndef MEDIAPLAYERPLAYLIST_H
#define MEDIAPLAYERPLAYLIST_H

#include <QObject>
#include <QAbstractListModel>
#include <QSettings>
#include <QFile>
#include <QDebug>
#include <QLoggingCategory>

class MediaPlayerPlaylistModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QVariantList sources READ getSources NOTIFY sourcesChanged)
public:
    explicit MediaPlayerPlaylistModel(QObject *parent = 0);

    enum MediaLibraryContainerContentRoles {
        TitleRole = Qt::UserRole + 1,
        ArtistRole,
        PathRole
    };

    void init();
    QHash<int, QByteArray> roleNames() const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    Q_INVOKABLE void setItems(QVariantList items);
    QVariantList getSources();
signals:
    void sourcesChanged();
private:
    QVariantList m_playlistContent;
    QSettings m_settings;
};

#endif // MEDIAPLAYERPLAYLIST_H
