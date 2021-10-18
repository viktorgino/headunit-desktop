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
#include <QQmlPropertyMap>
#include <QTextStream>
#include <QJsonParseError>
#include <QLoggingCategory>
#include "../includes/plugininterface.h"
#include "settingsloader.h"
#include "../includes/hvac-common.h"
#include "mediamanager.h"


class PluginManager : public QObject
{
    Q_OBJECT
public:
    explicit PluginManager(QQmlApplicationEngine *engine, bool filter, QStringList filterList, QObject *parent = nullptr);
    ~PluginManager();
signals:
    void themeEvent(QString, QString, QVariant);
    void pluginsUpdated();
public slots:
    QVariant getPluginProperty(QString plugin, QString property);
private slots:
    void messageHandler(QString id, QVariant message);
    void actionHandler(QString id, QVariant message);
private:
    QVariantList m_menuItems;
    QVariantList m_configItems;
    QMap<QString, PluginInterface *> m_plugins;
    QMap<QString, QJsonObject> m_pluginConfigs;
    QList<QPluginLoader *> m_pluginLoaders;
    MediaManager m_mediaManager;
    QVariantList m_settingsItems;
    QVariantMap m_settings;
    QHash<QString, QStringList> m_connections;
    QVariantMap m_overlays;

    bool loadPlugins(QQmlApplicationEngine *engine, bool filter, QStringList filterList);
    void initPlugins();
    void settingsChanged(QString key, QVariant value);
    void loadMenuItems(QQmlApplicationEngine *engine);
    void loadConfigItems();
    void processPluginEvents(QStringList events);
};

#endif // PLUGINMANAGER_H
