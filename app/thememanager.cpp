#include "thememanager.h"

Q_LOGGING_CATEGORY(THEMEMANAGER, "Theme Manager")

ThemeManager::ThemeManager(QQmlApplicationEngine *engine, QObject *parent) : QObject(parent),
    m_engine(engine)
{
    loadJson("theme.json");
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
    }
    if(!json.value("colors").isArray() || !json.value("sizes").isArray()){
        qCDebug(THEMEMANAGER) << "Error processing theme settings, \"colors\" or \"sizes\" field is not an array";
    }

    QQmlPropertyMap *colorsMap = new QQmlPropertyMap();
    HUDStyleSettings << loadSettingsMap("Colors", "Colors", "color", json.value("colors").toArray().toVariantList(), colorsMap);

    QQmlPropertyMap *sizesMap = new QQmlPropertyMap();
    HUDStyleSettings << loadSettingsMap("Sizes", "Sizes", "slider", json.value("sizes").toArray().toVariantList(), sizesMap);

    HUDStyle.insert("Colors", QVariant::fromValue<QQmlPropertyMap *>(colorsMap));
    HUDStyle.insert("Sizes", QVariant::fromValue<QQmlPropertyMap *>(sizesMap));


    QJsonArray settingsJson = json.value("settings").toArray();

    for(QJsonValueRef item : settingsJson){
        if(!item.isObject()){
            qDebug() << "Item is not object";
            continue;
        }

        QJsonObject jsonObject = item.toObject();

        QQmlPropertyMap *map = new QQmlPropertyMap();
        m_settings << new SettingsLoader(jsonObject, map);

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

    m_settings << new SettingsLoader(QJsonObject::fromVariantMap(settings), settingsMap);

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
