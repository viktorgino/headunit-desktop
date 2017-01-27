#include "headunit.h"

#include <QDebug>
#include <QJsonObject>
#include <QGlib/Error>
#include <QGlib/Connect>
#include <QGst/Init>
#include <QGst/Bus>
#include <QGst/Pipeline>
#include <QGst/Parse>
#include <QGst/Message>
#include <QGst/Utils/ApplicationSink>
#include <QGst/Utils/ApplicationSource>
#include <gst/gst.h>
#include <gst/app/gstappsrc.h>
#include <gst/app/gstappsink.h>
#include "hu_uti.h"
#include "hu_aap.h"

Headunit::Headunit(const QGst::ElementPtr & sink):callbacks(this)
{
    m_videoSink = sink;
    int ret = 0;
    ret = initGst();
    if (ret < 0) {
        qDebug("STATUS:gst_pipeline_init() ret: %d", ret);
        return;
    }
}

Headunit::~Headunit() {
    setGstState("");

    gst_object_unref(vid_pipeline);
    gst_object_unref(mic_pipeline);
    gst_object_unref(aud_pipeline);
    gst_object_unref(au1_pipeline);

    gst_object_unref(vid_src);
    gst_object_unref(mic_sink);
    gst_object_unref(aud_src);
    gst_object_unref(au1_src);
    if(huStarted){
        headunit->hu_aap_shutdown();
        qDebug("Headunit::~Headunit() called hu_aap_shutdown()");
    }
}
int Headunit::startHU(){
    headunit = new HUServer(callbacks);
    int ret = headunit->hu_aap_start(ep_in_addr, ep_out_addr);
    if ( ret >= 0) {
        g_hu = &headunit->GetAnyThreadInterface();
        setGstState("play");
        return 1;
    } else {
        delete(headunit);
        setGstState("");
        qDebug("Phone is not connected. Connect a supported phone.");
        return 0;
    }
}

