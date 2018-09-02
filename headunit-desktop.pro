TEMPLATE = subdirs
SUBDIRS = app.pro \
          module-volume-control \
          module-phone-bluetooth \
          module-android-auto \
          module-media-player \
          module-odb-car \
          module-usbconnectionlistener \
          module-welle-io

app.subdir = ./
module-volume-control.subdir           = modules/volume-control
module-phone-bluetooth.subdir          = modules/phone-bluetooth
module-android-auto.subdir             = modules/android-auto
module-media-player.subdir             = modules/media-player
module-odb-car.subdir                  = modules/odb-car
module-usbconnectionlistener.subdir    = modules/usbconnectionlistener
module-welle-io.subdir                 = modules/welle-io

app.depends = module-volume-control module-phone-bluetooth module-android-auto module-media-player module-odb-car module-usbconnectionlistener module-welle-io
