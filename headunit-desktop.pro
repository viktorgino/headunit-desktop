TEMPLATE = subdirs
SUBDIRS = app.pro \
          modules-volume-control

app.subdir = ./
modules-volume-control.subdir = modules/volume-control

app.depends = modules-volume-control