int Headunit::initGst(){
    GstBus *bus;
    GstStateChangeReturn state_ret;

    GError *error = NULL;

    gst_init(NULL, NULL);

    /*
     * Initialize Video pipeline
     */

    const char* vid_launch_str = "appsrc name=mysrc is-live=true block=false max-latency=100000 do-timestamp=true stream-type=stream typefind=true ! "
                                 "queue ! "
                                 "h264parse ! "
                                 "avdec_h264 ! "
                                 "videoscale ! "
                                 "videoconvert name=myconvert ";
    vid_pipeline = gst_parse_launch(vid_launch_str, &error);

    bus = gst_pipeline_get_bus(GST_PIPELINE(vid_pipeline));
    gst_bus_add_watch(bus, (GstBusFunc) Headunit::bus_callback, this);
    gst_object_unref(bus);


    GstElement *sink = QGlib::RefPointer<QGst::Element>(m_videoSink);
    GstElement *convert = gst_bin_get_by_name(GST_BIN(vid_pipeline), "myconvert");
    gst_bin_add(GST_BIN(vid_pipeline), GST_ELEMENT(sink));
    gst_element_link(convert, GST_ELEMENT(sink));

    vid_src = GST_APP_SRC(gst_bin_get_by_name(GST_BIN(vid_pipeline), "mysrc"));

    gst_app_src_set_stream_type(vid_src, GST_APP_STREAM_TYPE_STREAM);

    /*
     * Initialize Music pipeline
     */

    aud_pipeline = gst_parse_launch("appsrc name=audsrc is-live=true block=false max-latency=100000 do-timestamp=true ! "
                                    "audio/x-raw, signed=true, endianness=1234, depth=16, width=16, rate=48000, channels=2, format=S16LE ! "
                                    "alsasink buffer-time=400000 sync=false", &error);

    if (error != NULL) {
        qDebug("could not construct pipeline: %s", error->message);
        g_clear_error(&error);
        return -1;
    }

    aud_src = GST_APP_SRC(gst_bin_get_by_name(GST_BIN(aud_pipeline), "audsrc"));

    gst_app_src_set_stream_type(aud_src, GST_APP_STREAM_TYPE_STREAM);

    /*
     * Initialize Voice pipeline
     */

    au1_pipeline = gst_parse_launch("appsrc name=au1src is-live=true block=false max-latency=100000 do-timestamp=true ! "
                                    "audio/x-raw, signed=true, endianness=1234, depth=16, width=16, rate=16000, channels=1, format=S16LE  ! "
                                    "alsasink buffer-time=400000  sync=false", &error);

    if (error != NULL) {
        qDebug("could not construct pipeline: %s", error->message);
        g_clear_error(&error);
        return -1;
    }

    au1_src = GST_APP_SRC(gst_bin_get_by_name(GST_BIN(au1_pipeline), "au1src"));

    gst_app_src_set_stream_type(au1_src, GST_APP_STREAM_TYPE_STREAM);

    /*
     * Initialize Microphone pipeline
     */

    mic_pipeline = gst_parse_launch("alsasrc name=micsrc ! audioconvert ! "
                                    "audio/x-raw, signed=true, endianness=1234, depth=16, width=16, channels=1, rate=16000 ! "
                                    "queue ! "
                                    "appsink name=micsink async=false emit-signals=true blocksize=8192", &error);

    if (error != NULL) {
        qDebug("could not construct pipeline: %s", error->message);
        g_clear_error(&error);
        return -1;
    }

    mic_sink = gst_bin_get_by_name(GST_BIN(mic_pipeline), "micsink");

    g_object_set(G_OBJECT(mic_sink), "throttle-time", 3000000, NULL);

    g_signal_connect(mic_sink, "new-sample", G_CALLBACK(&Headunit::read_mic_data), NULL);

    gst_element_set_state(mic_pipeline, GST_STATE_READY);

    return 0;
}
void Headunit::read_mic_data(GstElement * sink) {
    GstSample *gstsample;
    GstBuffer *gstbuf;

    gstsample = gst_app_sink_pull_sample((GstAppSink *) sink);
    gstbuf = gst_sample_get_buffer(gstsample);

    if (gstbuf) {

        /* if mic is stopped, don't bother sending */
        GstState mic_state = GST_STATE_NULL;
        if (gst_element_get_state(mic_pipeline, &mic_state, NULL, GST_CLOCK_TIME_NONE) != GST_STATE_CHANGE_SUCCESS
                || mic_state != GST_STATE_PLAYING) {
            qDebug("Mic stopped.. dropping buffers ");
            gst_buffer_unref(gstbuf);
            return;
        }

        GstMapInfo mapInfo;
        if (gst_buffer_map(gstbuf, &mapInfo, GST_MAP_READ)) {

            if (mapInfo.size <= 64) {
                qDebug("Mic data < 64 ");
                gst_buffer_unref(gstbuf);
                return;
            }

            uint64_t bufTimestamp = GST_BUFFER_TIMESTAMP(gstbuf);
            uint64_t timestamp = GST_CLOCK_TIME_IS_VALID(bufTimestamp) ? (bufTimestamp / 1000) : get_cur_timestamp();
            g_hu->hu_queue_command([timestamp, gstbuf, mapInfo](IHUConnectionThreadInterface & s) {
                int ret = s.hu_aap_enc_send_media_packet(1, AA_CH_MIC, HU_PROTOCOL_MESSAGE::MediaDataWithTimestamp, timestamp, mapInfo.data, mapInfo.size);

                if (ret < 0) {
                    qDebug("read_mic_data(): hu_aap_enc_send() failed with (%d)", ret);
                }

                gst_buffer_unmap(gstbuf, const_cast<GstMapInfo*> (&mapInfo));
                gst_buffer_unref(gstbuf);
            });
        }
    }
}
uint64_t Headunit::get_cur_timestamp() {
    struct timespec tp;
    /* Fetch the time stamp */
    clock_gettime(CLOCK_REALTIME, &tp);

    return tp.tv_sec * 1000000 + tp.tv_nsec / 1000;
}
gboolean Headunit::bus_callback(GstBus *bus, GstMessage *message, gpointer *ptr) {
    Headunit *hu = (Headunit *) ptr;
    gchar *debug;
    GError *err;
    gchar *name;

    switch (GST_MESSAGE_TYPE(message)) {

    case GST_MESSAGE_ERROR:
        gst_message_parse_error(message, &err, &debug);
        qDebug("Error %s", err->message);
        g_error_free(err);
        g_free(debug);
        hu->setGstState("");
        break;

    case GST_MESSAGE_WARNING:
        gst_message_parse_warning(message, &err, &debug);
        qDebug("Warning %s | Debug %s", err->message, debug);

        name = (gchar *) GST_MESSAGE_SRC_NAME(message);

        qDebug("Name of src %s ", name ? name : "nil");
        g_error_free(err);
        g_free(debug);

        break;

    case GST_MESSAGE_EOS:
        qDebug("End of stream");
        hu->setGstState("");
        break;

    case GST_MESSAGE_STATE_CHANGED:
        break;
    }

    return TRUE;
}
void Headunit::setGstState(QString state){

    GstState gst_state;
    if(state == "play"){
        gst_state = GST_STATE_PLAYING;
        huStarted = true;
    } else if(state == "pause") {
        gst_state = GST_STATE_PAUSED;
        huStarted = false;
    } else {
        gst_state = GST_STATE_NULL;
        huStarted = false;
    }

    gst_element_set_state(vid_pipeline, gst_state);
    gst_element_set_state(aud_pipeline, gst_state);
    gst_element_set_state(au1_pipeline, gst_state);

}

