#ifndef PLUGINOBJECT_H
#define PLUGINOBJECT_H

#include <QObject>
#include <QJsonObject>
#include <QDebug>
#include <QLoggingCategory>

#include <QPluginLoader>

#include "../includes/plugininterface.h"
#include "settingsloader.h"
#include "../includes/mediainterface.h"

class PluginObject : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool loaded READ getLoaded NOTIFY loadedChanged)
    Q_PROPERTY(QVariant qmlSource READ getSource NOTIFY sourceChanged)
    Q_PROPERTY(QString name READ getName CONSTANT)
    Q_PROPERTY(QString label READ getLabel CONSTANT)
    Q_PROPERTY(QVariantMap menu READ getMenu CONSTANT)
    Q_PROPERTY(QQmlPropertyMap * settings READ getSettings CONSTANT)
public:
    explicit PluginObject(QString fileName, QObject *parent = nullptr);
    explicit PluginObject(QString name, QString label,
                          QObject *parent = nullptr, QString qmlSource = "",
                          QVariantMap menu = QVariantMap(),
                          QVariantMap settingsItems = QVariantMap(),
                          QQmlPropertyMap * settings = nullptr);

    void setSource(QString source);

    bool getLoaded();
    QString getSource();
    QString getName();
    QString getLabel();
    QVariantMap getMenu();

    QObject * getContextProperty();
    QQuickImageProvider *getImageProvider();

    QObject *getPlugin();
    QQmlPropertyMap *getSettings();
    QVariantMap getSettingsItems();
    MediaInterface *getMediaInterface();

    Q_INVOKABLE void callSlot(QString slot);

    void init();
public slots:
    void handleMessage(QString id, QVariant message);
    void callAction(QString id, QVariant message);

signals:
    void loadedChanged();
    void sourceChanged();
    void loaded();

    void message(QString sender, QString message, QVariant parameter);
    void action(QString sender, QString action, QVariant parameter);
private slots:
    void messageHandler(QString messageId, QVariant parameter);
    void actionHandler(QString actionId, QVariant parameter);
private:
    bool m_loaded;
    QVariant m_pluginFileName;

    QString m_source;
    QString m_name;
    QString m_label;
    QVariantMap m_menu;
    QVariant m_config;

    QPluginLoader m_pluginLoader;
    QObject *m_plugin;
    PluginInterface *m_pluginInterface;

    QQmlPropertyMap *m_settings;
    QVariantMap m_settingsItems;
    MediaInterface * m_mediaInterface;
};

#endif // PLUGINOBJECT_H
