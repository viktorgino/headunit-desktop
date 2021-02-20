#ifndef ODBCAR_H
#define ODBCAR_H

#include <QObject>
#include <QThreadPool>
#include "usbconnectionlistener.h"
#include <plugininterface.h>

class UsbConnectionListenerPlugin : public QObject, PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.viktorgino.headunit.usbconnectionlistener" FILE "config.json")
    Q_INTERFACES(PluginInterface)
public:
    explicit UsbConnectionListenerPlugin(QObject *parent = nullptr);
    QObject *getContextProperty() override;
    void init() override;

private slots:
    void usbDeviceAdded(QVariantMap deviceDetails);
    void usbDeviceRemoved(QVariantMap deviceDetails);
signals:
    void message(QString id, QString message);
private:
    UsbConnectionListener connectionListener;
};

#endif // ODBCAR_H