bool Headunit::mouseDown(QPoint point){
    lastAction = HU::TouchInfo::TOUCH_ACTION_PRESS;
    touchEvent(HU::TouchInfo::TOUCH_ACTION_PRESS, &point);
    return true;
}
bool Headunit::mouseMove(QPoint point){
    if (lastAction == HU::TouchInfo::TOUCH_ACTION_PRESS) {
        if (point.x() > m_outputWidth) {
            QPoint falsepoint = QPoint(m_outputWidth, point.y());
            return mouseUp(falsepoint);
        }
        if (point.y() > m_outputHeight) {
            QPoint falsepoint = QPoint(point.x(), m_outputHeight);
            mouseUp(falsepoint);
        }
        if (point.x() < 1) {
            QPoint falsepoint = QPoint(1, point.y());
            return mouseUp(falsepoint);
        }
        if (point.y() < 1) {
            QPoint falsepoint = QPoint(point.x(), 1);
            return mouseUp(falsepoint);
        }
        touchEvent(HU::TouchInfo::TOUCH_ACTION_DRAG, &point);
    }
    return true;
}
bool Headunit::mouseUp(QPoint point){
    if (lastAction == HU::TouchInfo::TOUCH_ACTION_PRESS) {
        lastAction = HU::TouchInfo::TOUCH_ACTION_RELEASE;
        touchEvent(HU::TouchInfo::TOUCH_ACTION_RELEASE, &point);
    }
    return true;
}

bool Headunit::keyEvent(QString key){

}

void Headunit::touchEvent(HU::TouchInfo::TOUCH_ACTION action, QPoint *point) {
    float normx = float(point->x()) / float(m_outputWidth);
    float normy = float(point->y()) / float(m_outputHeight);

    unsigned int x = (unsigned int) (normx * videoWidth);
    unsigned int y = (unsigned int) (normy * videoHeight);

    g_hu->hu_queue_command([action, x, y](IHUConnectionThreadInterface & s) {
        HU::InputEvent inputEvent;
        inputEvent.set_timestamp(Headunit::get_cur_timestamp());
        HU::TouchInfo* touchEvent = inputEvent.mutable_touch();
        touchEvent->set_action(action);
        HU::TouchInfo::Location* touchLocation = touchEvent->add_location();
        touchLocation->set_x(x);
        touchLocation->set_y(y);
        touchLocation->set_pointer_id(0);

        /* Send touch event */

        int ret = s.hu_aap_enc_send_message(0, AA_CH_TOU, HU_INPUT_CHANNEL_MESSAGE::InputEvent, inputEvent);
        if (ret < 0) {
            qDebug("aa_touch_event(): hu_aap_enc_send() failed with (%d)", ret);
        }
    });
}

