#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <QObject>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtQml/QQmlContext>
#include <QPluginLoader>
#include <QDir>
#include <QJsonObject>
#include <QDebug>
#include <QVariant>
#include <QHash>
#include <QTextStream>
#include "includes/plugininterface.h"

class PluginManager : public QObject
{
    Q_OBJECT
public:
    explicit PluginManager(QObject *parent = nullptr);
    ~PluginManager();
    bool loadPlugins(QQmlApplicationEngine *engine);
signals:

public slots:
private slots:
    void messageReceived(QString id, QString message);
private:
    QVariantList menuItems;
    QVariantList configItems;
    QMap<QString, PluginInterface *> plugins;
    QMap<QString, QJsonObject> pluginConfigs;

    void loadMenuItems(QQmlApplicationEngine *engine);
    void loadConfigItems(QQmlApplicationEngine *engine);
    void processPluginEvents(QStringList events);

    QHash<QString, QStringList> connections;
};

#endif // PLUGINMANAGER_H
