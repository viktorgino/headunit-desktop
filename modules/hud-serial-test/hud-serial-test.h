#ifndef SAMPLEPLUGIN_H
#define SAMPLEPLUGIN_H

#include <QDebug>
#include <plugininterface.h>
#include <hvac-common.h>

class HUDSerialTest : public QObject, PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.viktorgino.headunit.sample" FILE "config.json")
    Q_INTERFACES(PluginInterface)
public:
    explicit HUDSerialTest(QObject *parent = nullptr);

    void init() override;
    QObject *getContextProperty() override;
public slots:
    void buttonClicked(QString key);
    void setZoneParameter(QString zone, QString parameter, QVariant value);
    void setParameter(QString parameter, bool value);
    void setAudioParameter(QString parameter, int value);
signals:
    void message(QString id, QVariant message);
    void action(QString id, QVariant message);
private:
    ClimateControlCommandFrame m_ccCommandFrame;
};

#endif // SAMPLEPLUGIN_H
