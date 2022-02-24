#ifndef THEMEMANAGER_H
#define THEMEMANAGER_H

#include <QObject>
#include <QColor>
#include <QDebug>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQmlPropertyMap>
#include <QSettings>
#include <QDir>
#include <cstring>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QLoggingCategory>
#include <QSettings>
#include <QPluginLoader>
#include <QQmlExtensionPlugin>
#include <QCoreApplication>
#include <QThread>

#include "settingsloader.h"

class ThemeManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString themeSource READ getThemeSource NOTIFY themeSourceChanged)
public:
    explicit ThemeManager(QQmlApplicationEngine *engine, QString theme_name, bool initInThread, QObject *parent = nullptr);
    ~ThemeManager();
    void initTheme(QString themeName);
private:
    void loadJson(QString path);
    void processThemeSettings(QJsonObject json);
    QVariantMap loadSettingsMap(QString name, QString label, QString type, QVariantList items, QQmlPropertyMap * settingsMap);
    QVariantList themeSettingsToSettingsItems(QVariantList items, QString type);
    QQmlPropertyMap theme;
    QQmlApplicationEngine *m_engine;
    QVariantMap HUDStyle;
    QVariantList HUDStyleSettings;
    QList<SettingsLoader *>m_settings;
    QQmlExtensionPlugin * m_themePlugin;
    QString m_themeSource;
    QPluginLoader m_themeLoader;
signals:
    void themeEvent(QString sender, QString event, QVariant eventData);
    void themeSourceChanged();
public slots:
    void onEvent(QString sender, QString event, QVariant eventData);
private slots:
    void initFinished();
    QString getThemeSource() {
        return m_themeSource;
    }
};

class ThemeInitWorker : public QThread
{
    Q_OBJECT
public:
    ThemeInitWorker(QString themeName, ThemeManager *themeManager, QObject *parent = nullptr) :
          QThread(parent), m_themeName(themeName), m_themeManager(themeManager) {}
    void run() override;
private:
    QString m_themeName;
    ThemeManager *m_themeManager;
};

#endif // THEMEMANAGER_H
