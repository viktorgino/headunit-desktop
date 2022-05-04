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

class InitWorker : public QThread
{
    Q_OBJECT
public:
    InitWorker(PluginList *pluginList, MediaManager *mediaManager, PanelItemsModel *bottomBarModel, QObject *parent = nullptr) :
          QThread(parent), m_pluginList(pluginList), m_mediaManager(mediaManager), m_bottomBarModel(bottomBarModel) {}
    void run() override;
private:
    PluginList *m_pluginList;
    MediaManager *m_mediaManager;
    PanelItemsModel *m_bottomBarModel;
};

class PluginManager : public QObject
{
    Q_OBJECT
public:
    explicit PluginManager(QQmlApplicationEngine *engine, ThemeManager * themeManager, QStringList filterList, bool initInThread, QObject *parent = nullptr);

    ~PluginManager();
signals:
    void themeEvent(QString, QString, QVariant);

private slots:
    void messageHandler(QString sender, QString id, QVariant message);
    void actionHandler(QString sender, QString id, QVariant message);
private:

    MediaManager m_mediaManager;
    QHash<QString, QStringList> m_connections;
    PluginList m_pluginList;
    PanelItemsModel m_bottomBarModel;


    PluginObject *settingsMenu;
    PluginObject *themeSettings;
    ThemeManager * m_themeManager;

    bool loadPlugins(QQmlApplicationEngine *engine, QStringList filterList);

};

#endif // PLUGINMANAGER_H
