#include "pluginmanager.h"

Q_LOGGING_CATEGORY(PLUGINMANAGER, "Plugin Manager")

PluginManager::PluginManager(QQmlApplicationEngine *engine, bool filter, QStringList filterList, QObject *parent) : QObject(parent), m_mediaManager(this)
{
    loadPlugins(engine, filter, filterList);
    initPlugins();
    m_mediaManager.init();
}

void PluginManager::settingsChanged(QString key, QVariant value){
    qDebug () << "settingsChanged" << key << value;
}

void PluginManager::initPlugins(){
    for(PluginInterface * plugin : m_plugins){
        plugin->init();
    }
}

bool PluginManager::loadPlugins(QQmlApplicationEngine *engine, bool filter, QStringList filterList)
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
    for (QString fileName : pluginsDir.entryList(QDir::Files)) {

        QString fileBaseName=fileName.section(".",0,0);
        if (filter && !filterList.contains(fileBaseName,Qt::CaseInsensitive)) {
            qDebug() << "Plugin not whitelisted (disabled): " << fileBaseName;
            continue;
        }

        QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName), this);

        if(pluginLoader.metaData().value("MetaData").type() != QJsonValue::Object){
            qCDebug(PLUGINMANAGER) << "Invalid plugin : " << fileName << " config missing " << fileName << pluginLoader.errorString();
            continue;
        }

        QObject *plugin = pluginLoader.instance();

        if (!plugin) {
            qCDebug(PLUGINMANAGER) << "Error loading plugin : " << fileName << pluginLoader.errorString();
            continue;
        }

        plugin->setParent(this);

        PluginInterface * pluginObject = qobject_cast<PluginInterface *>(plugin);

        if(!pluginObject){
            qCDebug(PLUGINMANAGER) << "Error loading plugin : " << fileName << ", root component is not a valid instance of PluginInterface";
            continue;
        }

        qCDebug(PLUGINMANAGER) << "Plugin loaded : " << fileName;

        QJsonObject metaData = pluginLoader.metaData().value("MetaData").toObject();
        QString pluginName = metaData.value("name").toString();
        const QMetaObject *pluginMeta = plugin->metaObject();

        QStringList methods;
        for(int i = pluginMeta->methodOffset(); i < pluginMeta->methodCount(); ++i){
            if(pluginMeta->method(i).methodSignature() == "message(QString,QVariant)"){
                connect(plugin, SIGNAL(message(QString, QVariant)), this, SLOT(messageHandler(QString, QVariant)));
            }
            if(pluginMeta->method(i).methodSignature() == "action(QString,QVariant)"){
                connect(plugin, SIGNAL(action(QString, QVariant)), this, SLOT(actionHandler(QString, QVariant)));
            }
        }

        PluginSettings *pluginSettings = pluginObject->getPluginSettings();
        for(QString event : pluginSettings->eventListeners){
            if(m_connections.contains(event)){
                m_connections[event].append(pluginName);
            } else {
                m_connections.insert(event, QStringList() << pluginName);
            }
        }

        m_plugins.insert(pluginName, pluginObject);

        QObject * contextProperty = pluginObject->getContextProperty();
        if(contextProperty){
            engine->rootContext()->setContextProperty(pluginName,contextProperty);
        }

        QQuickImageProvider *imageProvider = pluginObject->getImageProvider();
        if(imageProvider && metaData.contains("imageProvider")){
            engine->addImageProvider(metaData.value("imageProvider").toString(),imageProvider);
        }

        QJsonValue menu = metaData.value("menu");
        if(menu.type() == QJsonValue::Object){
            m_menuItems << menu.toObject().toVariantMap();
        }

        QJsonValue overlay = metaData.value("overlay");
        if(overlay.type() == QJsonValue::String){
            m_overlays.insert(pluginName,overlay.toString());
        }

        QJsonValue config = metaData.value("config");
        if(config.type() == QJsonValue::Object){
            QJsonObject settingsObject = config.toObject();
            settingsObject.insert("name", pluginName);

            SettingsLoader *settings = new SettingsLoader(settingsObject, pluginObject->getSettings(), this);
//            pluginSettings << settings;

            QQmlPropertyMap * settingsMap = settings->getSettingsMap();

            m_settings.insert(pluginName,  QVariant::fromValue<QQmlPropertyMap *>(settingsMap));
            m_settingsItems.append(settingsObject.toVariantMap());
        }

        MediaInterface * mediaInterface = dynamic_cast<MediaInterface *>(plugin);

        if(mediaInterface){
            QString interfaceName = metaData.value("displayName").toString();
            m_mediaManager.addInterface(interfaceName.isEmpty() ? pluginName : interfaceName, plugin);
        }

        m_pluginLoaders << &pluginLoader;
        pluginObject->onLoad();
    }
    //Load QML plugins
    pluginsDir.cd("qml");
    for (QString fileName : pluginsDir.entryList(QDir::Files)) {
        qDebug() << "Loading qml plugin: " << fileName;
        QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));
        QJsonValue uri = pluginLoader.metaData().value("MetaData").toObject().value("uri");
        QList<QQmlError> errors;
        engine->importPlugin(pluginsDir.absoluteFilePath(fileName),uri.toString(),&errors);
    }

    loadConfigItems(engine);
    loadMenuItems(engine);

    engine->rootContext()->setContextProperty("HUDSettingsMenu", m_settingsItems);
    engine->rootContext()->setContextProperty("HUDSettings", m_settings);
    engine->rootContext()->setContextProperty("HUDOverlays", m_overlays);
    engine->rootContext()->setContextProperty("HUDPlugins", this);
    engine->rootContext()->setContextProperty("HUDMediaManager", &m_mediaManager);

    emit pluginsUpdated();
    return true;
}

