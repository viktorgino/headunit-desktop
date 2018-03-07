TEMPLATE = subdirs
SUBDIRS = app.pro \
          modules-volume-control \
          modules-phone-bluetooth

app.subdir = ./
modules-volume-control.subdir = modules/volume-control
modules-phone-bluetooth.subdir = modules/phone-bluetooth

app.depends = modules-volume-control modules-phone-bluetooth
