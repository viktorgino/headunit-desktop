#ifndef MEDIALIBRARYMEDIALISTMODEL_H
#define MEDIALIBRARYMEDIALISTMODEL_H

#include <QObject>
#include <QAbstractListModel>

#include <taglib/taglib.h>
#include <taglib/tag.h>
#include <taglib/fileref.h>
#include <taglib/tpropertymap.h>

#include "mediadbmanager.h"

class MediaLibraryMediaListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit MediaLibraryMediaListModel(MediaDBManager *mediaDBManager, QObject *parent = 0);

    enum MediaLibraryMediaListModelRoles {
        TitleRole = Qt::UserRole + 1,
        ArtistRole,
        AlbumRole,
        GenreRole,
        PathRole,
        ThumbnailRole,
        FolderRole
    };

    void init();
    QHash<int, QByteArray> roleNames() const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
public slots:
    void setFilter(QString type, QString key);
    QVariant getItem(int index);
    QVariantList getItems();
protected:
    MediaDBManager *m_mediaDBManager;
    QList<QVariantMap> m_containerContent;
    MediaDB::ListType m_type;

    QList<QVariantMap> getPlaylistContent(QString path);
};

#endif // MEDIALIBRARYMEDIALISTMODEL_H
