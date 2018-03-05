TEMPLATE = subdirs
SUBDIRS = qml \
          plugin


qml.subdir = qml
plugin.subdir = plugin
plugin.depends = qml
