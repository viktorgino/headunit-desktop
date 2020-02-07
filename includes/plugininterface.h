#ifndef PLUGININTERFACE_H
#define PLUGININTERFACE_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QQuickImageProvider>
#include <QQmlPropertyMap>

class PluginInterface
{
    Q_PROPERTY(QQmlPropertyMap *Settings MEMBER settings CONSTANT)
public:
    virtual QObject *getContextProperty() = 0;
    virtual QQuickImageProvider *getImageProvider() = 0;
    virtual QStringList eventListeners() = 0;
    virtual QStringList events() = 0;
    virtual QStringList actions() = 0;
    virtual ~PluginInterface() {}
    virtual void onLoad() {}
    QQmlPropertyMap settings;
public slots:
    virtual void eventMessage(QString id, QString message) = 0;
    virtual void actionMessage(QString id, QString message) = 0;
};

#define PluginInterface_iid "org.viktorgino.Headunit.PluginInterface"

Q_DECLARE_INTERFACE(PluginInterface, PluginInterface_iid)

#endif // PLUGININTERFACE_H
