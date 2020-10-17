#include "pluginmanager.h"

Q_LOGGING_CATEGORY(PLUGINMANAGER, "Plugin Manager")

PluginManager::PluginManager(QQmlApplicationEngine *engine, bool filter, QStringList filterList, QObject *parent) : QObject(parent)
{
    loadPlugins(engine, filter, filterList);
}
void PluginManager::settingsChanged(QString key, QVariant value){
    qDebug () << "settingsChanged" << key << value;
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
    foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {

        QString fileBaseName=fileName.section(".",0,0);
        if (filter && !filterList.contains(fileBaseName,Qt::CaseInsensitive)) {
            qDebug() << "Plugin not whitelisted (disabled): " << fileBaseName;
            continue;
        }

        QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));

        if(pluginLoader.metaData().value("MetaData").type() != QJsonValue::Object){
            qCDebug(PLUGINMANAGER) << "Invalid plugin : " << fileName << " config missing " << fileName << pluginLoader.errorString();
            continue;
        }

        QObject *plugin = pluginLoader.instance();
        if (!plugin) {
            qCDebug(PLUGINMANAGER) << "Error loading plugin : " << fileName << pluginLoader.errorString();
            continue;
        }

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
            if(pluginMeta->method(i).methodSignature() == "onMessage(QString,QString)"){
                connect(plugin, SIGNAL(onMessage(QString, QString)), this, SLOT(messageReceived(QString, QString)));
            }
        }
        for(QString event : pluginObject->eventListeners()){
            if(connections.contains(event)){
                connections[event].append(pluginMeta->className());
            } else {
                connections.insert(event, QStringList() << pluginMeta->className());
            }
        }

        plugins.insert(pluginMeta->className(), pluginObject);

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
            menuItems << menu.toObject().toVariantMap();
        }

        QJsonValue overlay = metaData.value("overlay");
        if(overlay.type() == QJsonValue::String){
            m_overlays << overlay.toString();
        }

        QJsonValue config = metaData.value("config");
        if(config.type() == QJsonValue::Object){
            QJsonObject settingsObject = config.toObject();
            settingsObject.insert("name", pluginName);

            SettingsLoader *settings = new SettingsLoader(settingsObject, &pluginObject->settings);
            pluginSettings << settings;

            QQmlPropertyMap * settingsMap = settings->getSettingsMap();

            m_settings.insert(pluginName,  QVariant::fromValue<QQmlPropertyMap *>(settingsMap));
            settingsItems.append(settingsObject.toVariantMap());
        }
        pluginLoaders<< &pluginLoader;
	pluginObject->onLoad();
    }
    //Load QML plugins
    pluginsDir.cd("qml");
    foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
	qDebug() << "Loading qml plugin: " << fileName;
        QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));
        QJsonValue uri = pluginLoader.metaData().value("MetaData").toObject().value("uri");
        QList<QQmlError> errors;
        engine->importPlugin(pluginsDir.absoluteFilePath(fileName),uri.toString(),&errors);
    }

    loadConfigItems(engine);
    loadMenuItems(engine);

    engine->rootContext()->setContextProperty("HUDSettingsMenu", settingsItems);
    engine->rootContext()->setContextProperty("HUDSettings", m_settings);
    engine->rootContext()->setContextProperty("HUDOverlays", m_overlays);

    return true;
}

void PluginManager::messageReceived(QString id, QString message){
    QStringList messageId = id.split("::");

    if(messageId.size() == 2 && messageId[0] == "GUI") {
        emit themeEvent(messageId[1], message);
        return;
    }

    QString event = QString("%1::%2").arg(QString(sender()->metaObject()->className())).arg(id);
    if(connections.contains(event)){
        QStringList listeners = connections.value(event);
        for(QString listener : listeners){
            plugins[listener]->eventMessage(event, message);
        }
    }
}
void PluginManager::loadMenuItems(QQmlApplicationEngine *engine){
    menuItems << QJsonObject {{"source","qrc:/qml/Radio/RadioLayout.qml"},{"image","icons/svg/radio-waves.svg"},{"text","Radio"},{"color","#E91E63"}}.toVariantMap()
              << QJsonObject {{"source","qrc:/qml/SettingsPage/SettingsPage.qml"},{"image","icons/svg/gear-a.svg"},{"text","Settings"},{"color","#4CAF50"}}.toVariantMap();
    engine->rootContext()->setContextProperty("menuItems", menuItems);
}

void PluginManager::loadConfigItems(QQmlApplicationEngine *engine){
    settingsItems << QJsonObject {{"name","theme"},{"iconImage","qrc:/qml/icons/android-color-palette.png"},{"label","Theme"},{"type","loader"},{"section","General"},{"source","qrc:/qml/SettingsPage/SettingsPageTheme.qml"}}.toVariantMap()
                  << QJsonObject {{"name","quit"},{"iconImage","qrc:/qml/icons/log-out.png"},{"label","Quit headunit-desktop"},{"type","action"},{"action","Qt.quit()"},{"section","Other"},{"source",""}}.toVariantMap();

    engine->rootContext()->setContextProperty("configItems", configItems);
}

#ifdef HAVE_WELLEIO
void PluginManager::loadWelleIo(QQmlApplicationEngine *engine){
    welleioError = false;
    GUI = nullptr;
    RadioController = nullptr;
    // Default values
    QVariantMap commandLineOptions;
    CDABParams DABParams(1);
    commandLineOptions["dabDevice"] = "auto";
    commandLineOptions["ipAddress"] = "127.0.0.1";
    commandLineOptions["ipPort"] = 1234;
    commandLineOptions["rawFile"] =  "";
    commandLineOptions["rawFileFormat"] = "u8";

    if(!welleioError){
        try {
            // Create a new radio interface instance
            RadioController = new CRadioController(commandLineOptions, DABParams);
            QTimer::singleShot(0, RadioController, SLOT(onEventLoopStarted()));
            GUI = new CGUI(RadioController, &DABParams);
            // Load welle.io's control object to QML context
            engine->rootContext()->setContextProperty("cppGUI", GUI);
            engine->rootContext()->setContextProperty("cppRadioController", RadioController);
            // Add MOT slideshow provider
            engine->addImageProvider(QLatin1String("motslideshow"), GUI->MOTImage);
            // Load the DAB helper class to QML context
            DABHelper *dabHelper = new DABHelper();
            engine->rootContext()->setContextProperty("dabHelper", dabHelper);
        } catch (int e) {
            qCDebug(PLUGINMANAGER)<< "Can't initialise welle.io. Exception: " << e ;
            welleioError = true;
        }
    }
    if(!welleioError){
        menuItems.insert(2,QJsonObject {{"source","qrc:/DABLayout.qml"},{"image","icons/svg/radio-waves.svg"},{"text","DAB"},{"color","#9C27B0"}}.toVariantMap());
    }
}
#endif

PluginManager::~PluginManager(){
    for (QPluginLoader * pluginLoader : pluginLoaders) {
        if(pluginLoader->isLoaded()){
            pluginLoader->unload();
        }
    }

//    for (PluginInterface * plugin : plugins) {
//        delete(plugin);
//    }
    qDeleteAll(plugins);

    for(SettingsLoader * settings : pluginSettings){
        delete(settings);
    }
#ifdef HAVE_WELLEIO
    if(!welleioError){
        delete GUI;
        delete RadioController;
    }
#endif

}
