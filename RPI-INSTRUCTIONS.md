Setting up the PI
-----------------------------------------------

*Follow these steps to setup the necessary packages and make the necessary changes to Raspbian in order to run the code in this repo. These steps were reproduced on a Raspberry PI 3 using the official LCD screen.*

Grab the latest Raspbian Jessie Lite image from : https://www.raspberrypi.org/downloads/raspbian/ . Unzip it and write it to an SD Card and extend the root partition of the SD card to fill out the empty space. (either with gparted, resize2fs or when Raspbian booted with raspi-config)

Setup up the image for the Pi
-----------------------------------------------
Mount the SD card/USB drive on a PC which has an OS that supports ext4 filesystems. The commands here are for Debian, Ubuntu ...etc

### Setup the WiFi ###

Edit the the /etc/wpa_supplicant/wpa_supplicant.conf file in Raspbian's root:

	sudo nano /MOUNTING_POINT/etc/wpa_supplicant/wpa_supplicant.conf

and add the following to it:

	network={
	     ssid="Your Wifi's ESSID"
	     psk="Your wifi's password"
	}

#### *Optional: Rotate the screen* ####

If you are using the official LCD screen with the case then your screen will be upside down, so also add the following to the /boot/config.txt

	#Rotate screen
	display_rotate=2

### Increase the GPU memory to 256MB ###

Add the following line to /boot/config.txt

	gpu_mem=256

### Enable SSH access to the PI ###

Create a file in the /MOUNTING_POINT/boot dir named ssh

	echo "" > /MOUNTING_POINT/boot/ssh

### Update the kernel to 4.9.9 ###

You will also need to update the Pi's kernel to the latest version (the current kernel version is 4.8.0) to get the Pi LCD on the DSI working with the vc4 driver. If you use a display connected through the HDMI port, then you can skip this steps.
 Get rpi-update and do an offline update of the Raspbian kernel on your Linux machine. Install rpi-update on a Linux PC with the following:

	sudo curl -L --output /usr/bin/rpi-update https://raw.githubusercontent.com/Hexxeh/rpi-update/master/rpi-update && sudo chmod +x /usr/bin/rpi-update

Then run it with the following arguments:

	sudo ROOT_PATH=/MOUNTING_POINT/root BOOT_PATH=/MOUNTING_POING/boot BRANCH=next rpi-update

Setup boot from USB
-----------------------------------------------

>*This step is optional, but booting from USB makes it easier to access the Pi's filesystem if something goes wrong*

>To enable USB boot mode follow the steps from here: https://github.com/raspberrypi/documentation/blob/master/hardware/raspberrypi/bootmodes/msd.md

Update Raspbian to Stretch
--------------------------

*To have Qt5.7 you either need to cross-compile it or update Raspbian from Jessie to Stretch and install it from the Raspbian package repository. Here I'll explain how to get Raspbian Stretch running on your Pi. If you'd rather cross-compile Qt5.7 then follow the steps from the [Qt Wiki](https://wiki.qt.io/RaspberryPi2EGLFS), but please beware that it takes hours (even on a decent PC with 100% CPU utilisation) to compile qtbase plus all the needed modules.*

When done unmount your USB drive / SD card insert it into the PI then boot it up. Login into the PI using SSH (either by using the ssh command or using PuTTy). The default username is pi the password is raspberry

To update to stretch add the following line top the /etc/apt/sources.list file:

	deb http://mirrordirector.raspbian.org/raspbian/ stretch main contrib non-free rpi

Run the update

	sudo apt-get update && sudo apt-get -y dist-upgrade

Remove libnettle4:

	sudo apt-get remove libnettle4

Autoremove all the uneeded packages:

	sudo apt autoremove

Install the mesa driver

	sudo apt-get install libgl1-mesa-dri

Get the driver for the onboard WLAN adapter from the RPi-Distro repo

	sudo wget -P /lib/firmware/brcm/ https://raw.githubusercontent.com/RPi-Distro/firmware-nonfree/master/brcm80211/brcm/brcmfmac43430-sdio.bin
	sudo wget -P /lib/firmware/brcm/ https://raw.githubusercontent.com/RPi-Distro/firmware-nonfree/master/brcm80211/brcm/brcmfmac43430-sdio.txt


Change Qt's default platform plugin to EGLFS by running the following command:

	echo "export QT_QPA_PLATFORM=eglfs" >> ~/.profile

To enable the OpenGL driver run `sudo raspi-config` then go to *Advanced Options* and select *GL (Fake KMS) OpenGL desktop driver with fake KMS* confirm that you want to enable the GL driver, press enter and click on finish in the raspi-config menu.

Reboot the PI

	sudo reboot

Install the prerequisites for building and running headunit
---------------------------------------------------------------

Install the neccesarry build tools:

	sudo apt-get install build-essential qt5-default qml-module-qtquick2 cmake protobuf-compiler
	
Install the mesa driver

	sudo apt-get install libgl1-mesa-dri

Install all the requirements:

	sudo apt-get install libusb-1.0-0-dev libssl-dev openssl libglib2.0-dev libgstreamer1.0-dev gstreamer1.0-plugins-base-apps gstreamer1.0-plugins-bad gstreamer1.0-libav gstreamer1.0-alsa libboost-dev libgstreamer-plugins-base1.0-dev qtdeclarative5-dev qtmultimedia5-dev libqt5multimedia5-plugins qml-module-qtquick* qml-module-qtmultimedia qml-module-qt-labs-settings libudev-dev libtag1-dev libprotobuf-dev libunwind-dev

Install Git:

	sudo apt-get install git

Change device permission for all devices that are made by known Android vendors and are connected through USB, so libusb can access them. Download and copy the `51-android.rules` file from https://github.com/snowdream/51-android to /etc/udev/rules.dev/:

	sudo wget -P /etc/udev/rules.d https://raw.githubusercontent.com/snowdream/51-android/master/51-android.rules
