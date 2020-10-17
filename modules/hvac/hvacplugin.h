#ifndef SAMPLEPLUGIN_H
#define SAMPLEPLUGIN_H

#include <plugininterface.h>
#include "hvaccontroller.h"

class HVACPlugin : public QObject, PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.viktorgino.headunit.hvac" FILE "config.json")
    Q_INTERFACES(PluginInterface)
public:
    explicit HVACPlugin(QObject *parent = nullptr);
    ~HVACPlugin() override;
    QObject *getContextProperty() override;
    QQuickImageProvider *getImageProvider() override;
    QStringList eventListeners() override;
    QStringList events() override;
    QStringList actions() override;
private slots:
    void usbDeviceAdded(QString deviceDetails);
    void usbDeviceRemoved(QString deviceDetails);
signals:
    void onMessage(QString id, QString message);
public slots:
    void eventMessage(QString id, QString message) override;
    void actionMessage(QString id, QString message) override;
private:
    QObject *m_carContextProperty;
    QSettings *carPlatformSettings;
    HVACController *m_hvac = nullptr;
    QQmlPropertyMap hvacProperties;
    void loadHVAC(QString settingsFile);
};

#endif // SAMPLEPLUGIN_H
