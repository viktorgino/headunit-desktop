#!/bin/bash

IMGFILE=""
MOUNTPOINT="$HOME/rpi_mnt"
COPYFOLDER=""
CLEAN=0
WELLEIO=0
DEST=0
DRYRUN=0

#Display help
if [ "$1" == "-h" ] || [ "$1" == "--help" ]; then
cat<< EOF
viktorgino's headunit-desktop compile script
https://github.com/viktorgino/headunit-desktop

Usage: recompile-pi-qemu [options]
  options
     ______________________ ________________________________________________________________
    | -i | --imagefile     | Specify the .img file to use.                                  |
    |    |                 | An image file with all the prerequisites installed is expected |
    | -m | --mountpoint    | The mount point for the image. ~/rpi_mnt is used by default    |
    | -c | --copyfolder    | Compile from a local folder rather than from git               |
    | -x | --clean         | run "make clean" before compiling                              |
    | -w | --welleio       | compile with welle.io, set -x when recompiling                 |
    | -d | --dryrun        | run without compiling                                          |
    | -r | --rsync         | If set to a valid destination it will rsync the new files over |
    |    |                 | ie: pi@raspberrypi:/opt/headunit-desktop                       |
    | -h | --help          | Display this help                                              |
     ¯¯¯¯ ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯ ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯
EOF
    exit
fi

while [[ $# -gt 0 ]]
do
key="$1"

case $key in
    -i|--imagefile) #
    IMGFILE="$2"
    shift
    ;;
    -m|--mountpoint) #Specify mount point, otherwise use ~/rpi_mnt
    MOUNTPOINT="$2"
    shift
    ;;
    -c|--copyfolder) #Copy local version of headunit instead of git
    COPYFOLDER="$2"
    shift
    ;;
    -r|--rsync) #rsync files over
    DEST="$2"
    shift
    ;;
    -x|--clean) #Copy local version of headunit instead of git
    CLEAN=1
    ;;
    -w|--welleio) #rsync files over
    WELLEIO=1
    ;;
    -d|--dryrun) #rsync files over
    DRYRUN=1
    echo "Running DRY"
    ;;
    *)
    echo "The option $key is unknown."
    exit
    ;;
esac
shift
done

#Check if sudo
if [[ $EUID > 0 ]]; then
  echo "Please run as root/sudo"
  exit 1
fi
#Check if the imagefile is set
if [ "$IMGFILE" == "" ]; then
    echo "Image file is not set"
    exit
fi

#Create mounting point dir
mkdir -p "$MOUNTPOINT"

#Make a couple of loopback devices for the whole image and its partitions
loop_back_dev=$(losetup -f -P --show $IMGFILE)
echo "Loop back device created at $loop_back_dev"

# Mount the image		
mount $loop_back_dev"p2" -o rw "$MOUNTPOINT"
echo "$loop_back_dev""p2 mounted at $MOUNTPOINT"

mount $loop_back_dev"p1" -o rw "$MOUNTPOINT"/boot
echo "$loop_back_dev""p1 mounted at $MOUNTPOINT/boot"

#Take care of etc/ld.so.preload
mv "$MOUNTPOINT"/etc/ld.so.preload "$MOUNTPOINT"/etc/ld.so.preload.orig
echo "" | tee -a  "$MOUNTPOINT"/etc/ld.so.preload

#Copy QemuUserEmulation binary to the chroot
cp /usr/bin/qemu-arm-static "$MOUNTPOINT"/usr/bin


#Copy compile script
rsync compile-local.sh "$MOUNTPOINT"/opt/compile-headunit.sh

#Copy local files
if [ "$COPYFOLDER" == "" ]; then
    echo "Compiling from git"
else
    echo "Compiling from local files ($COPYFOLDER)"
    #if [ ! -d /opt/headunit-desktop ] || [ "$CLEAN" == 1 ]; then
    #    rm -rf "$MOUNTPOINT"/opt/headunit-desktop
    #fi
    rsync -a --exclude='headunit-desktop/.git/' $COPYFOLDER "$MOUNTPOINT"/opt
fi

cat << EOF

***************************************************
                Starting compiling
***************************************************

EOF
#chroot in and build headunit
chroot $MOUNTPOINT << EOF
export CLEAN=${CLEAN}
export WELLEIO=${WELLEIO}
export DRYRUN=${DRYRUN}
export COPYFOLDER=${COPYFOLDER}
chmod +x /opt/compile-headunit.sh
/opt/compile-headunit.sh
chown -R pi:pi /opt/headunit-desktop
EOF

cat << EOF

***************************************************
                Compilation finished
***************************************************

EOF

#rsync files over to Pi
rsync -av "$MOUNTPOINT"/opt/headunit-desktop "$DEST"

### Clean up

#Restore /etc/ld.so.preload
rm "$MOUNTPOINT"/etc/ld.so.preload
mv "$MOUNTPOINT"/etc/ld.so.preload.orig "$MOUNTPOINT"/etc/ld.so.preload
echo "/etc/ld.so.preload restored"
#Remove compile script
#rm "$MOUNTPOINT"/opt/compile-headunit.sh

#remove QemuUserEmulation binary from rpi root
rm "$MOUNTPOINT"/usr/bin/qemu-arm-static

#Unmount
umount "$MOUNTPOINT"/boot
umount "$MOUNTPOINT"
echo "$MOUNTPOINT and $MOUNTPOINT/boot unmounted" 

#Remove loop devices
losetup -d $loop_back_dev
echo "$loop_back_dev detached" 