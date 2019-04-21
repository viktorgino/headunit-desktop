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

class PluginManager : public QObject
{
    Q_OBJECT
public:
    explicit PluginManager(QQmlApplicationEngine *engine, QObject *parent = nullptr);
    ~PluginManager();
signals:

public slots:
private slots:
    void messageReceived(QString id, QString message);
private:
    bool loadPlugins(QQmlApplicationEngine *engine);
    QVariantList menuItems;
    QVariantList configItems;
    QMap<QString, PluginInterface *> plugins;
    QMap<QString, QJsonObject> pluginConfigs;
    QList<QPluginLoader *>pluginLoaders;
    QList<SettingsLoader *>pluginSettings;

    void settingsChanged(QString key, QVariant value);
//    QQmlPropertyMap settings;
    QVariantList settingsItems;

    void loadMenuItems(QQmlApplicationEngine *engine);
    void loadConfigItems(QQmlApplicationEngine *engine);
    void processPluginEvents(QStringList events);
    QVariantMap m_settings;
    QHash<QString, QStringList> connections;
};

#endif // PLUGINMANAGER_H
