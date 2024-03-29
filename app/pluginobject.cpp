#include "pluginobject.h"

Q_LOGGING_CATEGORY(PLUGINOBJECT, "PluginObject")

PluginObject::PluginObject(QString fileName, QObject *parent) :
      QObject(parent), m_loaded(false), m_pluginFileName(fileName), m_pluginLoader(fileName, this)
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

    if(pluginMetaData.contains("source")) {
        m_source = pluginMetaData.value("source").toString();
    } else {
        m_source = getPropertyValue("source").toString();
        if(!m_source.isEmpty()){
            connectToPropertySignal("source", "sourceChanged");
        }
    }

    m_label = pluginMetaData.value("label").toString();
    m_icon = pluginMetaData.value("icon").toString();

    const QMetaObject *metaObject = m_plugin->metaObject();

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

        SettingsLoader *settings = nullptr;
        if(settingsObject.contains("items")){
            settingsObject.insert("name", m_name);
            settings = new SettingsLoader(settingsObject, m_name, m_pluginInterface->getSettings(), this);
        } else if(settingsObject.contains("settings")) {
            QJsonObject configSettingsMap;
            configSettingsMap.insert("name", m_name);
            configSettingsMap.insert("type", "items");
            configSettingsMap.insert("items", settingsObject["settings"].toArray());
            configSettingsMap.insert("autoSave", settingsObject["settingsAutoSave"].toBool());

            settings = new SettingsLoader(configSettingsMap, m_name, m_pluginInterface->getSettings(), this);
        }
        if(settings){
            m_settings = QVariant::fromValue<QQmlPropertyMap * >(settings->getSettingsMap());
        }
        m_settingsMenu = settingsObject.toVariantMap();
    }

    qCDebug(PLUGINOBJECT) << "Plugin loaded : " << m_name;

    m_mediaInterface = dynamic_cast<MediaInterface *>(m_plugin);

    m_pluginInterface->onLoad();
}

PluginObject::PluginObject(QString name,
                           QString label,
                           QObject *parent,
                           QString icon,
                           QString qmlSource,
                           QVariantMap settingsMenu,
                           QVariant settings, QVariantList bottomBarItems) :
      QObject(parent), m_name(name), m_label(label), m_icon(icon), m_source(qmlSource), m_settings(settings), m_settingsMenu(settingsMenu)
{
    loadBottomBarItems(bottomBarItems);
}

PluginObject::~PluginObject() {
    qDebug() << "Plugin object deleted : " << m_name;
}

void PluginObject::init(){
    if(m_pluginInterface)
        m_pluginInterface->init();

    if(m_plugin) {
        m_plugin->setParent(this);

        QJsonObject pluginMetaData = m_pluginLoader.metaData().value("MetaData").toObject();

        if(pluginMetaData.value("bottomBarItems").isArray()){
            QVariantList bottomBarItems = pluginMetaData.value("bottomBarItems").toArray().toVariantList();
            loadBottomBarItems(bottomBarItems);
        } else {
            QVariantList bottomBarItems = getPropertyValue("bottomBarItems").toList();
            if(bottomBarItems.size() > 0){
                connectToPropertySignal("bottomBarItems", "updateBottomBarItems");
                loadBottomBarItems(bottomBarItems);
            }
        }
    }

    m_loaded = true;
    emit loadedChanged();
}

void PluginObject::connectToPropertySignal(QString propertyName, QString slotName){
    if(m_plugin) {
        const QMetaObject *metaObject = m_plugin->metaObject();

        for(int i = metaObject->propertyOffset(); i < metaObject->propertyCount(); ++i){
            QMetaProperty property =  metaObject->property(i);
            QString name = QString::fromLatin1(property.name());
            if(propertyName == name) {
                if(property.isReadable()) {
                    QMetaMethod notifySignal = m_plugin->metaObject()->method(property.notifySignalIndex());
                    if(notifySignal.isValid()){
                        const QMetaObject *ownMeta = qobject_cast<QObject *>(this)->metaObject();
                        for(int i = ownMeta->methodOffset(); i < ownMeta->methodCount(); ++i){
                            QMetaMethod updateBottomBarItemsMethod =  ownMeta->method(i);
                            if(updateBottomBarItemsMethod.name() == slotName){
                                QMetaMethod signalHandler = ownMeta->method(i);
                                connect(m_plugin, notifySignal, qobject_cast<QObject *>(this), signalHandler);
                            }
                        }
                    } else {
                        qCDebug(PLUGINOBJECT) << "Property " << propertyName << " doesn't have a notifiable signal";
                    }
                } else {
                    qCDebug(PLUGINOBJECT) << "Property " << propertyName << " is not readable";
                }
                break;
            }
        }
    } else {
        qCDebug(PLUGINOBJECT) << "connectToPropertySignal : Plugin not loaded : " << m_name;
    }
}

