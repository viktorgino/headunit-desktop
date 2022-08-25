#ifndef SAMPLEPLUGIN_H
#define SAMPLEPLUGIN_H

#include <QDebug>
#include <plugininterface.h>

class AudioProcessorPlugin : public QObject, PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.viktorgino.headunit.audio-processor" FILE "config.json")
    Q_INTERFACES(PluginInterface)
public:
    explicit AudioProcessorPlugin(QObject *parent = nullptr);

    void init() override;

    Q_INVOKABLE void testNotification();
    Q_INVOKABLE void setAudioParameter(QString parameter, int value);
    Q_INVOKABLE void onSettingsPageDestroyed();
    Q_INVOKABLE void closeOverlay();
    QObject *getContextProperty() override;
signals:
    void message(QString id, QVariant message);
    void action(QString id, QVariant message);
};

#endif // SAMPLEPLUGIN_H
