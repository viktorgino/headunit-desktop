#ifndef PLUGINLISTMODEL_H
#define PLUGINLISTMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <QSortFilterProxyModel>
#include "pluginobject.h"
#include "pluginlist.h"

class PluginListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit PluginListModel(QObject *parent = nullptr);

    enum PluginListModelRoles {
        NameRole = Qt::UserRole + 1,
        LabelRole,
        IconRole,
        QmlSourceRole,
        LoadedRole,
        ContextPropertyRole,
        SettingsRole,
        SettingsMenuRole
    };

    Q_INVOKABLE QHash<int, QByteArray> roleNames() const override;
    Q_INVOKABLE int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    Q_INVOKABLE QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void setPlugins(PluginList *plugins);
    void setType(QString type);

private:
    PluginList *m_plugins;
};

class PluginListProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(PluginList * plugins WRITE setPlugins)
    Q_PROPERTY(QString listType WRITE setType)
public:

    enum PluginListType {
        PluginsList,
        MenuItemsList,
        SettingMenuList
    };
    explicit PluginListProxyModel(QObject *parent = nullptr);

    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

    void setPlugins(PluginList *plugins);
    void setType(QString type);

private:
    PluginListType m_type;
    PluginListModel m_listModel;

};

#endif // PLUGINLISTMODEL_H
