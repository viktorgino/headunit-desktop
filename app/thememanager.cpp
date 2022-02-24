#include "thememanager.h"

Q_LOGGING_CATEGORY(THEMEMANAGER, "Theme Manager")


void ThemeInitWorker::run() {
    m_themeManager->initTheme(m_themeName);
}

ThemeManager::ThemeManager(QQmlApplicationEngine *engine, QString theme_name, bool initInThread, QObject *parent) : QObject(parent),
    m_engine(engine), m_themeLoader(this)
{
    m_engine->rootContext()->setContextProperty("ThemeManager", this);
    ThemeInitWorker *workerThread = new ThemeInitWorker(theme_name, this, this);
    if(initInThread){
        connect(workerThread, &ThemeInitWorker::finished, this, &ThemeManager::initFinished);
        workerThread->start();
    } else {
        initTheme(theme_name);
        initFinished();
    }
}

void ThemeManager::initFinished() {
    QJsonObject themeSettings = m_themeLoader.metaData().value("MetaData").toObject();

    processThemeSettings(themeSettings);

    m_themePlugin->registerTypes("");
    m_themePlugin->initializeEngine(m_engine, "");

    if(themeSettings.contains("source")) {
        m_themeSource = themeSettings.value("source").toString();
        emit themeSourceChanged();
    }
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

    QQmlExtensionPlugin * theme = static_cast<QQmlExtensionPlugin *>(m_themeLoader.instance());

    if (!theme) {
        qCDebug(THEMEMANAGER) << "Error loading plugin : " << fileName << m_themeLoader.errorString();
        return;
    }

    m_themePlugin = static_cast<QQmlExtensionPlugin *>(m_themeLoader.instance());

    if(!m_themePlugin){
        qCDebug(THEMEMANAGER) << "Error loading theme : " << m_themeLoader.metaData().value("name") << ", root component is not a valid instance of QQmlExtensionPlugin";
        return;
    }

    const QMetaObject *pluginMeta = m_themePlugin->metaObject();

    QStringList methods;
    for(int i = pluginMeta->methodOffset(); i < pluginMeta->methodCount(); ++i){
        if(pluginMeta->method(i).methodSignature() == "onEvent(QString,QString,QVariant)"){
            connect(this, SIGNAL(themeEvent(QString, QString, QVariant)), m_themePlugin, SLOT(onEvent(QString, QString, QVariant)));
        }
    }
    m_engine->addImportPath(themeDir.absolutePath());

    qCDebug(THEMEMANAGER) << "Theme loaded : " << fileName;


}
void ThemeManager::onEvent(QString sender, QString event, QVariant eventData) {
    emit themeEvent(sender, event, eventData);
}

void ThemeManager::loadJson(QString path){
    QFile file(path);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qCDebug(THEMEMANAGER) << "Error opening settings theme file " << path;
        return;
    }

    QString jsonString = file.readAll();
    QJsonParseError error;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonString.toUtf8(), &error);

    if(jsonDoc.isNull()){
        qCDebug(THEMEMANAGER) << "Error parsing json file " << path << " " << error.errorString();
        return;
    }
    processThemeSettings(jsonDoc.object());
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

    QQmlPropertyMap *colorsMap = new QQmlPropertyMap(this);
    HUDStyleSettings << loadSettingsMap("Colors", "Colors", "color", json.value("colors").toArray().toVariantList(), colorsMap);

    QQmlPropertyMap *sizesMap = new QQmlPropertyMap(this);
    HUDStyleSettings << loadSettingsMap("Sizes", "Sizes", "tumbler", json.value("sizes").toArray().toVariantList(), sizesMap);

    HUDStyle.insert("Colors", QVariant::fromValue<QQmlPropertyMap *>(colorsMap));
    HUDStyle.insert("Sizes", QVariant::fromValue<QQmlPropertyMap *>(sizesMap));


    QJsonArray settingsJson = json.value("settings").toArray();

    for(QJsonValueRef item : settingsJson){
        if(!item.isObject()){
            qDebug() << "Item is not object";
            continue;
        }

        QJsonObject jsonObject = item.toObject();

        QQmlPropertyMap *map = new QQmlPropertyMap(this);
        m_settings << new SettingsLoader(jsonObject, map, this);

        HUDStyle.insert(jsonObject.value("name").toString(), QVariant::fromValue<QQmlPropertyMap *>(map));
        HUDStyleSettings.append(jsonObject.toVariantMap());
    }

    m_engine->rootContext()->setContextProperty("HUDStyle", HUDStyle);
    m_engine->rootContext()->setContextProperty("HUDStyleSettings", HUDStyleSettings);
}


QVariantMap ThemeManager::loadSettingsMap(QString name, QString label, QString type, QVariantList items, QQmlPropertyMap *settingsMap){
    QVariantMap settings({{"label",label}, {"name",name}, {"type","items"}});

    QVariantList itemsList;
    if(type == ""){
        itemsList = items;
    } else {
        itemsList = themeSettingsToSettingsItems(items, type);
    }
    settings.insert("items", itemsList);

    m_settings << new SettingsLoader(QJsonObject::fromVariantMap(settings), settingsMap, this);

    return settings;
}
QVariantList ThemeManager::themeSettingsToSettingsItems(QVariantList items, QString type){
    QVariantList settingsList;
    for(QVariant itemVariant : items){
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

ThemeManager::~ThemeManager(){
    for(SettingsLoader * settings : m_settings){
        delete(settings);
    }
    qDebug() << "Theme manager dead";
}
