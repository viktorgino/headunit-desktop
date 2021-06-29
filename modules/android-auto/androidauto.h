#ifndef ANDROIDAUTO_H
#define ANDROIDAUTO_H

#include <QObject>
#include <QThreadPool>
#include <QDebug>
#include <plugininterface.h>
#include <mediainterface.h>
#include "headunit.h"

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
    Headunit *headunit = nullptr;
};

#endif // ANDROIDAUTO_H
