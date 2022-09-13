#include "thememanager.h"

Q_LOGGING_CATEGORY(THEMEMANAGER, "Theme Manager")


ThemeManager::ThemeManager(QQmlApplicationEngine *engine, PluginList *pluginList, QObject *parent) : QObject(parent),
      m_engine(engine), m_themeLoader(this), m_pluginList(pluginList), m_colorsMap(this), m_sizesMap(this)
{
    m_engine->rootContext()->setContextProperty("ThemeManager", this);

}

void ThemeManager::initFinished() {

    QJsonObject themeSettings = m_themeLoader.metaData().value("MetaData").toObject();

    processThemeSettings(themeSettings);
    m_themePlugin->registerTypes("");
    m_themePlugin->initializeEngine(m_engine, "");

    m_engine->rootContext()->setContextProperty("HUDStyle", HUDStyle);

    if(themeSettings.contains("source")) {
        m_themeSource = themeSettings.value("source").toString();
    }
    emit themeSourceChanged();

    qCDebug(THEMEMANAGER) << "Theme init finished";
}
void ThemeManager::initTheme(QString themeName) {
    QDir themeDir(QCoreApplication::applicationDirPath()+"/themes");

    if(!themeDir.cd(themeName)){
        qCDebug(THEMEMANAGER) << "Error loading plugin, plugin doesn't exists" << themeDir.absolutePath();
        return;
    }
    themeDir.setNameFilters(QStringList() << "*-theme.so");

    QFileInfoList themeLibraryFiles = themeDir.entryInfoList();

    if(themeLibraryFiles.size() == 0){
        qCDebug(THEMEMANAGER) << "No library in theme folder" << themeDir.absolutePath();
        return;
    }

    QString fileName = themeLibraryFiles[0].absoluteFilePath();

    m_themeLoader.setFileName(fileName);

    m_themeObject = m_themeLoader.instance();

    m_themePlugin = static_cast<QQmlExtensionPlugin *>(m_themeObject);

    if(!m_themePlugin){
        qCDebug(THEMEMANAGER) << "Error loading theme : " << m_themeLoader.metaData().value("name") << ", root component is not a valid instance of QQmlExtensionPlugin";
        return;
    }

    m_engine->addImportPath(themeDir.absolutePath());

    QJsonObject themeSettings = m_themeLoader.metaData().value("MetaData").toObject();

    QJsonValue bottomBarItems = themeSettings.value("bottomBarItems");
    if(bottomBarItems.isArray()){
        m_bottomBarItems = bottomBarItems.toArray().toVariantList();
    }

    QVariantMap themeSettingsItems;
    themeSettingsItems["type"] = "loader";
    themeSettingsItems["source"] = themeSettings.value("settingsPageSource").toString();
    m_themeSettings = new PluginObject("ThemeSettings", "Theme", nullptr, "",  "", themeSettingsItems, HUDStyle, m_bottomBarItems);
    m_settingsMenu = new PluginObject("Settings", "Settings", nullptr, "icons/svg/gear-a.svg", "qrc:/qml/HUDSettingsPage/SettingsPage.qml");

    m_pluginList->addPlugin(m_settingsMenu);
    m_pluginList->addPlugin(m_themeSettings);

    qCDebug(THEMEMANAGER) << "Theme loaded : " << fileName;
}

void ThemeManager::onEvent(QString sender, QString event, QVariant eventData) {
    qDebug() << "ThemeManager :: Theme event : " << event << eventData;

    const QMetaObject *pluginMeta = m_themeObject->metaObject();

    AbstractPlugin* plugin = m_pluginList->getPlugin(sender);

    for(int i = pluginMeta->methodOffset(); i < pluginMeta->methodCount(); ++i){
        if(pluginMeta->method(i).methodSignature() == "onEvent(AbstractPlugin*,QString,QString,QVariant)"){
            QMetaMethod method = pluginMeta->method(i);
            bool ret = method.invoke(m_themeObject, Qt::DirectConnection, Q_ARG(AbstractPlugin*, plugin), Q_ARG(QString, sender), Q_ARG(QString, event), Q_ARG(QVariant, eventData));
            if(!ret){
                qDebug() << "Error invoking onEvent";
            }
        }
    }
}

