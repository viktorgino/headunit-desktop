#include "hudloader.h"

Q_LOGGING_CATEGORY(HUDLOADER, "HUDLoader")

InitThread::InitThread(HUDLoader *hudLoader)
    : QThread(hudLoader)
    , m_hudLoader(hudLoader)
{
}
void InitThread::run()
{
    m_hudLoader->init();
    emit initFinished();
}

HUDLoader::HUDLoader(QQmlApplicationEngine *engine, bool lazyLoading, QStringList plugins, QObject *parent)
    : QObject(parent)
    , m_engine(engine)
    , m_initThread(this)
    , m_plugins(plugins)
{
    qCDebug(HUDLOADER) << "Loading Plugin List";
    m_pluginList = new PluginList(this);

    qCDebug(HUDLOADER) << "Loading Media Manager";
    m_mediaManager = new MediaManager();

    qCDebug(HUDLOADER) << "Loading Bottom Bar Model";
    m_bottomBarModel = new PanelItemsModel();

    qCDebug(HUDLOADER) << "Loading Theme";
    m_themeManager = new ThemeManager(m_engine, m_pluginList);

    qCDebug(HUDLOADER) << "Loading plugins";
    m_pluginManager = new PluginManager(m_engine, m_pluginList, m_mediaManager, this);

    qCDebug(HUDLOADER) << "Registering types";
    qmlRegisterType<PluginListProxyModel>("HUDPlugins", 1, 0, "PluginListModel");
    qmlRegisterAnonymousType<PluginObject>("HUDPlugins", 1);
    qmlRegisterSingletonInstance("HUDPlugins", 1, 0, "BottomBarModel", m_bottomBarModel);
    m_engine->rootContext()->setContextProperty("HUDPlugins", m_pluginList);
    m_engine->rootContext()->setContextProperty("HUDMediaManager", m_mediaManager);

    QObject::connect(m_pluginManager, &PluginManager::themeEvent, m_themeManager, &ThemeManager::onEvent);

    connect(&m_initThread, &InitThread::initFinished, this, &HUDLoader::initFinished);
    connect(this, &HUDLoader::themeLoaded, this, &HUDLoader::onThemeLoaded);
    m_pluginManager->loadPlugins(m_plugins);
    if (lazyLoading) {
        qCDebug(HUDLOADER) << "Loading in a thread";
        m_initThread.start();
    } else {
        qCDebug(HUDLOADER) << "Loading on main thread";
        init();
        emit initFinished();
    }
}

void HUDLoader::init()
{
    qCDebug(HUDLOADER) << "Init theme";
    m_themeManager->initTheme("default-theme");
    emit themeLoaded();
    qCDebug(HUDLOADER) << "Init plugins";
    m_pluginList->initPlugins();
    m_mediaManager->init();
}
void HUDLoader::onThemeLoaded()
{
    m_themeManager->initFinished();
}

void HUDLoader::initFinished()
{
    qCDebug(HUDLOADER) << "Setting bottom bar plugin list";
    m_bottomBarModel->setPluginList(m_pluginList);
    m_bottomBarModel->removeUnusedItems();
}
