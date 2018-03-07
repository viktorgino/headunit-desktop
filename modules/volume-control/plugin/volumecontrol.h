#ifndef VOLUMECONTROL_H
#define VOLUMECONTROL_H

#include <QObject>
#include <plugininterface.h>

class VolumeControl : public QObject, PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.viktorgino.headunit.volumecontrol" FILE "config.json")
    Q_INTERFACES(PluginInterface)
public:
    explicit VolumeControl(QObject *parent = nullptr);
    QObject *getContextProperty() override;

signals:

public slots:
};

#endif // VOLUMECONTROL_H
