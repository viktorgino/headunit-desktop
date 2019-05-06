QT += quick
CONFIG += c++11

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
        main.cpp

RESOURCES += qml/navit_qml.qrc

QMAKE_LFLAGS += -Wl,-rpath,"'$$PWD/../navit/build/navit/'"

LD_LIBRARY_PATH += $$PWD/../navit/build/navit/

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target



unix:!macx: LIBS += -L/home/gino/projects/build-navit-Desktop_Qt_5_11_1_GCC_64bit_system-Default/navit -lnavit_core

INCLUDEPATH += $$PWD/../navit/navit
DEPENDPATH += $$PWD/../navit/navit

DISTFILES += \
    navit.xml \
    navit_layout_bike.xml \
    navit_layout_car.xml \
    navit_layout_car_android.xml \
    navit_layout_car_dark.xml \
    navit_layout_car_simple.xml
