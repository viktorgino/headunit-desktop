#ifndef MEDIALIBRARYCONTAINERMODEL_H
#define MEDIALIBRARYCONTAINERMODEL_H

#include <QObject>
#include <QAbstractItemModel>

#include "mediadbmanager.h"

class MediaLibraryContainerModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit MediaLibraryContainerModel(MediaDBManager *mediaDBManager, QObject *parent = 0);

    enum MediaLibraryContainerRoles {
        TitleRole = Qt::UserRole + 1,
        SubtitleRole,
        ThumbnailRole,
        FolderRole,
        PathRole
    };

    void init();
    Q_INVOKABLE QHash<int, QByteArray> roleNames() const override;
    Q_INVOKABLE int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    Q_INVOKABLE QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
public slots:
    void setFilter(QString type);
    QVariant getItem(int index);
private:
    MediaDBManager *m_mediaDBManager;
    QList<QVariantMap> m_containerContent;
    MediaDB::ListType m_type;
};

#endif // MEDIALIBRARYCONTAINERMODEL_H
