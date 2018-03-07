#ifndef PLUGININTERFACE_H
#define PLUGININTERFACE_H

#include <QObject>
#include <QString>

class PluginInterface
{
public:
    virtual ~PluginInterface() {}
    //virtual void load(const QString &message) = 0;
    virtual QObject *getContextProperty() = 0;
};

#define PluginInterface_iid "org.viktorgino.Headunit.PluginInterface"

Q_DECLARE_INTERFACE(PluginInterface, PluginInterface_iid)

#endif // PLUGININTERFACE_H
