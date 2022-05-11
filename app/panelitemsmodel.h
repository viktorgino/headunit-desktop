#ifndef PANELITEMSMODEL_H
#define PANELITEMSMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <QSettings>

#include <hud-common.h>

#include "pluginlistmodel.h"

class PanelItemsModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int length READ rowCount NOTIFY lengthChanged)
    Q_PROPERTY(bool editing READ editing WRITE setEditing NOTIFY editingChanged)
public:
    explicit PanelItemsModel(QObject *parent = 0);

    enum PanelItemsModelRoles { NameRole = Qt::UserRole + 1, LabelRole, SourceRole, PropertiesRole, FillSpaceRole };

    QHash<int, QByteArray> roleNames() const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void setPluginList(PluginList *pluginList);

    void removeUnusedItems();

public slots:
    void insertPluginItem(int position, QString name);
    void move(int from, int to);
    void remove(int index);
signals:
    void lengthChanged();
    void editingChanged();

private:
    PluginList *m_pluginList = nullptr;
    QList<PanelItem> m_modelItems;
    bool m_editing = false;
    void saveList();
    void loadList();
    bool editing();
    void setEditing(bool editing);
};

#endif // PANELITEMSMODEL_H
