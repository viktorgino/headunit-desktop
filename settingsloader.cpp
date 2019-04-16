#include "settingsloader.h"

Q_LOGGING_CATEGORY(SETTINGSLOADER, "Settings Loader")

SettingsLoader::SettingsLoader(QJsonObject obj, QQmlPropertyMap* settingsMap, QObject *parent) : QObject(parent), m_settings(settingsMap)
{
    connect(settingsMap, &QQmlPropertyMap::valueChanged, this, &SettingsLoader::settingsChanged);
    m_name = obj.value("name").toString();
    loadJson(obj);
}
SettingsLoader::~SettingsLoader(){
    qCDebug(SETTINGSLOADER) << "Settings loader died" << m_name;
}

void SettingsLoader::settingsChanged(QString key, QVariant value){
    saveSettings();
}

QQmlPropertyMap *SettingsLoader::createPropertyMap(QString group, QVariantMap map, QQmlPropertyMap *propertyMap) {
    settings.beginGroup(group);
    for(QString item : map.keys()){
        if(QString(map.value(item).typeName()) == "QVariantMap"){
            QQmlPropertyMap * propMapTemp = new QQmlPropertyMap();
            connect(propMapTemp, &QQmlPropertyMap::valueChanged, this, &SettingsLoader::settingsChanged);

            createPropertyMap(item, map.value(item).toMap(), propMapTemp);

            propertyMap->insert(item, QVariant::fromValue(propMapTemp));
        } else {
            if(settings.contains(item)){
                QVariant value = settings.value(item);

                QMap<QJsonValue::Type, QVariant::Type> typeMapping = {
                    {QJsonValue::Bool, QVariant::Bool},
                    {QJsonValue::Double, QVariant::Double},
                    {QJsonValue::String, QVariant::String},
                    {QJsonValue::Array, QVariant::List},
                    {QJsonValue::Object, QVariant::Map},
                };

                QJsonValue::Type jsonType = map.value(item).value<QJsonValue>().type();
                if(typeMapping.keys().contains(jsonType)){
                    value.convert(static_cast<int>(typeMapping.value(jsonType)));
                }

                QJsonValue jsonValue = QJsonValue::fromVariant(value);
                propertyMap->insert(item, jsonValue);
            } else {
                propertyMap->insert(item, map.value(item));
            }
        }
    }
    settings.endGroup();
    return propertyMap;
}

QQmlPropertyMap *SettingsLoader::getSettingsMap(){
    return m_settings;
}

void SettingsLoader::saveSettings(){
    saveSettings(m_name, m_settings);
}
void SettingsLoader::saveSettings(QString group, QQmlPropertyMap *settingsMap){
    if(!settingsMap){
        return;
    }

    settings.beginGroup(group);
    for(QString item : settingsMap->keys()){
        if(settingsMap->value(item).canConvert<QQmlPropertyMap *>()){
            QQmlPropertyMap * propMap = settingsMap->value(item).value<QQmlPropertyMap*>();

            if(!propMap){
                qCDebug(SETTINGSLOADER) << "Invalid property map";
                continue;
            }
            saveSettings(item, propMap);
        } else {
            if (settingsMap->value(item).type() == QMetaType::QObjectStar ){
                continue;
            }
            if(settings.value(item) != settingsMap->value(item)){
                QVariant value = settingsMap->value(item);
                if(value.type() == QMetaType::QJsonValue){
                    settings.setValue(item, value.value<QJsonValue>().toVariant());
                } else {
                    settings.setValue(item, value);
                }
            }
        }
    }

    settings.endGroup();
}

void SettingsLoader::loadJson(QJsonObject json){
    if(json.value("type").toString() != "items"){
        qCDebug(SETTINGSLOADER)  << settings.group() << " : Error loading settings JSON, root type should be of \"items\" type";
        return;
    }

    settings.beginGroup(m_name);
    QVariant root = processItem(json);
    settings.endGroup();

    if(root.type() != static_cast<QVariant::Type>(QMetaType::QVariantMap)){
        qCDebug(SETTINGSLOADER) << "Invalid root object";
        return;
    }

    QVariantMap rootMap = root.toMap();
    createPropertyMap(m_name, rootMap, m_settings);

    saveSettings();
}

void SettingsLoader::loadJson(QString path){
    QFile file(path);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qCDebug(SETTINGSLOADER) << "Error opening settings theme file " << path;
        return;
    }

    QString jsonString = file.readAll();
    QJsonParseError error;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonString.toUtf8(), &error);

    if(jsonDoc.isNull()){
        qCDebug(SETTINGSLOADER) << "Error parsing json file " << path << " " << error.errorString();
        return;
    }
    loadJson(jsonDoc.object());
}

QVariant SettingsLoader::processItem(QJsonObject json){
    if(json.value("label") == "" || json.value("name") == "" || json.value("type") == "" ){
        qCDebug(SETTINGSLOADER) << settings.group() << " : Missing required property(s). All item should include \"label\", \"name\" and \"type\" properties";
        return QVariant();
    }

    if(json.value("type").toString() == "items"){
        if(!json.value("items").isArray()){
            qCDebug(SETTINGSLOADER) << settings.group() << " : Missing \"items\" array for \"items\" type";
            return QVariant();
        }

        QVariantMap itemsMap;

        if(json.value("name").toString() != m_name){
            settings.beginGroup(json.value("name").toString());
        }

        QVariantList items = json.value("items").toArray().toVariantList();
        for(QVariant item : items){
            if(item.type() != static_cast<QVariant::Type>(QMetaType::QVariantMap)){
                qCDebug(SETTINGSLOADER) << settings.group() << " : Invalid items type, skipping.";
                continue;
            }
            QJsonObject obj = QJsonObject::fromVariantMap(item.toMap());
            if(itemsMap.keys().contains(obj.value("name").toString())){
                qCDebug(SETTINGSLOADER) << "Duplicate name ( " << settings.group() << obj.value("name").toString() << " ) skipping.";
                continue;
            }
            QString tempGroup;

            QVariant itemValue = processItem(obj);
            itemsMap.insert(obj.value("name").toString(), itemValue);
        }

        if(json.value("name").toString() != m_name){
            settings.endGroup();
        }
        return itemsMap;
    } else if (types.keys().contains(json.value("type").toString())) {
        QVariant value = settings.value(json.value("name").toString());

        if(value.isNull()){
            if(json.value("defaultValue").isUndefined()){
                QMetaType::Type type = types.value(json.value("type").toString());
                switch (type) {
                case QMetaType::Double:
                    value = 0.0;
                    break;
                case QMetaType::Bool:
                    value = false;
                    break;
                case QMetaType::QVariant:
                    value =  QString("");
                    break;
                case QMetaType::Int:
                    value =  0;
                    break;
                case QMetaType::QColor:
                    value =  QColor("#ffffff");
                    break;
                case QMetaType::QString:
                    value =  QString("");
                    break;
                default:
                    value =  QVariant();
                }
            } else {
                value = json.value("defaultValue");
            }
        }
        return value;
    }
    return QVariant();
}