void PluginManager::messageHandler(QString id, QVariant message){
    QStringList messageId = id.split("::");


    PluginInterface * pluginObject = qobject_cast<PluginInterface *>(sender());
    if(!pluginObject){
        qWarning () << "messageHandler() : Sender is not a PluginInterface : " << sender()->metaObject()->className();
        return;
    }

    QString senderName = m_plugins.key(pluginObject);
    QString event;

    if(messageId.size() == 2 && messageId[0] == "GUI") {
        emit themeEvent(senderName,messageId[1], message);
        return;
    } else if(id == "MediaInput"){
        if(message == "Next"){
            m_mediaManager.nextTrack();
        } else if(message == "Previous"){
            m_mediaManager.prevTrack();
        }
    } else if(id == "KeyInput"){
        event = "KeyInput";
    } else {
        event = QString("%1::%2").arg(senderName).arg(id);
    }

    if(m_connections.contains(event)){
        QStringList listeners = m_connections.value(event);
        for(QString listener : listeners){
            PluginInterface * receiverObject = m_plugins[listener];
            if(receiverObject){
                receiverObject->eventMessage(event, message);
            } else {
                qWarning () << "messageHandler() : Invalid plugin object " << listener << id;
            }
        }
    }
}

void PluginManager::actionHandler(QString id, QVariant message){
    QStringList messageId = id.split("::");

    if(messageId.size() == 2){
        if(messageId[0] == "GUI") {
            PluginInterface * pluginObject = qobject_cast<PluginInterface *>(sender());
            if(!pluginObject){
                qWarning () << "actionHandler() : Sender is not a PluginInterface : " << sender()->metaObject()->className();
                return;
            }

            QString senderName = m_plugins.key(pluginObject);

            emit themeEvent(senderName,messageId[1], message);

        } else if(m_plugins.keys().contains(messageId[0])){
            PluginInterface *pluginObject = m_plugins[messageId[0]];

            PluginSettings *pluginSettings = pluginObject->getPluginSettings();

            if(pluginSettings->actions.contains(messageId[1])){
                pluginObject->actionMessage(messageId[1], message);
            } else {
                qWarning () << "actionHandler() : Invalid action";
            }
        } else {
            qWarning () << "actionHandler() : Invalid plugin";
        }
    } else {
        qWarning () << "actionHandler() : Action Handler id";
    }
}

void PluginManager::loadMenuItems(QQmlApplicationEngine *engine){
    m_menuItems << QJsonObject {{"source","qrc:/qml/SettingsPage/SettingsPage.qml"},{"image","icons/svg/gear-a.svg"},{"text","Settings"},{"color","#4CAF50"}}.toVariantMap();
    engine->rootContext()->setContextProperty("menuItems", m_menuItems);
}

void PluginManager::loadConfigItems(QQmlApplicationEngine *engine){
    m_settingsItems << QJsonObject {{"name","theme"},{"iconImage","qrc:/qml/icons/android-color-palette.png"},{"label","Theme"},{"type","loader"},{"section","General"},{"source","qrc:/qml/SettingsPage/SettingsPageTheme.qml"}}.toVariantMap()
                    << QJsonObject {{"name","quit"},{"iconImage","qrc:/qml/icons/log-out.png"},{"label","Quit headunit-desktop"},{"type","action"},{"action","Qt.quit()"},{"section","Other"},{"source",""}}.toVariantMap();

    engine->rootContext()->setContextProperty("configItems", m_configItems);
}


QVariant PluginManager::getPluginProperty(QString plugin, QString property){
    if(m_plugins.keys().contains(plugin)){
        QObject * pluginContext = m_plugins[plugin]->getContextProperty();
        QVariant ret;

        ret = pluginContext->property(property.toLocal8Bit());
        if(!ret.isValid()){
            qCWarning(PLUGINMANAGER) << "Invalid property name when getting property : " << plugin << " : " << property;
        }

        return ret;


    } else {
        qCWarning(PLUGINMANAGER) << "Invalid plugin name when getting property : " << plugin;
    }
    return QVariant();
}


PluginManager::~PluginManager(){
    qCDebug(PLUGINMANAGER) << "Plugin manager died";
}
