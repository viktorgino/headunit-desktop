#ifndef ABSTRACTPLUGIN_H
#define ABSTRACTPLUGIN_H

#include <QObject>
#include <QJsonObject>
#include <QDebug>
#include <QLoggingCategory>

#include <QPluginLoader>

#include <plugininterface.h>
#include <mediainterface.h>
#include <hud-common.h>

class AbstractPlugin
{
public:
    // virtual void setSource(QString source) = 0;

    virtual bool getLoaded() = 0;
    virtual QString getSource() = 0;
    virtual QString getName() = 0;
    virtual QString getLabel() = 0;
    virtual QString getIcon() = 0;

    virtual QObject * getContextProperty() = 0;
    virtual QQuickImageProvider *getImageProvider() = 0;

    virtual QObject *getPlugin() = 0;
    virtual QVariant getSettings() = 0;
    virtual QVariantMap getSettingsItems() = 0;
    virtual QList<PanelItem> getBottomBarItems() = 0;
    virtual MediaInterface *getMediaInterface() = 0;
    virtual void connectToPropertySignal(QString propertyName, QString slotName) = 0;
    virtual QVariant getPropertyValue(QString propertyName) = 0;

    virtual Q_INVOKABLE void callSlot(QString slot) = 0;

    virtual void init() = 0;
};
Q_DECLARE_METATYPE(AbstractPlugin*);
#endif // ABSTRACTPLUGIN_H
