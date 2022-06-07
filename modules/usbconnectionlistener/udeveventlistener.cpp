#include "udeveventlistener.h"

UdevEventListener::UdevEventListener(QObject *parent) : QThread(parent)
{

}
UdevEventListener::~UdevEventListener()
{
    stop();
}

void UdevEventListener::stop(){
    done = true;
    wait(QDeadlineTimer(10));
}

void UdevEventListener::run() {
    int ret = 0;

    struct udev *udev = udev_new();
    struct udev_device *dev;
    struct udev_monitor *mon;
    int fd;
    /* Set up a monitor to monitor USB devices */
    mon = udev_monitor_new_from_netlink(udev, "udev");
    if(mon == NULL) {
        qDebug() << "udev_monitor_new_from_netlink error : " << ret;
        return;
    }

    ret = udev_monitor_filter_add_match_subsystem_devtype(mon, "block", "disk");
    if(ret != 0) {
        qDebug() << "udev_monitor_filter_add_match_subsystem_devtype error : " << ret;
        return;
    }

    ret = udev_monitor_enable_receiving(mon);
    if(ret != 0){
        qDebug() << "udev_monitor_enable_receiving error : " << ret;
        return;
    }

    fd = udev_monitor_get_fd(mon);
    while(!done) {
        fd_set fds;
        struct timeval tv;
        int ret;

        FD_ZERO(&fds);
        FD_SET(fd, &fds);
        tv.tv_sec = 0;
        tv.tv_usec = 0;

        ret = select(fd+1, &fds, NULL, NULL, &tv);

        /* Check if our file descriptor has received data. */
        if (ret > 0 && FD_ISSET(fd, &fds)) {
            /* Make the call to receive the device. select() ensured that this will not block. */
            dev = udev_monitor_receive_device(mon);
            if (dev) {
                QString node = udev_device_get_devnode(dev);
                if(strcmp(udev_device_get_action(dev),"add") == 0){
                    QThread::sleep(1); //A bit how you doin, but it works...
                    struct mntent *ent;
                    FILE *aFile;

                    aFile = setmntent("/proc/mounts", "r");
                    if (aFile) {
                        while (NULL != (ent = getmntent(aFile))) {
                            QString fsName = ent->mnt_fsname;
                            if(fsName.startsWith(node)) {
                                qDebug() << "Mounted at : " << ent->mnt_dir;
                                emit deviceAdded(ent->mnt_dir);
                            }
                        }
                        endmntent(aFile);
                    }
                }
                udev_device_unref(dev);
            }
            else {
                qDebug() << "udev_monitor_receive_device error: no new device";
            }
        }
        QThread::usleep(2500);
    }
}
