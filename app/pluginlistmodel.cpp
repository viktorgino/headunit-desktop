#include "pluginlistmodel.h"

PluginListProxyModel::PluginListProxyModel(QObject *parent) : QSortFilterProxyModel(parent), m_listModel(this)
{
    setSourceModel(&m_listModel);
}

bool PluginListProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const {
    QModelIndex index0 = sourceModel()->index(sourceRow, 0, sourceParent);

    switch (m_type) {
    case PluginsList:
        return true;
    case MenuItemsList:
        return sourceModel()->data(index0,PluginListModel::MenuRole).toMap().size() > 0;
    case SettingMenuItemsList:
        return sourceModel()->data(index0,PluginListModel::SettingsItemsRole).toMap().size() > 0;
    }
}
void PluginListProxyModel::setPlugins(PluginList *plugins) {
    m_listModel.setPlugins(plugins);
}
void PluginListProxyModel::setType(QString type) {
    if(type == "") {
        m_type = PluginsList;
    } else if(type == "plugin") {
        m_type = PluginsList;
    } else if(type == "all") {
        m_type = PluginsList;
    } else if(type.toLower() == "mainmenu") {
        m_type = MenuItemsList;
    } else if(type.toLower() == "settingsmenu") {
        m_type = SettingMenuItemsList;
    }
    invalidateFilter();
}

PluginListModel::PluginListModel(QObject *parent) : QAbstractListModel(parent)
{

}

QHash<int, QByteArray> PluginListModel::roleNames() const {
    QHash<int, QByteArray> roles;

    roles[NameRole] = "name";
    roles[LabelRole] = "label";
    roles[PluginRole] = "plugin";
    roles[QmlSourceRole] = "qmlSource";
    roles[LoadedRole] = "pluginLoaded";
    roles[MenuRole] = "menu";
    roles[SettingsRole] = "settings";
    roles[SettingsItemsRole] = "settingsItems";
    return roles;
}

int PluginListModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent)
    if(m_plugins) {
        return m_plugins->size();
    }
    return 0;
}

QVariant PluginListModel::data(const QModelIndex &index, int role) const {

    if(!m_plugins) {
        qDebug() << "Invalid plugin";
        return QVariant::Invalid;
    }

    PluginObject * plugin = m_plugins->at(index.row());
    switch ((enum PluginListModelRoles)role) {
    case NameRole:
        return plugin->getName();
    case LabelRole:
        qDebug() << plugin->getLabel();
        return plugin->getLabel();
    case PluginRole:
        return QVariant::fromValue<PluginObject *>(plugin);
    case QmlSourceRole:
        return plugin->getSource();
    case LoadedRole:
        return plugin->getLoaded();
    case MenuRole:
        return plugin->getMenu();
    case SettingsRole:
        return QVariant::fromValue<QQmlPropertyMap *>(plugin->getSettings());
    case SettingsItemsRole:
        return plugin->getSettingsItems();
    }
}

void PluginListModel::setPlugins(PluginList *plugins) {
    if(plugins){
        beginResetModel();
        m_plugins = plugins;
        endResetModel();

        connect(m_plugins,&PluginList::pluginLoaded, [=](const int pluginIndex) {
            emit dataChanged(index(pluginIndex, 0), index(pluginIndex, 0));
        });
    }
}
