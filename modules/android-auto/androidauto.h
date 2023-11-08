#ifndef ANDROIDAUTO_H
#define ANDROIDAUTO_H

#include "headunitbluetoothserver.h"
#include "headunitbluetoothservice.h"
#include "headunitvideosource.h"
#include <QDebug>
#include <QObject>
#include <QThreadPool>
#include <mediainterface.h>
#include <plugininterface.h>

class AndroidAutoPlugin : public QObject, PluginInterface, public MediaInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.viktorgino.headunit.androidauto" FILE "config.json")
    Q_INTERFACES(PluginInterface)
public:
    explicit AndroidAutoPlugin(QObject *parent = nullptr);
    QObject *getContextProperty() override;
    void init() override;

    void start() override;
    void stop() override;
    void prevTrack() override;
    void nextTrack() override;
    void setMediaVolume(uint8_t volume) override;
    void setVoiceVolume(uint8_t volume) override;
signals:
    void playbackStarted() override;
    void message(QString id, QVariant message);

public slots:
    void eventMessage(QString id, QVariant message) override;
private slots:
    void huStatusChanged();

private:
    HeadunitVideoSource m_headunit = nullptr;
    HeadunitBluetoothServer m_bluetoothServer;
    HeadunitBluetoothService m_bluetoothService;
};

#endif // ANDROIDAUTO_H
