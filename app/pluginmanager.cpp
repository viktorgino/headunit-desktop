#include "pluginmanager.h"

Q_LOGGING_CATEGORY(PLUGINMANAGER, "Plugin Manager")

void InitWorker::run(){
    m_pluginList->initPlugins();

    m_mediaManager->init();
}

PluginManager::PluginManager(QQmlApplicationEngine *engine, QStringList filterList, bool initInThread, QObject *parent) :
      QObject(parent), m_mediaManager(this), m_pluginList(this)
{
    qmlRegisterType<PluginListProxyModel>("HUDPlugins", 1, 0, "PluginListModel");
//    qmlRegisterUncreatableType<PluginObject>("HUDPlugins", 1, 0, "PluginListModel", "PluginObject is uncreatable");
    qmlRegisterAnonymousType<PluginObject>("HUDPlugins", 1);

    loadPlugins(engine, filterList);

    QVariantMap settingsMenuMap;
    settingsMenuMap["text"] = "Settings";
    settingsMenuMap["image"] = "icons/svg/gear-a.svg";
    settingsMenuMap["color"] = "#4CAF50";

    settingsMenu = new PluginObject("Settings","Settings", this, "qrc:/qml/HUDSettingsPage/SettingsPage.qml",settingsMenuMap);

    m_pluginList.addPlugin(settingsMenu);

    if(initInThread) {
        InitWorker *workerThread = new InitWorker(&m_pluginList, &m_mediaManager, this);
        workerThread->start();
    } else {
        m_pluginList.initPlugins();
        m_mediaManager.init();
    }
}

bool PluginManager::loadPlugins(QQmlApplicationEngine *engine, QStringList filterList)
{
    QDir pluginsDir(qApp->applicationDirPath());
#if defined(Q_OS_WIN)
    if (pluginsDir.dirName().toLower() == "debug" || pluginsDir.dirName().toLower() == "release")
        pluginsDir.cdUp();
#elif defined(Q_OS_MAC)
    if (pluginsDir.dirName() == "MacOS") {
        pluginsDir.cdUp();
        pluginsDir.cdUp();
        pluginsDir.cdUp();
    }
#endif
    pluginsDir.cd("plugins");
    //Load plugins
    for (const QString &fileName : pluginsDir.entryList(QDir::Files)) {

        QString fileBaseName=fileName.section(".",0,0);
        if (filterList.size() > 0 && !filterList.contains(fileBaseName,Qt::CaseInsensitive)) {
            qDebug() << "Plugin not whitelisted (disabled): " << fileBaseName;
            continue;
        }

        PluginObject * plugin = m_pluginList.addPlugin(pluginsDir.absoluteFilePath(fileName));

        if(plugin->getMediaInterface()){
            qDebug() << "Adding interface" << plugin->getName();
            m_mediaManager.addInterface(plugin->getName(), plugin->getPlugin());
        }


        if(plugin->getImageProvider()){
            engine->addImageProvider(plugin->getName(),plugin->getImageProvider());
        }

        connect(plugin, &PluginObject::action, this, &PluginManager::actionHandler);
        connect(plugin, &PluginObject::message, this, &PluginManager::messageHandler);

    }
    //Load QML plugins
    if(pluginsDir.cd("qml")){
        for (const QString &fileName : pluginsDir.entryList(QDir::Files)) {
            qDebug() << "Loading qml plugin: " << fileName;
            QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));
            QJsonValue uri = pluginLoader.metaData().value("MetaData").toObject().value("uri");
            QList<QQmlError> errors;
            engine->importPlugin(pluginsDir.absoluteFilePath(fileName),uri.toString(),&errors);
        }
    }

    engine->rootContext()->setContextProperty("HUDPlugins", &m_pluginList);
    engine->rootContext()->setContextProperty("HUDMediaManager", &m_mediaManager);

    return true;
}

void PluginManager::messageHandler(QString sender, QString id, QVariant message){
    QStringList messageId = id.split("::");

    QString event;

    if(messageId.size() == 2 && messageId[0] == "GUI") {
        emit themeEvent(sender,messageId[1], message);
        return;
    } else if(messageId.size() == 2 && messageId[0] == "SYSTEM") {
        event = id;
    } else if(id == "MediaInput"){
        m_mediaManager.mediaInput(message.toString());
        return;
    } else if(id == "KeyInput"){
        event = "KeyInput";
        return;
    } else {
        event = QString("%1::%2").arg(sender).arg(id);
    }

    m_pluginList.handleMessage(event, message);
}

void PluginManager::actionHandler(QString sender, QString id, QVariant message){
    QStringList messageId = id.split("::");

    if(messageId.size() == 2){
        if(messageId[0] == "GUI") {

            emit themeEvent(sender,messageId[1], message);

        } else if(messageId[0] == "SYSTEM") {

            emit themeEvent(sender,messageId[1], message);
            messageHandler(sender, id, message);

        } else {
            PluginObject *pluginObject = m_pluginList.getPlugin(messageId[0]);
            if(pluginObject) {
                pluginObject->callAction(messageId[1], message);
            } else {
                qWarning() << "Invalid plugin object " << id;
            }
        }
    } else {
        qWarning () << "actionHandler() : Action Handler id";
    }
}

PluginManager::~PluginManager(){
    qCDebug(PLUGINMANAGER) << "Plugin manager died";
}