void Headunit::setUsbConnectionListener(UsbConnectionListener *m_connectionListener){
    connectionListener = m_connectionListener;
    connect(connectionListener,SIGNAL(androidDeviceConnected(QString)),this,SLOT(slotAndroidDeviceAdded(QString)));
    connect(connectionListener,SIGNAL(usbDriveConnected(QString)),this,SLOT(slotDeviceAdded(QString)));
    connect(connectionListener,SIGNAL(deviceRemoved(QString)),this,SLOT(slotDeviceRemoved(QString)));
}
void Headunit::slotDeviceAdded(const QString &dev)
{
    emit deviceConnected(QJsonObject {{"image","icons/usb.png"},
                                      {"title","New USB storage detected"},
                                      {"text",QString("%1 connected").arg(dev)}}.toVariantMap());
    qDebug("add %s", qPrintable(dev));
}
void Headunit::slotAndroidDeviceAdded(const QString &dev)
{
    startHU();
    emit deviceConnected(QJsonObject {{"image","icons/usb.png"},
                                      {"title","New Android device detected"},
                                      {"text",QString("%1 connected").arg(dev)}}.toVariantMap());
    qDebug("add %s", qPrintable(dev));
}

void Headunit::slotDeviceChanged(const QString &dev)
{
    qDebug("change %s", qPrintable(dev));
    emit deviceConnected(QJsonObject {{"image","icons/refresh.png"},{"title","USB device changed"},{"text",""}}.toVariantMap());
}

void Headunit::slotDeviceRemoved(const QString &dev)
{
    qDebug("remove %s", qPrintable(dev));
    emit deviceConnected(QJsonObject {{"image","icons/eject.png"},{"title","USB device removed"},{"text",""}}.toVariantMap());
}

int DesktopEventCallbacks::MediaPacket(int chan, uint64_t timestamp, const byte * buf, int len) {
    GstAppSrc* gst_src = nullptr;
    GstElement* gst_pipe = nullptr;
    if (chan == AA_CH_VID) {
        gst_src = headunit->vid_src;
        gst_pipe = headunit->vid_pipeline;
    } else if (chan == AA_CH_AUD) {
        gst_src = headunit->aud_src;
        gst_pipe = headunit->aud_pipeline;
    } else if (chan == AA_CH_AU1) {
        gst_src = headunit->au1_src;
        gst_pipe = headunit->au1_pipeline;
    }

    if (gst_src) {
        GstBuffer * buffer = gst_buffer_new_and_alloc(len);
        gst_buffer_fill(buffer, 0, buf, len);
        int ret = gst_app_src_push_buffer((GstAppSrc *) gst_src, buffer);
        if (ret != GST_FLOW_OK) {
            qDebug("push buffer returned %d for %d bytes ", ret, len);
        }
    }
    return 0;
}

int DesktopEventCallbacks::MediaStart(int chan) {
    if (chan == AA_CH_MIC) {
        qDebug("SHAI1 : Mic Started");
        gst_element_set_state(headunit->mic_pipeline, GST_STATE_PLAYING);
    }
    return 0;
}

int DesktopEventCallbacks::MediaStop(int chan) {
    if (chan == AA_CH_MIC) {
        qDebug("SHAI1 : Mic Stopped");
        gst_element_set_state(headunit->mic_pipeline, GST_STATE_READY);
    }
    return 0;
}

void DesktopEventCallbacks::DisconnectionOrError() {
    qDebug("Android Device disconnected, pausing gstreamer");
    headunit->setGstState("");
}

void DesktopEventCallbacks::CustomizeOutputChannel(int chan, HU::ChannelDescriptor::OutputStreamChannel& streamChannel) {

}