void ThemeManager::processThemeSettings(QJsonObject json){
    if(!json.keys().contains("name") || !json.keys().contains("label") || !json.keys().contains("colors") || !json.keys().contains("sizes")){
        qCDebug(THEMEMANAGER) << "Error processing theme settings, missing required field(s)";
        return;
    }
    if(!json.value("colors").isArray() || !json.value("sizes").isArray()){
        qCDebug(THEMEMANAGER) << "Error processing theme settings, \"colors\" or \"sizes\" field is not an array";
        return;
    }

    HUDStyleSettings << loadSettingsMap("colors", "Colors", "color", json.value("colors").toArray().toVariantList(), &m_colorsMap);

    HUDStyleSettings << loadSettingsMap("sizes", "Sizes", "tumbler", json.value("sizes").toArray().toVariantList(), &m_sizesMap);

    HUDStyle.insert("colors", QVariant::fromValue<QQmlPropertyMap *>(&m_colorsMap));
    HUDStyle.insert("sizes", QVariant::fromValue<QQmlPropertyMap *>(&m_sizesMap));


    QJsonArray settingsJson = json.value("settings").toArray();

    for(const QJsonValue &item : qAsConst(settingsJson)){
        if(!item.isObject()){
            qDebug() << "Item is not object";
            continue;
        }

        QJsonObject jsonObject = item.toObject();

        QQmlPropertyMap *map = new QQmlPropertyMap();
        m_settings << new SettingsLoader(jsonObject, "", map);

        HUDStyle.insert(jsonObject.value("name").toString(), QVariant::fromValue<QQmlPropertyMap *>(map));
        HUDStyleSettings.append(jsonObject.toVariantMap());
    }

    HUDStyle.insert("settings", HUDStyleSettings);
}


QVariantMap ThemeManager::loadSettingsMap(QString name, QString label, QString type, QVariantList items, QQmlPropertyMap *settingsMap){
    QVariantMap settings({{"label",label}, {"type","items"}, {"name",name}});

    QVariantList itemsList;
    if(type == ""){
        itemsList = items;
    } else {
        itemsList = themeSettingsToSettingsItems(items, type);
    }
    settings.insert("items", itemsList);

    m_settings << new SettingsLoader(QJsonObject::fromVariantMap(settings), name, settingsMap);

    return settings;
}
QVariantList ThemeManager::themeSettingsToSettingsItems(QVariantList items, QString type){
    QVariantList settingsList;
    for(const QVariant &itemVariant : items){
        if(itemVariant.type() != static_cast<QVariant::Type>(QMetaType::QVariantMap)){
            qCDebug(THEMEMANAGER) << " : Invalid settings type, skipping";
            continue;
        }
        QVariantMap itemMap = itemVariant.toMap();
        if(!itemMap.keys().contains("name") || !itemMap.keys().contains("label") || !itemMap.keys().contains("defaultValue")){
            qCDebug(THEMEMANAGER) << "Error processing color, missing required field(s), skipping";
            continue;
        }
        QVariantMap item({
                             {"label", itemMap.value("label")},
                             {"name", itemMap.value("name")},
                             {"defaultValue", itemMap.value("defaultValue")},
                             {"type", type}
                         });
        settingsList << item;
    }
    return settingsList;
}

QVariantMap &ThemeManager::getStyle() {
    return HUDStyle;
}

QVariantList &ThemeManager::getBottomBarItems() {
    return m_bottomBarItems;
}

ThemeManager::~ThemeManager(){
    for(SettingsLoader * settings : qAsConst(m_settings)){
        delete(settings);
    }
    qDebug() << "Theme manager dead";
}