QVariant PluginObject::getPropertyValue(QString propertyName) {
    if(m_plugin) {
        const QMetaObject *metaObject = m_plugin->metaObject();

        for(int i = metaObject->propertyOffset(); i < metaObject->propertyCount(); ++i){
            QMetaProperty property =  metaObject->property(i);
            QString name = QString::fromLatin1(property.name());
            if(propertyName == name) {
                if(property.isReadable()) {
                    return property.read(m_plugin);
                } else {
                    qCDebug(PLUGINOBJECT) << "Property " << propertyName << " is not readable";
                }
                break;
            }
        }
    } else {
        qCDebug(PLUGINOBJECT) << "getPropertyValue : Plugin not loaded : " << m_name;
    }
    return QVariant();
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
QString PluginObject::getIcon(){
    return m_icon;
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
QVariant PluginObject::getSettings(){
    return m_settings;
}

QVariantMap PluginObject::getSettingsItems() {
    return m_settingsMenu;
}

QList<PanelItem> PluginObject::getBottomBarItems() {
    return m_bottomBarItems;
}

MediaInterface *PluginObject::getMediaInterface() {
    return m_mediaInterface;
}

void PluginObject::callSlot(QString slot) {
    if(m_plugin) {
        QMetaObject::invokeMethod(m_plugin, slot.toLocal8Bit());
    }
}

void PluginObject::updateBottomBarItems() {
    const QMetaObject *metaObject = m_plugin->metaObject();

    for(int i = metaObject->propertyOffset(); i < metaObject->propertyCount(); ++i){
        QMetaProperty property =  metaObject->property(i);
        QString propertyName = QString::fromLatin1(property.name());
        if(propertyName == "bottomBarItems" && property.isReadable()) {
            QVariantList propertyValue = property.read(m_plugin).toList();
            loadBottomBarItems(propertyValue);
        }
    }
}

void PluginObject::loadBottomBarItems(QVariantList bottomBarItems) {
    m_bottomBarItems.clear();
    for(const QVariant &bottomBarItem : bottomBarItems){
        QVariantMap bottomBarItemMap = bottomBarItem.toMap();
        if(bottomBarItemMap.contains("name") && bottomBarItemMap.contains("source") && bottomBarItemMap.contains("label")){

            PanelItem panelItem;

            panelItem.name = QString("%1::%2").arg(m_name).arg(bottomBarItemMap["name"].toString());
            panelItem.source = bottomBarItemMap["source"].toString();
            panelItem.label = bottomBarItemMap["label"].toString();
            panelItem.fillSpace = bottomBarItemMap["fillSpace"].toBool();
            panelItem.properties = bottomBarItemMap["properties"].toMap();
            panelItem.contextProperty = getContextProperty();

            bool itemExists = false;
            for(const PanelItem &item : qAsConst(m_bottomBarItems)){
                if(item.name == panelItem.name) {
                    itemExists = true;
                }
            }
            if(!itemExists) {
                m_bottomBarItems.append(panelItem);
            } else {
                qCDebug(PLUGINOBJECT) << "Error loading plugin bottom bar items " << m_name << ", panel item already exists : " << panelItem.name;
            }
        } else {
            qCDebug(PLUGINOBJECT) << "Error loading plugin bottom bar items " << m_name << ", invalid bottomBarItems Object";
        }
    }
    emit bottomBarItemsUpdated();
}
