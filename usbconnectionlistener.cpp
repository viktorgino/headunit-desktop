#include "usbconnectionlistener.h"
#include <QDirIterator>
#include <QDebug>
#include <mntent.h>
#include <unistd.h>
#include <fcntl.h>
#include "hu_uti.h"
UsbConnectionListener::UsbConnectionListener(){
    if (libusb_init(&hotplug_context) < 0)
    {
        qDebug ("Error libusb_init usb_err failed");
        return;
    }
    libusb_set_debug(hotplug_context, LIBUSB_LOG_LEVEL_INFO);

    if (libusb_has_capability (LIBUSB_CAP_HAS_HOTPLUG)) {
        int rc;
        rc = libusb_hotplug_register_callback(hotplug_context, static_cast<libusb_hotplug_event>(LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED |
                                                                                                 LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT)
                                              , LIBUSB_HOTPLUG_NO_FLAGS , LIBUSB_HOTPLUG_MATCH_ANY,
                                              LIBUSB_HOTPLUG_MATCH_ANY, LIBUSB_HOTPLUG_MATCH_ANY, hotplugCallback, this, NULL);
        if (rc != LIBUSB_SUCCESS) {
            qDebug("libusb_hotplug_register_callback() failed: Error registering callback 0");
            return;
        }
    }
}

UsbConnectionListener::~UsbConnectionListener(){
    if (libusb_has_capability (LIBUSB_CAP_HAS_HOTPLUG)) {
        libusb_hotplug_deregister_callback(hotplug_context, LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED |
                                           LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT);
    }
    libusb_exit(hotplug_context);
}

int LIBUSB_CALL UsbConnectionListener::hotplugCallback(libusb_context */* unused */, libusb_device *dev, libusb_hotplug_event event, void *user_data) {
    struct libusb_device_descriptor desc;
    int rc;

    rc = libusb_get_device_descriptor(dev, &desc);
    if (LIBUSB_SUCCESS != rc) {
        qWarning("Error getting device descriptor");
    }

    UsbConnectionListener *parent = static_cast<UsbConnectionListener *>(user_data);

    if(event == LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED){
        libusb_device_handle* handle = nullptr;
        uint8_t deviceClass = desc.bDeviceClass;
        unsigned char manufacturer[256];
        unsigned char product[256];
        unsigned char serialNumber[256];
        QString device;

        if (libusb_open(dev, &handle) < 0)
        {
            qWarning("Error opening device: 0x%04x | 0x%04x", desc.idVendor, desc.idProduct);
            return -1;
        }
        if (libusb_get_string_descriptor_ascii(handle, desc.iManufacturer, manufacturer,256) < 0){
            qWarning("Error getting manufacturer string from device: 0x%04x:0x%04x | Manufacturer string set as Unknown", desc.idVendor, desc.idProduct);
        }

        if (libusb_get_string_descriptor_ascii(handle, desc.iProduct, product,256) < 0){
            qWarning("Error getting product string from device: 0x%04x:0x%04x | Product string set as Unknown", desc.idVendor, desc.idProduct);
        }

        if (libusb_get_string_descriptor_ascii(handle, desc.iSerialNumber, serialNumber,256) < 0){
            qWarning("Error getting serial number from device: 0x%04x:0x%04x | Serial number string set as Unknown", desc.idVendor, desc.idProduct);
        }

        QTextStream(&device) << reinterpret_cast<char *>(manufacturer) << " " << reinterpret_cast<char *>(product);
        uint8_t path[8];
        QString deviceInfo;
        QTextStream(&deviceInfo) << "Device attached: "
                                 << device.toLatin1().data()
                                 << " " << desc.idVendor
                                 << ":" << desc.idProduct
                                 << " (bus " << libusb_get_bus_number(dev)
                                 <<", device "<<libusb_get_device_address(dev);
        int r = libusb_get_port_numbers(dev, path, sizeof(path));
        if (r > 0) {
            QTextStream(&deviceInfo) << " path: " <<path[0];
            for (int j = 1; j < r; j++)
                QTextStream(&deviceInfo) << "." <<path[j];
        }
        QTextStream(&deviceInfo) << ") | Serial no.: "<<reinterpret_cast<char *>(serialNumber)<<" | bDeviceClass:"<<deviceClass;

        qDebug(deviceInfo.toLatin1());
        if(UsbConnectionListener::isAndroidDevice(handle) && !UsbConnectionListener::isOapDevice(&desc)){
            parent->emitSignal(USB_EVENT_TYPE::ANDROID_DEVICE_CONNECTED,device);
        } else if(UsbConnectionListener::hasMassStorageInterface(dev)) {
            QList<QString> mountList;
            //http://stackoverflow.com/questions/5070801/monitoring-mount-point-changes-via-proc-mounts/5072579
            int mfd = open("/proc/mounts", O_RDONLY, 0);
            fd_set rfds;
            struct timeval tv;
            //Set timeout to 5 seconds
            int timeout = 5;
            FD_ZERO(&rfds);
            FD_SET(mfd, &rfds);
            tv.tv_sec = timeout;

            int ret;
            bool triggered = false;
            //Wait for the mass storage device to mount
            while((ret= select(mfd+1, NULL, NULL, &rfds, &tv)) >= 0){
                if(ret > 0 && FD_ISSET(mfd, &rfds)){
                    triggered = true;
                } else if(ret ==  0) {
                    if(triggered){
                        mountList = UsbConnectionListener::getMountPoint(reinterpret_cast<char *>(&serialNumber));
                        parent->emitSignal(USB_EVENT_TYPE::MASS_STORAGE_CONNECTED,device,mountList);
                    } else {
                        qWarning("Error: Detected USB Mass Storage didn't mount within %d seconds", timeout);
                    }
                    break;
                } else {
                    qDebug("Error: while waiting for USB Mass Storage to mount");
                    break;
                }

                FD_ZERO(&rfds);
                FD_SET(mfd, &rfds);
                tv.tv_sec = timeout;
            }
        }

        libusb_close(handle);
    } else if(event == LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT){
        parent->emitSignal(USB_EVENT_TYPE::USB_REMOVED,"device");
    }
    return 0;
}

