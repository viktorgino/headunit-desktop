#ifndef PLUGINLIST_H
#define PLUGINLIST_H

#include <QAbstractListModel>
#include <QObject>
#include "pluginobject.h"

class PluginList : public QObject
{
    Q_OBJECT
public:
    explicit PluginList(QObject *parent = nullptr);

    void initPlugins();

    bool containsPlugin(QString pluginName);
    PluginObject *at(int index);
    int size();
    int indexOf(PluginObject * object);
    PluginObject *getPlugin(QString pluginName);
    PluginObject *addPlugin(QString pluginPath);
    void addPlugin(PluginObject *plugin);

    void handleMessage(QString id, QVariant message);

    Q_INVOKABLE void callSlot(QString pluginName, QString slot);
signals:
    void pluginLoaded(int index);
private slots:
    void pluginLoadedHandler();
private:
    QVector<PluginObject *> m_plugins;
};

#endif // PLUGINLIST_H
