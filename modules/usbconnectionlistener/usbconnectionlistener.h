#ifndef USBCONNECTIONLISTENER_H
#define USBCONNECTIONLISTENER_H

#include <QObject>
#include <QString>
#include <QRunnable>
#include <QJsonDocument>
#include <QThread>
#include <QDirIterator>
#include <QDebug>
#include <libusb.h>

class UsbConnectionListener : public QObject, public QRunnable
{
    Q_OBJECT
public:
    UsbConnectionListener();
    ~UsbConnectionListener() override;
    virtual void run() override;
    void stop();
    void usbDeviceAdd(QString device);
    void usbDeviceRemove(QString device);
private:
    bool done = false;
    static int LIBUSB_CALL hotplugCallback(libusb_context *ctx, libusb_device *dev, libusb_hotplug_event event, void *user_data);
    libusb_context *hotplug_context = nullptr;
    QStringList newDevices;
signals:
    void usbDeviceAdded(QVariantMap deviceDetails);
    void usbDeviceRemoved(QVariantMap deviceDetails);
};

#endif // USBCONNECTIONLISTENER_H
