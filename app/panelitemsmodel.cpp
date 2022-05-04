#include "panelitemsmodel.h"

Q_LOGGING_CATEGORY(PANELITEMSMODEL, "Panel Items Model")

PanelItemsModel::PanelItemsModel(QObject *parent) : QAbstractListModel(parent), m_pluginList(nullptr)
{
    connect(this, &PanelItemsModel::dataChanged, this, &PanelItemsModel::lengthChanged);
}

enum PanelItemsModelRoles { NameRole = Qt::UserRole + 1, LabelRole, SourceRole, PropertiesRole };


QHash<int, QByteArray> PanelItemsModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[LabelRole] = "label";
    roles[SourceRole] = "source";
    roles[PropertiesRole] = "properties";
    roles[FillSpaceRole] = "fillSpace";
    return roles;
}


int PanelItemsModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent)
    return m_modelItems.size();
}

QVariant PanelItemsModel::data(const QModelIndex &index, int role) const {
    PluginObject::PanelItem panelItem = m_modelItems[index.row()];
    switch (role) {
    case NameRole:
        return panelItem.name;
    case LabelRole:
        return panelItem.label;
    case SourceRole:
        return panelItem.source;
    case FillSpaceRole:
        return panelItem.fillSpace;
    case PropertiesRole: {
        QVariantMap properties = panelItem.properties;
        properties.insert("pluginContext", QVariant::fromValue<QObject *>(panelItem.contextProperty));
        return properties;
    }
    default:
        return "";
        break;
    }
}

void PanelItemsModel::setPluginList(PluginList *pluginList) {
    m_pluginList = pluginList;
    loadList();
}

void PanelItemsModel::loadList() {
    QSettings settings;
    QStringList bottomBarItems = settings.value("bottomBarItems").toString().split(",");

    int i = 0;
    for(const QString &item : qAsConst(bottomBarItems)){
        insertPluginItem(i++, item);
    }
}

void PanelItemsModel::move(int from, int to) {
    if(from == to){
        return;
    }

    beginResetModel();
    m_modelItems.move(from,to);
    endResetModel();
    saveList();
}

void PanelItemsModel::remove(int index) {
    beginResetModel();
    m_modelItems.removeAt(index);
    endResetModel();

    saveList();
}

void PanelItemsModel::saveList() {
    QSettings settings;

    QStringList items;

    for(const PluginObject::PanelItem &panelItem : qAsConst(m_modelItems)){
        items << panelItem.name;
    }
    settings.setValue("bottomBarItems", items.join(","));
}
void PanelItemsModel::insertPluginItem(int position, QString name) {
    if(!m_pluginList){
        qCDebug(PANELITEMSMODEL) << "Plugin list uninitialised";
        return;
    }

    QStringList nameList = name.split("::");
    if (nameList.size() != 2) {
        qCDebug(PANELITEMSMODEL) << "Invalid panel item name : " + name;
        return;
    }
    QString pluginName = nameList[0];

    PluginObject *plugin = m_pluginList->getPlugin(pluginName);
    if(!plugin) {
        qCDebug(PANELITEMSMODEL) << "Invalid plugin name : " + pluginName;
        return;
    }

    QList<PluginObject::PanelItem> itemList = plugin->getBottomBarItems();

    PluginObject::PanelItem panelItem;
    for(const PluginObject::PanelItem &item : qAsConst(itemList)){
        if(item.name == name){
            panelItem = item;
            break;
        }
    }

    beginInsertRows(QModelIndex(), position, position);
    m_modelItems.insert(position, panelItem);
    endInsertRows();
    saveList();
}

bool PanelItemsModel::editing(){
    return m_editing;
}
void PanelItemsModel::setEditing(bool editing){
    m_editing = editing;
    emit editingChanged();
}
