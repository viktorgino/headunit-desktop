#ifndef SAMPLEPLUGIN_H
#define SAMPLEPLUGIN_H

#include <QDebug>
#include <plugininterface.h>

class SamplePlugin : public QObject, PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.viktorgino.headunit.sample" FILE "config.json")
    Q_INTERFACES(PluginInterface)
public:
    explicit SamplePlugin(QObject *parent = nullptr);

    void init() override;

    Q_INVOKABLE void testNotification();
    Q_INVOKABLE void onSettingsPageDestroyed();
signals:
    void message(QString id, QVariant message);
};

#endif // SAMPLEPLUGIN_H
