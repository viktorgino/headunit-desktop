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


class PluginManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantMap plugins READ getPlugins NOTIFY pluginsUpdated)
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
    bool loadPlugins(QQmlApplicationEngine *engine, bool filter, QStringList filterList);
    void initPlugins();
    QVariantList menuItems;
    QVariantList configItems;
    QVariantMap plugins;
    QMap<QString, QJsonObject> pluginConfigs;
    QList<QPluginLoader *>pluginLoaders;
//    QList<SettingsLoader *>pluginSettings;

    void settingsChanged(QString key, QVariant value);
//    QQmlPropertyMap settings;
    QVariantList settingsItems;

    void loadMenuItems(QQmlApplicationEngine *engine);
    void loadConfigItems(QQmlApplicationEngine *engine);
    void processPluginEvents(QStringList events);
    QVariantMap m_settings;
    QHash<QString, QStringList> connections;

    QVariantMap m_overlays;

    QVariantMap getPlugins(){
        return plugins;
    }

};

#endif // PLUGINMANAGER_H
