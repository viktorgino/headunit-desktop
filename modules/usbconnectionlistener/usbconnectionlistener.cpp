#include "usbconnectionlistener.h"

UsbConnectionListener::UsbConnectionListener(QObject *parent) : QThread(parent){

}

UsbConnectionListener::~UsbConnectionListener(){
    stop();
}

int LIBUSB_CALL UsbConnectionListener::hotplugCallback(libusb_context */* unused */, libusb_device *dev, libusb_hotplug_event event, void *user_data) {
    struct libusb_device_descriptor desc;
    int rc;

    UsbConnectionListener *parent = static_cast<UsbConnectionListener *>(user_data);

    rc = libusb_get_device_descriptor(dev, &desc);
    if (LIBUSB_SUCCESS != rc) {
        qWarning("Error getting device descriptor");
        return -1;
    }

    if(event == LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED){
        QString device = QString("%1:%2").arg(desc.idVendor).arg(desc.idProduct);
        parent->usbDeviceAdd(device);
    } else if(event == LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT){
        QString device = QString("%1:%2").arg(desc.idVendor).arg(desc.idProduct);
        parent->usbDeviceRemove(device);

        QVariantMap deviceDetails;
        deviceDetails.insert("vid", QString("%1").arg(desc.idVendor));
        deviceDetails.insert("pid", QString("%1").arg(desc.idProduct));

        emit parent->usbDeviceRemoved(deviceDetails);
    }
    return 0;
}

void UsbConnectionListener::stop(){
    done = true;
    libusb_interrupt_event_handler(nullptr);
    wait(QDeadlineTimer(10));
}

void UsbConnectionListener::usbDeviceAdd(QString device){
    if(!newDevices.contains(device)){
        newDevices.append(device);
    }
}

void UsbConnectionListener::usbDeviceRemove(QString device){
    newDevices.removeAll(device);
}

void UsbConnectionListener::run() {
    int rc;
    libusb_context *hotplug_context = nullptr;
    libusb_hotplug_callback_handle callback_handle = 0;
    struct timeval timeout;
    timeout.tv_usec = 1;

    if (libusb_init(&hotplug_context) < 0)
    {
        qDebug ("Error libusb_init usb_err failed");
        return;
    }
    libusb_set_option(hotplug_context, LIBUSB_OPTION_LOG_LEVEL, LIBUSB_LOG_LEVEL_INFO);

    rc = libusb_hotplug_register_callback(hotplug_context, static_cast<libusb_hotplug_event>(LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED |
                                                                                             LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT)
                                          , LIBUSB_HOTPLUG_NO_FLAGS , LIBUSB_HOTPLUG_MATCH_ANY,
                                          LIBUSB_HOTPLUG_MATCH_ANY, LIBUSB_HOTPLUG_MATCH_ANY, hotplugCallback, this, NULL);
    if (rc != LIBUSB_SUCCESS) {
        qDebug("libusb_hotplug_register_callback() failed: Error registering callback 0");
        return;
    }

    while(!done) {
        newDevices.clear();
        rc = libusb_handle_events_completed(hotplug_context, nullptr);
        if (rc < 0) {
            qDebug("libusb_handle_events() failed: %s", libusb_error_name(rc));
            break;
        }

        if(newDevices.length() > 0){
            libusb_device** devices = nullptr;
            ssize_t dev_count = libusb_get_device_list(hotplug_context, &devices);
            if (dev_count > 0){
                for (ssize_t i = 0; i < dev_count; i++)
                {
                    libusb_device_descriptor desc;
                    int usb_err = libusb_get_device_descriptor(devices[i], &desc);
                    if (usb_err < 0)
                    {
                        continue;
                    }

                    QString deviceString = QString ("%1:%2").arg(desc.idVendor).arg(desc.idProduct);

                    if(!newDevices.contains(deviceString)){
                        continue;
                    }

                    libusb_device_handle* handle = nullptr;
                    usb_err = libusb_open(devices[i], &handle);
                    if (usb_err < 0)
                    {
                        continue;
                    }

                    unsigned char manufacturer[256];
                    unsigned char product[256];
                    unsigned char serialNumber[256];
                    if (!desc.iManufacturer || libusb_get_string_descriptor_ascii(handle, desc.iManufacturer, manufacturer,256) < 0){
                        qWarning("Error getting manufacturer string from device: 0x%04x:0x%04x | Manufacturer string set as Unknown", desc.idVendor, desc.idProduct);
                    }

                    if (!desc.iProduct || libusb_get_string_descriptor_ascii(handle, desc.iProduct, product,256) < 0){
                        qWarning("Error getting product string from device: 0x%04x:0x%04x | Product string set as Unknown", desc.idVendor, desc.idProduct);
                    }

                    if (!desc.iSerialNumber || libusb_get_string_descriptor_ascii(handle, desc.iSerialNumber, serialNumber,256) < 0){
                        qWarning("Error getting serial number from device: 0x%04x:0x%04x | Serial number string set as Unknown", desc.idVendor, desc.idProduct);
                    }

                    libusb_close(handle);

                    QVariantMap deviceDetails;
                    deviceDetails.insert("product", QString(reinterpret_cast<char *>(product)));
                    deviceDetails.insert("manufacturer", QString(reinterpret_cast<char *>(manufacturer)));
                    deviceDetails.insert("serialNumber", QString(reinterpret_cast<char *>(serialNumber)));
                    deviceDetails.insert("vid", QString("%1").arg(desc.idVendor));
                    deviceDetails.insert("pid", QString("%1").arg(desc.idProduct));

                    emit usbDeviceAdded(deviceDetails);
                }
            }
            libusb_free_device_list(devices, 1);
        }
    }
    qDebug() << "Exiting UsbConnectionListener";

    libusb_hotplug_deregister_callback(hotplug_context, callback_handle);

    libusb_exit(hotplug_context);
}
