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
#include "panelitemsmodel.h"
#include "thememanager.h"

class PluginManager : public QObject
{
    Q_OBJECT
public:
    explicit PluginManager(QQmlApplicationEngine *engine, PluginList *pluginList, MediaManager *mediaManager, QObject *parent = nullptr);

    ~PluginManager();
    bool loadPlugins(QStringList filterList);
signals:
    void themeEvent(QString, QString, QVariant);

private slots:
    void messageHandler(QString senderStr, QString id, QVariant message);
    void actionHandler(QString senderStr, QString id, QVariant message);
private:

    MediaManager *m_mediaManager;
    QHash<QString, QStringList> m_connections;
    PluginList *m_pluginList;
    QQmlApplicationEngine *m_engine;
};

#endif // PLUGINMANAGER_H
