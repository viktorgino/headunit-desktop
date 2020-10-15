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
    QQuickImageProvider *getImageProvider() override;
    QStringList eventListeners() override;
    QStringList events() override;
    QStringList actions() override;
private slots:
    void usbDeviceAdded(QVariantMap deviceDetails);
    void usbDeviceRemoved(QVariantMap deviceDetails);
signals:
    void onMessage(QString id, QString message);
public slots:
    void eventMessage(QString id, QString message) override;
    void actionMessage(QString id, QString message) override;
private:
    UsbConnectionListener connectionListener;
};

#endif // ODBCAR_H
