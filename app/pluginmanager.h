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
#include <QThread>
#include <QHash>
#include <QQmlPropertyMap>
#include <QTextStream>
#include <QJsonParseError>
#include <QLoggingCategory>
#include "../includes/plugininterface.h"
#include "settingsloader.h"
#include "../includes/hvac-common.h"
#include "mediamanager.h"
#include "pluginlistmodel.h"
#include "pluginlist.h"


class InitWorker : public QThread
{
    Q_OBJECT
public:
    InitWorker(PluginList *pluginList, MediaManager *mediaManager, QObject *parent = nullptr) :
          QThread(parent), m_pluginList(pluginList), m_mediaManager(mediaManager) {}
    void run() override;
private:
    PluginList *m_pluginList;
    MediaManager *m_mediaManager;
};

class PluginManager : public QObject
{
    Q_OBJECT
public:
    explicit PluginManager(QQmlApplicationEngine *engine, QStringList filterList, bool initInThread, QObject *parent = nullptr);

    ~PluginManager();
signals:
    void themeEvent(QString, QString, QVariant);

private slots:
    void messageHandler(QString sender, QString id, QVariant message);
    void actionHandler(QString sender, QString id, QVariant message);
private:

    MediaManager m_mediaManager;
    QVariantMap m_settings;
    QHash<QString, QStringList> m_connections;
    PluginList m_pluginList;

    PluginObject *settingsMenu;

    bool loadPlugins(QQmlApplicationEngine *engine, QStringList filterList);

};

#endif // PLUGINMANAGER_H