void UsbConnectionListener::stop(){
    done = true;
}

void UsbConnectionListener::emitSignal(USB_EVENT_TYPE event_type, QString device_descriptor){
    switch(event_type){
    case USB_EVENT_TYPE::USB_REMOVED:
        emit deviceRemoved("");
        break;
    case USB_EVENT_TYPE::ANDROID_DEVICE_CONNECTED:
        emit androidDeviceConnected(device_descriptor);
        break;
    case USB_EVENT_TYPE::MASS_STORAGE_CONNECTED:
    case USB_EVENT_TYPE::USB_CONNECTED:
    default:
        break;
    }
}

void UsbConnectionListener::emitSignal(USB_EVENT_TYPE event_type, QString device_descriptor, QList<QString> mountList){
    emit usbDriveConnected(device_descriptor);
}

void UsbConnectionListener::run() {
    timeval tv = { 0 };
    while(!done){
        int rc;
        rc = libusb_handle_events_timeout_completed(hotplug_context,&tv,NULL);
        if (rc < 0) {
            qDebug("libusb_handle_events() failed: %s", libusb_error_name(rc));
            break;
        }
    }
}

//https://github.com/gregkh/usbutils/blob/master/lsusb.c
bool UsbConnectionListener::hasMassStorageInterface(libusb_device *dev){
    libusb_device_handle *handle;
    libusb_device_descriptor desc;
    int ret;

    ret = libusb_open(dev, &handle);

    if (ret) {
        qWarning("libusb_open() : Couldn't open device");
        return false;
    }
    ret = libusb_get_device_descriptor(dev, &desc);

    if (ret) {
        qWarning("libusb_get_device_descriptor() : Couldn't get device descriptor");
        return false;
    }

    if (desc.bNumConfigurations) {
        struct libusb_config_descriptor *config;

        for (int i = 0; i < desc.bNumConfigurations; ++i) {
            ret = libusb_get_config_descriptor(dev, i, &config);
            if (ret) {
                qWarning("libusb_get_config_descriptor()  : Couldn't get device config descriptor", i);
                return false;
            } else {
                for (i = 0 ; i < config->bNumInterfaces ; i++){
                    const libusb_interface *interface = &config->interface[i];
                    for (int j = 0; j < interface->num_altsetting; j++){
                        const libusb_interface_descriptor *interface2 = &interface->altsetting[i];
                        if(interface2->bInterfaceClass == LIBUSB_CLASS_MASS_STORAGE){
                            libusb_close(handle);
                            return true;
                        } else {
                            continue;
                        }
                    }
                }
            }
            libusb_free_config_descriptor(config);
        }
    }
    return false;
}

//http://stackoverflow.com/questions/9280759/linux-function-to-get-mount-points
QList<QString> UsbConnectionListener::getMountPoint(QString serialNumber){
    QDirIterator it("/dev/disk/by-id", QDirIterator::NoIteratorFlags);
    QList<QString> returnList;
    while (it.hasNext()) {
        QString currentPath = it.next();
        QString temp;
        if(currentPath.contains(serialNumber)){
            currentPath = QFile::symLinkTarget(currentPath);

            struct mntent *ent;
            FILE *aFile;

            aFile = setmntent("/proc/mounts", "r");
            if (aFile == NULL) {
                qWarning("Error: setmntent failed to open file system descriptor file");
            } else {
                while (NULL != (ent = getmntent(aFile))) {
                    if(QString::compare(currentPath,ent->mnt_fsname)==0){
                        returnList.append(ent->mnt_dir);
                    }
                }
            }
            endmntent(aFile);
        }
    }
    return returnList;
}

bool UsbConnectionListener::isAndroidDevice(libusb_device_handle *device){
    uint16_t oap_proto_ver = 0;

    if (libusb_control_transfer(device, USB_DIR_IN | USB_TYPE_VENDOR, ACC_REQ_GET_PROTOCOL, 0, 0, (unsigned char*)&oap_proto_ver, sizeof(uint16_t), 1000) < 0){
        return false;
    }

    oap_proto_ver = le16toh(oap_proto_ver);
    if (oap_proto_ver < 1)
    {
        return false;
    }
    return true;
}

bool UsbConnectionListener::isOapDevice(libusb_device_descriptor *desc){
    if(desc->idVendor == VEN_ID_GOOGLE){
        if(desc->idProduct == DEV_ID_OAP || desc->idProduct == DEV_ID_OAP_WITH_ADB){
            return true;
        }
    }
    return false;
}
