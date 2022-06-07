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
#include <sys/time.h>


class UsbConnectionListener : public QThread
{
    Q_OBJECT
public:
    UsbConnectionListener(QObject *parent = nullptr);
    ~UsbConnectionListener() override;
    void run() override;
    void stop();
    void usbDeviceAdd(QString device);
    void usbDeviceRemove(QString device);
private:
    bool done = false;
    static int LIBUSB_CALL hotplugCallback(libusb_context *ctx, libusb_device *dev, libusb_hotplug_event event, void *user_data);
    QStringList newDevices;
signals:
    void usbDeviceAdded(QVariantMap deviceDetails);
    void usbDeviceRemoved(QVariantMap deviceDetails);
};

#endif // USBCONNECTIONLISTENER_H
