#include "pluginobject.h"

Q_LOGGING_CATEGORY(PLUGINOBJECT, "Plugin Object")

PluginObject::PluginObject(QString fileName, QObject *parent) :
      QObject(parent), m_loaded(false), m_pluginFileName(fileName), m_pluginLoader(fileName, this), m_plugin(nullptr), m_pluginInterface(nullptr), m_settings(nullptr) , m_mediaInterface(nullptr)
{

    if(m_pluginLoader.metaData().value("MetaData").type() != QJsonValue::Object){
        qCDebug(PLUGINOBJECT) << "Invalid plugin : " << fileName << " config missing " << fileName << m_pluginLoader.errorString();
        return;
    }

    QJsonObject pluginMetaData = m_pluginLoader.metaData().value("MetaData").toObject();
    m_name = pluginMetaData.value("name").toString();
    m_plugin = m_pluginLoader.instance();

    if (!m_plugin) {
        qCDebug(PLUGINOBJECT) << "Error loading plugin : " << m_name << m_pluginLoader.errorString();
        return;
    }

    m_pluginInterface = qobject_cast<PluginInterface *>(m_plugin);

    if(!m_pluginInterface){
        qCDebug(PLUGINOBJECT) << "Error loading plugin : " << m_name << ", root component is not a valid instance of PluginInterface";
        return;
    }

    const QMetaObject *metaObject = m_plugin->metaObject();

    QJsonValue menu = pluginMetaData.value("menu");
    if(menu.type() == QJsonValue::Object){
        m_menu = menu.toObject().toVariantMap();
        m_source = m_menu["source"].toString();
        m_label = m_menu["label"].toString();
    }

    for(int i = metaObject->methodOffset(); i < metaObject->methodCount(); ++i){
        if(metaObject->method(i).methodSignature() == "message(QString,QVariant)"){
            connect(m_plugin, SIGNAL(message(QString,QVariant)), this, SLOT(messageHandler(QString,QVariant)));
        }
        if(metaObject->method(i).methodSignature() == "action(QString,QVariant)"){
            connect(m_plugin, SIGNAL(action(QString,QVariant)), this, SLOT(actionHandler(QString,QVariant)));
        }
    }

    QJsonValue config = pluginMetaData.value("config");
    if(config.type() == QJsonValue::Object){
        QJsonObject settingsObject = config.toObject();
        settingsObject.insert("name", m_name);

        SettingsLoader *settings = nullptr;
        if(settingsObject.contains("items")){
            settings = new SettingsLoader(settingsObject, m_pluginInterface->getSettings(), this);
        } else if(settingsObject.contains("settings")) {
            QJsonObject configSettingsMap;
            configSettingsMap.insert("name", m_name);
            configSettingsMap.insert("type", "items");
            configSettingsMap.insert("items", settingsObject["settings"].toArray());
            configSettingsMap.insert("autoSave", settingsObject["settingsAutoSave"].toBool());

            settings = new SettingsLoader(configSettingsMap, m_pluginInterface->getSettings(), this);
        }
        if(settings){
            m_settings = settings->getSettingsMap();
        }
        m_settingsItems = settingsObject.toVariantMap();
    }

    qCDebug(PLUGINOBJECT) << "Plugin loaded : " << m_name;

    m_mediaInterface = dynamic_cast<MediaInterface *>(m_plugin);

    m_pluginInterface->onLoad();
}

PluginObject::PluginObject(QString name, QString label, QObject *parent, QString qmlSource, QVariantMap menu, QVariantMap settingsItems, QQmlPropertyMap * settings) :
      QObject(parent), m_source(qmlSource), m_name(name), m_label(label), m_menu(menu), m_settings(settings), m_settingsItems(settingsItems)
{

}

void PluginObject::init(){
    if(m_pluginInterface)
        m_pluginInterface->init();

    if(m_plugin)
        m_plugin->setParent(this);

    emit loaded();
    m_loaded = true;
    emit loadedChanged();
}
void PluginObject::messageHandler(QString messageId, QVariant parameter) {
    emit message(m_name, messageId, parameter);
}
void PluginObject::actionHandler(QString actionId, QVariant parameter) {
    emit action(m_name, actionId, parameter);
}

void PluginObject::handleMessage(QString id, QVariant message) {
    if(m_pluginInterface)
        m_pluginInterface->eventMessage(id,message);
}
void PluginObject::callAction(QString id, QVariant message){
    if(m_pluginInterface)
        m_pluginInterface->actionMessage(id,message);
}

bool PluginObject::getLoaded() {
    return m_loaded;
}

QString PluginObject::getSource() {
    return m_source;
}

QString PluginObject::getName(){
    return m_name;
}
QString PluginObject::getLabel(){
    return m_label;
}

QVariantMap PluginObject::getMenu(){
    return m_menu;
}

QObject *PluginObject::getContextProperty() {
    if(m_pluginInterface)
        return m_pluginInterface->getContextProperty();
    return nullptr;
}
QQuickImageProvider *PluginObject::getImageProvider() {
    if(m_pluginInterface)
        return m_pluginInterface->getImageProvider();
    return nullptr;
}

QObject *PluginObject::getPlugin(){
    return m_plugin;
}
QQmlPropertyMap *PluginObject::getSettings(){
    return m_settings;
}

QVariantMap PluginObject::getSettingsItems() {
    return m_settingsItems;
}

MediaInterface *PluginObject::getMediaInterface() {
    return m_mediaInterface;
}

void PluginObject::callSlot(QString slot) {
    if(m_plugin) {
        QMetaObject::invokeMethod(m_plugin, slot.toLocal8Bit());
    }
}
