CONFIG += qtquickcompiler

isEmpty(PREFIX) {
    PREFIX = /opt/headunit-desktop
}

rpi{
DEFINES += RPI
}
