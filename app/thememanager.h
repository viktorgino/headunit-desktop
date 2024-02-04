#ifndef THEMEMANAGER_H
#define THEMEMANAGER_H

#include <QColor>
#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLoggingCategory>
#include <QObject>
#include <QPluginLoader>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQmlExtensionPlugin>
#include <QQmlPropertyMap>
#include <QSettings>
#include <QThread>
#include <cstring>

#include "settingsloader.h"
#include "pluginlist.h"

class ThemeManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString themeSource READ getThemeSource NOTIFY themeSourceChanged)
public:
    explicit ThemeManager(QQmlApplicationEngine *engine, PluginList *pluginList, QObject *parent = nullptr);
    ~ThemeManager();
    void initTheme(QString themeName);
    QVariantMap &getStyle();
    QVariantList &getBottomBarItems();

private:
    void processThemeSettings(QJsonObject json);
    QVariantMap loadSettingsMap(QString name, QString label, QString type, QVariantList items, QQmlPropertyMap *settingsMap);
    QVariantList themeSettingsToSettingsItems(QVariantList items, QString type);
    QQmlPropertyMap theme;
    QQmlApplicationEngine *m_engine;
    QVariantMap HUDStyle;
    QVariantList HUDStyleSettings;
    QList<SettingsLoader *> m_settings;
    QQmlExtensionPlugin *m_themePlugin;
    QObject *m_themeObject;
    QString m_themeSource;
    QPluginLoader m_themeLoader;
    QVariantList m_bottomBarItems;
    PluginList * m_pluginList;

    PluginObject *m_themeSettings;

    QQmlPropertyMap m_colorsMap;
    QQmlPropertyMap m_sizesMap;
signals:
    void themeSourceChanged();
    void themeLoaded();
public slots:
    void onEvent(QString sender, QString event, QVariant eventData);
    void initFinished();
    QString getThemeSource()
    {
        return m_themeSource;
    }
};

#endif // THEMEMANAGER_H
