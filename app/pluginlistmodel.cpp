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
        return !sourceModel()->data(index0,PluginListModel::QmlSourceRole).toString().isEmpty();
    case SettingMenuList:
        return sourceModel()->data(index0,PluginListModel::SettingsMenuRole).toMap().size() > 0;
    case BottomBarItemsList:
        return sourceModel()->data(index0,PluginListModel::BottomBarItemsRole).toList().size() > 0;
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
        m_type = SettingMenuList;
    } else if (type.toLower() == "bottombar") {
        m_type = BottomBarItemsList;
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
    roles[IconRole] = "icon";
    roles[QmlSourceRole] = "qmlSource";
    roles[LoadedRole] = "pluginLoaded";
    roles[ContextPropertyRole] = "contextProperty";
    roles[SettingsRole] = "settings";
    roles[SettingsMenuRole] = "settingsMenu";
    roles[BottomBarItemsRole] = "bottomBarItems";
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
        return plugin->getLabel();
    case IconRole:
        return plugin->getIcon();
    case QmlSourceRole:
        return plugin->getSource();
    case LoadedRole:
        return plugin->getLoaded();
    case ContextPropertyRole:
        return QVariant::fromValue<QObject *>(plugin->getContextProperty());
    case SettingsRole:
        return plugin->getSettings();
    case SettingsMenuRole:
        return plugin->getSettingsItems();
    case BottomBarItemsRole :
        QList<PanelItem> panelItems = plugin->getBottomBarItems();

        QVariantList bottomBarItems;

        for (const PanelItem &panelItem : qAsConst(panelItems)) {
            QVariantMap item;
            item.insert("name", panelItem.name);
            item.insert("label", panelItem.label);
            bottomBarItems.append(item);
        }
        return bottomBarItems;
    }
}

void PluginListModel::onDataChanged() {
    PluginObject * sender = qobject_cast<PluginObject *> (QObject::sender());
    if(sender) {
        int pluginIndex = m_plugins->indexOf(sender);
        emit dataChanged(index(pluginIndex, 0), index(pluginIndex, 0));
    }
}

void PluginListModel::setPlugins(PluginList *plugins) {
    if(plugins){
        beginResetModel();
        m_plugins = plugins;
        endResetModel();

        for(int i = 0 ; i < m_plugins->size(); i++){
            PluginObject * plugin = m_plugins->at(i);

            connect(plugin,&PluginObject::loadedChanged, this, &PluginListModel::onDataChanged);
            connect(plugin,&PluginObject::sourceChanged, this, &PluginListModel::onDataChanged);
        }
        connect(m_plugins,&PluginList::pluginAdded, [=](const int pluginIndex) {
            beginInsertRows(QModelIndex(), pluginIndex, pluginIndex);
            endInsertRows();
        });
    }
}
