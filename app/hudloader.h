#ifndef HUDLOADER_H
#define HUDLOADER_H

#include <QObject>

#include "panelitemsmodel.h"
#include "pluginmanager.h"
#include "thememanager.h"

class HUDLoader;
class InitThread : public QThread
{
    Q_OBJECT
public:
    explicit InitThread(HUDLoader *hudLoader);
    void run() override;
private:
    HUDLoader *m_hudLoader;
signals:
    void initFinished();
};

class HUDLoader : public QObject
{
    Q_OBJECT
public:
    explicit HUDLoader(QQmlApplicationEngine *engine, bool lazyLoading, QStringList plugins, QObject *parent = nullptr);
    void init();

signals:
    void themeLoaded();

public slots:
    void initFinished();
    void onThemeLoaded();

private:
    QQmlApplicationEngine *m_engine;

    PanelItemsModel *m_bottomBarModel;
    PluginList *m_pluginList;
    ThemeManager *m_themeManager;
    PluginManager *m_pluginManager;
    MediaManager *m_mediaManager;
    InitThread m_initThread;
    QStringList m_plugins;

};

#endif // HUDLOADER_H
