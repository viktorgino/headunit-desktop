# HeadUnit Desktop

What is this?
-------------
HeadUnit Desktop is a free and open source software that is intended to be run on computers built into cars. This software is currently under active development and lot of the features are experimental. As of now there are two main features: 

 - Media player with a media library and media scanner
 - Android Auto™ client

For screenshots see the http://headunit.viktorgino.me/SCREENSHOTS

----------
How to get the code working?
----------------------------
For instruction on to get it setup on a Raspberry Pi and Raspbian [click here](https://github.com/viktorgino/headunit/blob/master/RPi_instructions.md).
Install QT5 and you will probably want to install QT Creator with it as well. You can get it with apt-get:

    sudo apt-get install qtcreator

This should install all the files required to get started with developing QT, but it is very likely, that you will get an outdated version of it so get it from the official site https://www.qt.io/download-open-source
After you've installed Qt add its libraries to the library load path. On Ubuntu/Debian x64 just add the following line to /etc/ld.so.conf/x86_64-linux-gnu.conf :

    /home/YOUR_USERNAME/Qt/5.7/gcc_64/lib

Install the following packages:

    libusb-1.0-0-dev libssl-dev libgstreamer1.0-dev gstreamer1.0-plugins-base-apps gstreamer1.0-plugins-bad gstreamer1.0-libav libboost-dev libgstreamer-plugins-base1.0-dev 

**Build and install QtGStreamer**

Get the latest code for QtGStreamer from https://cgit.freedesktop.org/gstreamer/qt-gstreamer configure it with CMake and install it on your system. Most of the build steps are detailed in its README file. You might want to install cmake-qt-gui to make it easier:

    sudo apt-get install cmake-qt-gui

With the cmake gui installed just open the folder where you've cloned the qt-gstreamer git, specify where you want to build it (I just used the same dir as it makes generating the Doxygen document easier) and click generate (if it throws up an error message just correct it). You will need tell cmake to use QT5:

    -DQT_VERSION=5
    -CMAKE_CXX_FLAGS=-std=c++11
   After succesfully generating the make file with cmake just open the terminal and cd into the folder you specified as build directory in and run: `make && sudo make install`

After completing the above steps you should be able to build the code with QT Creator an run it. If you have any problem, then reach out to me on XDA Developers (http://forum.xda-developers.com/member.php?u=6642908) or drop me an email on me@viktorgino.me

----------
Android and Android Auto is a trademark of Google Inc.
