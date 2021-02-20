#ifndef PLUGININTERFACE_H
#define PLUGININTERFACE_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QQuickImageProvider>
#include <QQmlPropertyMap>

typedef struct PluginSettings {
    QStringList eventListeners;
    QStringList events;
    QStringList actions;
} PluginSettings;

class PluginInterface
{
    Q_GADGET
    Q_PROPERTY(QQmlPropertyMap * Settings READ getSettings CONSTANT)
public:
    explicit PluginInterface() {}
    virtual QObject *getContextProperty() { return nullptr;}
    virtual QQuickImageProvider *getImageProvider() { return nullptr;}

    virtual void init() = 0;
    virtual ~PluginInterface() {}
    virtual void onLoad() {}

    QQmlPropertyMap *getSettings() {
        return &m_settings;
    }
    PluginSettings *getPluginSettings() {
        return &m_pluginSettings;
    }
public slots:
    virtual void eventMessage(QString id, QVariant message) {};
    virtual void actionMessage(QString id, QVariant message) {};
protected:
    PluginSettings m_pluginSettings;
    QQmlPropertyMap m_settings;
};

#define PluginInterface_iid "org.viktorgino.Headunit.PluginInterface"

Q_DECLARE_INTERFACE(PluginInterface, PluginInterface_iid)

#endif // PLUGININTERFACE_H
