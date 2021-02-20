#ifndef FMRADIOPLUGIN_H
#define FMRADIOPLUGIN_H

#include <plugininterface.h>

class FMRadioPlugin : public QObject, PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.viktorgino.headunit.sample" FILE "config.json")
    Q_INTERFACES(PluginInterface)
public:
    explicit FMRadioPlugin(QObject *parent = nullptr);
    void init() override;
};

#endif // SAMPLEPLUGIN_H
