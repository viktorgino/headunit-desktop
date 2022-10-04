#ifndef SAMPLEPLUGIN_H
#define SAMPLEPLUGIN_H

#include <QDebug>
#include <plugininterface.h>

class ReversingCamera : public QObject, PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.viktorgino.headunit.reversing-camera" FILE "config.json")
    Q_INTERFACES(PluginInterface)
public:
    explicit ReversingCamera(QObject *parent = nullptr);

    void init() override;

signals:
    void message(QString id, QVariant message);
};

#endif // SAMPLEPLUGIN_H
