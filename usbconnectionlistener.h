#ifndef USBCONNECTIONLISTENER_H
#define USBCONNECTIONLISTENER_H

#include <QObject>
#include <QString>
#include <QRunnable>
#include <libusb.h>
#include <libudev.h>

#define ACC_REQ_GET_PROTOCOL        51
#define USB_DIR_IN              0x80
#define USB_DIR_OUT             0x00
#define USB_TYPE_VENDOR         0x40

#define VEN_ID_GOOGLE           0x18D1
#define DEV_ID_OAP              0x2D00
#define DEV_ID_OAP_WITH_ADB     0x2D01

enum USB_EVENT_TYPE
{
    USB_CONNECTED = 0,
    USB_REMOVED = 1,
    MASS_STORAGE_CONNECTED = 2,
    ANDROID_DEVICE_CONNECTED  = 3,
};
/*
 * First run echo 'SUBSYSTEM=="usb", ENV{DEVTYPE}=="usb_device", MODE="0666"' | sudo tee /lib/udev/rules.d/99-allow-all-usb.rules && sudo udevadm control --reload-rules
 */
class UsbConnectionListener : public QObject,public QRunnable
{
Q_OBJECT
public:
    UsbConnectionListener();
    virtual ~UsbConnectionListener();
    virtual void run() override;
    void stop();
    void emitSignal(USB_EVENT_TYPE event_type, QString device_descriptor);
    void emitSignal(USB_EVENT_TYPE event_type, QString device_descriptor, QList<QString> mountList);
    static bool isAndroidDevice(libusb_device_handle *device);
    static bool isOapDevice(libusb_device_descriptor *desc);
    static QList<QString> getMountPoint(QString serialNumber);
    static bool hasMassStorageInterface(libusb_device *dev);
private:
    bool done = false;
    static int LIBUSB_CALL hotplugCallback(libusb_context *ctx, libusb_device *dev, libusb_hotplug_event event, void *user_data);
    libusb_context *hotplug_context = NULL;
signals:
    void usbDriveConnected(QString device_descriptor);
    void androidDeviceConnected(QString device_descriptor);
    void deviceRemoved(QString device_descriptor);
};

#endif // USBCONNECTIONLISTENER_H
