#include "pluginmanager.h"

Q_LOGGING_CATEGORY(PLUGINMANAGER, "Plugin Manager")

PluginManager::PluginManager(QQmlApplicationEngine *engine, bool filter, QStringList filterList, QObject *parent) : QObject(parent)
{
    loadPlugins(engine, filter, filterList);
    initPlugins();
}

void PluginManager::settingsChanged(QString key, QVariant value){
    qDebug () << "settingsChanged" << key << value;
}

void PluginManager::initPlugins(){
    for(QVariant plugin : plugins){
        QObject * plug = plugin.value<QObject *>();
        PluginInterface * pluginObject = qobject_cast<PluginInterface *>(plug);
        pluginObject->init();
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
    foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {

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
//        plugin->setParent(this);
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
            if(pluginMeta->method(i).methodSignature() == "message(QString,QVariant)"){
                connect(plugin, SIGNAL(message(QString, QVariant)), this, SLOT(messageHandler(QString, QVariant)));
            }
            if(pluginMeta->method(i).methodSignature() == "action(QString,QVariant)"){
                connect(plugin, SIGNAL(action(QString, QVariant)), this, SLOT(actionHandler(QString, QVariant)));
            }
        }
        PluginSettings *pluginSettings = pluginObject->getPluginSettings();
        for(QString event : pluginSettings->eventListeners){
            if(connections.contains(event)){
                connections[event].append(pluginMeta->className());
            } else {
                connections.insert(event, QStringList() << pluginMeta->className());
            }
        }

        plugins.insert(pluginName, QVariant::fromValue(plugin));

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
    engine->rootContext()->setContextProperty("HUDPlugins", this);

    emit pluginsUpdated();
    return true;
}

void PluginManager::messageHandler(QString id, QVariant message){
    QStringList messageId = id.split("::");

    QString senderName = sender()->metaObject()->className();

    if(messageId.size() == 2 && messageId[0] == "GUI") {
        emit themeEvent(senderName,messageId[1], message);
        return;
    }
    QString event = QString("%1::%2").arg(senderName).arg(id);

    if(connections.contains(event)){
        QStringList listeners = connections.value(event);
        for(QString listener : listeners){
            QObject *plugin = plugins[listener].value<QObject *>();
            PluginInterface * pluginObject = qobject_cast<PluginInterface *>(plugin);
            pluginObject->eventMessage(event, message);
        }
    }
}

void PluginManager::actionHandler(QString id, QVariant message){
    QStringList messageId = id.split("::");

    if(messageId.size() == 2){
        if(messageId[0] == "GUI") {

            QString senderName = sender()->metaObject()->className();
            emit themeEvent(senderName,messageId[1], message);

        } else if(plugins.keys().contains(messageId[0])){

            QObject *plugin = plugins[messageId[0]].value<QObject *>();
            PluginInterface *pluginObject = qobject_cast<PluginInterface *>(plugin);
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
    menuItems << QJsonObject {{"source","qrc:/qml/SettingsPage/SettingsPage.qml"},{"image","icons/svg/gear-a.svg"},{"text","Settings"},{"color","#4CAF50"}}.toVariantMap();
    engine->rootContext()->setContextProperty("menuItems", menuItems);
}

void PluginManager::loadConfigItems(QQmlApplicationEngine *engine){
    settingsItems << QJsonObject {{"name","theme"},{"iconImage","qrc:/qml/icons/android-color-palette.png"},{"label","Theme"},{"type","loader"},{"section","General"},{"source","qrc:/qml/SettingsPage/SettingsPageTheme.qml"}}.toVariantMap()
                  << QJsonObject {{"name","quit"},{"iconImage","qrc:/qml/icons/log-out.png"},{"label","Quit headunit-desktop"},{"type","action"},{"action","Qt.quit()"},{"section","Other"},{"source",""}}.toVariantMap();

    engine->rootContext()->setContextProperty("configItems", configItems);
}


QVariant PluginManager::getPluginProperty(QString plugin, QString property){
    if(plugins.keys().contains(plugin)){
        QObject *pluginObj = plugins[plugin].value<QObject *>();

        QObject * pluginContext = qobject_cast<PluginInterface *>(pluginObj)->getContextProperty();
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
