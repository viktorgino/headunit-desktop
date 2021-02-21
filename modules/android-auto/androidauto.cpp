#include "androidauto.h"

#ifdef BUILD_QTGST
#include <gst/gstplugin.h>
#include "qtgst/gstqtsink.h"
#include <QtQml/QQmlApplicationEngine>



gboolean qt_gst_plugin_init (GstPlugin * plugin)
{
    qDebug() << "Loading QtGst plugin";
    if (!gst_element_register (plugin, "qmlglsink",
                               GST_RANK_NONE, GST_TYPE_QT_SINK)) {
        qDebug () << "Error registering qmlglsink";
    }
    /* this means the plugin must be loaded before the qml engine is loaded */
    qmlRegisterType<QtGLVideoItem> ("org.freedesktop.gstreamer.GLVideoItem", 1, 0, "GstGLVideoItem");

    return TRUE;
}
#endif

AndroidAuto::AndroidAuto(QObject *parent) : QObject (parent), headunit(this)
{
    m_pluginSettings.eventListeners = QStringList() << "UsbConnectionListenerPlugin::UsbDeviceAdded";

    gst_init(NULL, NULL);
#ifdef BUILD_QTGST
    qDebug() << "Loading registering QtGst";
    gst_plugin_register_static (
                GST_VERSION_MAJOR,
                GST_VERSION_MINOR,
                "qmlgl",
                "Qt gl plugin",
                qt_gst_plugin_init,
                "1.13.0.1",
                "LGPL",
                "GStreamer Bad Plug-ins (qmake)",
                "GStreamer",
                "Unknown package origin");
#endif
}

QObject *AndroidAuto::getContextProperty(){
    return qobject_cast<QObject *>(&headunit);
}

void AndroidAuto::eventMessage(QString id, QVariant message){
    if(id == "UsbConnectionListenerPlugin::UsbDeviceAdded"){
        qDebug () << "AndroidAuto event UsbConnectionListenerPlugin::UsbDeviceAdded : " << message;
        headunit.startHU();
    } else if(id == "UsbConnectionListenerPlugin::UsbDeviceRemoved"){
        qDebug () << "AndroidAuto event UsbConnectionListenerPlugin::UsbDeviceRemoved : " << message;
    }
}
void AndroidAuto::init(){
    headunit.init();
}
