#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QColor>
#include <QDebug>
#include <QQmlApplicationEngine>
#include <QQmlPropertyMap>
#include <QSettings>
#include <QDir>
#include <cstring>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QLoggingCategory>
#include <QSettings>

class SettingsLoader : public QObject
{
    Q_OBJECT
public:
    explicit SettingsLoader(QJsonObject obj, QString name, QQmlPropertyMap* settingsMap, QObject *parent = nullptr);

    ~SettingsLoader();
    QQmlPropertyMap  *getSettingsMap();
signals:
    void propertyChanged();
public slots:
    void settingsChanged(QString key, QVariant value);
private:
    void loadJson(QString path);
    void loadJson(QJsonObject json);
    QVariant processItem(QJsonObject json);
    QVariantMap processSettingsItems(QJsonObject obj, QString parentKey = "");
    QVariantMap processColorItem(QJsonObject obj, QString parentKey = "");
    QQmlPropertyMap *createPropertyMap(QString group, QVariantMap map, QQmlPropertyMap *propertyMap);

    QQmlPropertyMap *m_settings;
    QString m_name;
    QSettings settings;
    void saveSettings();
    void saveSettings(QString group, QQmlPropertyMap *settingsMap);
    QMap<QString, QMetaType::Type> types =
    {
        {"items",QMetaType::QVariantMap},
        {"slider",QMetaType::Double},
        {"switch",QMetaType::Bool},
        {"checkbox",QMetaType::Bool},
        {"textfield",QMetaType::QVariant},
        {"combobox",QMetaType::QString},
        {"color",QMetaType::QColor},
        {"file",QMetaType::QString},
        {"folder",QMetaType::QString},
        {"tumbler",QMetaType::Double},
            {"string",QMetaType::QString},
            {"int",QMetaType::Int},
            {"uint",QMetaType::UInt},
            {"double",QMetaType::Double},
            {"long",QMetaType::Long},
            {"bool",QMetaType::Bool},
    };
};

#endif // SETTINGS_H
