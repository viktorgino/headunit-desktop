# HeadUnit Desktop

**Table of Contents**

- [HeadUnit Desktop](#headunit-desktop)
	- [What is this?](#what-is-this)
	- [API documentation](#api-documentation)
	- [Build targets](#build-targets)
	- [Setting the Raspberry Pi up](#setting-the-raspberry-pi-up)
- [How to get the code working?](#how-to-get-the-code-working)
	- [Install Qt](#install-qt)
	- [Install the prerequisites for building and running headunit](#install-the-prerequisites-for-building-and-running-headunit)
	- [Build and install QtGstreamer](#build-and-install-qtgstreamer)
	- [Build headunit](#build-headunit)
	- [Building headunit with welle.io](#building-headunit-with-welleio)
		- [Make and install QtCharts](#make-and-install-qtcharts)
	- [Contributing to this project](#contributing-to-this-project)

What is this?
-------------
HeadUnit Desktop is a based free and open source software that is intended to be run on computers built into cars. This software is currently under active development and lot of the features are experimental. As of now there are two main features: 

 - Media player with a media library and media scanner
 - Android Auto™ client

Proposed features:

 - FM and DAB radio
 - CAN bus sniffer (depending on how much control different modules of the car allow and how much information, such as A/C steering wheel controls, different gauges and sensor data) with a customizable profile for each car.

The GUI for some of the proposed features is already there. For screenshots of the current state of the GUI [go to the screenshots page.](http://headunit.viktorgino.me/SCREENSHOTS)

----------

API documentation
----------------------------
There is an API documentation available at [http://headunit.viktorgino.me/html/index.html](http://headunit.viktorgino.me/html/index.html) within the docs you will find more information about the implementation details of this project. 

Build targets
----------------------------

There are for sub projects in the headunit-dekstop.pro file and five build targets. The build targets are:

| name             | make target                  | makefile               | description                                                                                                                                                                          |
|------------------|------------------------------|------------------------|--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| app              | sub-app-pro                  | ./Makefile             |  The main project, QML files from headunit-gui, C++ files and QML files from dab-gui are included here.                                                                              |
| app-with-welleio | sub-app-with-welleio-pro     | ./Makefile             | Builds the app with welleio included                                                                                                                                                 |
| dab-gui          | sub-dab-gui                  | ./Makefile             | A project to provide the embedding of welle.io into the main GUI                                                                                                                     |
| headunit-gui     | sub-qml_gui-headunit-gui-pro | ./Makefile             | The GUI for the headunit, this is mainly a QML project the C++ is there so it can be developed independently. The project can be found at https://github.com/viktorgino/headunit-gui |
| mediascanner     | sub-mediascanner             | /tests/manual/Makefile | Tests for the MediaScanner and MediaDB classes.                                                                                                                                      |
| qmllive          | sub-qmllive                  | /test/Makefile         | This is a project setup for easier developement of the GUI with QML Live. For building and installing see: http://doc.qt.io/QtQmlLive/index.html                                     |                                    |

Setting the Raspberry Pi up
----------------------------

For instructions on how to set a Raspberry Pi before compiling [go to the RPI-INSTRUCTIONS page](http://headunit.viktorgino.me/RPI-INSTRUCTIONS).

# How to get the code working? #


Install Qt
----------------------------

If Qt 5.7 or higher is avaialable through your distro then install it and the required QML modules using the apt package manager:

	sudo apt-get install qt5-default qml-module-qtquick2 qtdeclarative5-dev qtmultimedia5-dev libqt5multimedia5-plugins qml-module-qtquick* qml-module-qtmultimedia qml-module-qt-labs-settings

If Qt 5.7 or newer is not available through your operating system's package manager then you can get the x86 and x64 binaries from the Qt website. If it is available through your distro then you can skip to the next step.
Get the installer from Qt's official website and install it: [https://www.qt.io/download-open-source](https://www.qt.io/download-open-source)

After you've installed Qt add the *lib* folder its installed in to the library load path, the *bin* folder to your PATH and the *lib/pkgconfig* folder to the pkg-config path.

Install the prerequisites for building and running headunit
---------------------------------------------------------------

Install the neccesarry build tools:

	sudo apt-get install build-essential cmake protobuf-compiler

Install all the other dependencies:

	sudo apt-get install libusb-1.0-0-dev libssl-dev openssl libglib2.0-dev libgstreamer1.0-dev gstreamer1.0-plugins-base-apps gstreamer1.0-plugins-bad gstreamer1.0-libav gstreamer1.0-alsa libboost-dev libgstreamer-plugins-base1.0-dev libudev-dev libtag1-dev libprotobuf-dev libunwind-dev

Install Git:

	sudo apt-get install git

Change device permission for all devices that are made by known Android vendors and are connected through USB, so libusb can access them. Download and copy the `51-android.rules` file from https://github.com/snowdream/51-android to /etc/udev/rules.d/:

	sudo wget -P /etc/udev/rules.d https://raw.githubusercontent.com/snowdream/51-android/master/51-android.rules

Build and install QtGstreamer
-----------------------------

Clone QtGstreamer

	git clone git://anongit.freedesktop.org/gstreamer/qt-gstreamer
	cd qt-gstreamer

Create build dir:

	mkdir build && cd build

cmake QtGstreamer

	cmake .. -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_INSTALL_LIBDIR=lib/$(dpkg-architecture -qDEB_HOST_MULTIARCH) -DCMAKE_INSTALL_INCLUDEDIR=include -DQT_VERSION=5 -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS=-std=c++11

Make and install QtGstreamer

	make -j6 && sudo make install

Check if QtGstreamer is installed properly.

	gst-inspect-1.0 | grep qt5videosink

If the output is something similar, then it is installed properly:

	qt5videosink:  qt5videosink: Qt video sink
	qt5videosink:  qt5glvideosink: Qt GL video sink
	qt5videosink:  qwidget5videosink: QWidget video sink
	qt5videosink:  qtquick2videosink: QtQuick2 video sink

Build headunit
--------------

Clone this repo

	git clone --recursive --depth 1 -j6 https://github.com/viktorgino/headunit-desktop.git && cd headunit-desktop

Generate protobuf with protoc

	protoc --proto_path=headunit/hu/ --cpp_out=headunit/hu/generated.x64/ headunit/hu/hu.proto

And compile headunit-dekstop

	qmake && make -j4 sub-app-pro

If the program compiles without error then run it with `./app`

Building headunit with welle.io
--------------

To get DAB on the headunit you can build headunit desktop with welle.io integrated for more details on welle.io see [AlbrechtL/welle.io](https://github.com/AlbrechtL/welle.io). Please note that you'll need a RTL2832U based DVB-T/DAB adapter for it to work.

Install the following packages:

	sudo apt-get install libfaad-dev libfftw3-dev librtlsdr-dev libusb-1.0-0-dev mesa-common-dev libglu1-mesa-dev zlib1g-dev  portaudio19-dev libsndfile1-dev libsamplerate0-dev 

***Make and install QtCharts***

Download QtCharts into your home directory

	cd ~ && git clone https://github.com/qt/qtcharts.git -b 5.7

Change 5.7 to the major and minor version of Qt you have. After cloning chekout the exact version you have:

	cd qtcharts && git checkout tags/v5.7.1

Then build QtCharts:

	qmake CONFIG+=release && make -j4

Then install it:

	sudo make install

Finally build headunit-desktop with welle.io:

	cd ~/headunit-desktop && make clean && qmake && make -j4 sub-app-with-welleio-pro


----------

After completing the above steps you should be able to build the code with QT Creator an run it. If you have any problem, then reach out to me on XDA Developers [http://forum.xda-developers.com/member.php?u=6642908](http://forum.xda-developers.com/member.php?u=6642908) or drop me an email on [me@viktorgino.me](me@viktorgino.me)

Contributing to this project
----------------------------
If you like this project and you'd like to contribute to it, then I'm more than happy to merge pull requests.
If you would like to see a feature implemented or have some ideas you can open an issue here. You can also reach out to me on [me@viktorgino.me](me@viktorgino.me)

----------
This projects is licensed under GNU GENERAL PUBLIC LICENSE Version 3

*Android and Android Auto is a trademark of Google Inc.*

