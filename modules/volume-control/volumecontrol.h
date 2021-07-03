#ifndef VOLUMECONTROL_H
#define VOLUMECONTROL_H

#include <QObject>
#include <QQmlEngine>
#include <plugininterface.h>
#include "pulseaudio-qt/src/models.h"

class VolumeControl : public QObject, PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.viktorgino.headunit.volumecontrol" FILE "config.json")
    Q_INTERFACES(PluginInterface)
public:
    explicit VolumeControl(QObject *parent = nullptr);
    void init() override;
    QObject *getContextProperty() override;
};

#endif // VOLUMECONTROL_H
