#include "pluginmanager.h"

Q_LOGGING_CATEGORY(PLUGINMANAGER, "Plugin Manager")

PluginManager::PluginManager(QQmlApplicationEngine *engine, PluginList *pluginList, MediaManager *mediaManager, QObject *parent) :
      QObject(parent), m_mediaManager(mediaManager), m_pluginList(pluginList), m_engine(engine)
{
}

bool PluginManager::loadPlugins(QStringList filterList)
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

        PluginObject * plugin = m_pluginList->addPlugin(pluginsDir.absoluteFilePath(fileName));

        if(plugin->getMediaInterface()){
            qDebug() << "Adding interface" << plugin->getName();
            m_mediaManager->addInterface(plugin->getName(), plugin->getPlugin());
        }


        if(plugin->getImageProvider()){
            m_engine->addImageProvider(plugin->getName(),plugin->getImageProvider());
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
            m_engine->importPlugin(pluginsDir.absoluteFilePath(fileName),uri.toString(),&errors);
        }
    }

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
        m_mediaManager->mediaInput(message.toString());
        return;
    } else if(id == "KeyInput"){
        event = "KeyInput";
        return;
    } else {
        event = QString("%1::%2").arg(sender).arg(id);
    }

    m_pluginList->handleMessage(event, message);
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
            PluginObject *pluginObject = m_pluginList->getPlugin(messageId[0]);
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
