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
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QSettings>
#include <QFileInfo>
#include <gst/gst.h>
#include <gst/app/gstappsrc.h>
#include <gst/app/gstappsink.h>
#include <gst/video/video.h>
#include "hu_uti.h"
#include "hu_aap.h"

Headunit::Headunit(const QGst::ElementPtr & sink):callbacks(this)
{
    m_videoSink = sink;
    int ret = 0;
    ret = initGst();
    if (ret < 0) {
        qDebug("STATUS:initGst() ret: %d", ret);
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
    QSettings settings;
    std::map<std::string, std::string> aa_settings;

    if(settings.childGroups().contains("AndroidAuto")){
        settings.beginGroup("AndroidAuto");
        QStringList keys = settings.childKeys();

        for (int i = 0; i < keys.size(); ++i){
            aa_settings[keys.at(i).toStdString()] = settings.value(keys.at(i)).toString().toStdString();
        }
    }

    headunit = new HUServer(callbacks, aa_settings);

    int ret = headunit->hu_aap_start(false);
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

int Headunit::restartHU(){
    if(huStarted){
        headunit->hu_aap_shutdown();
        qDebug("Headunit::~Headunit() called hu_aap_shutdown()");
    }
    startHU();
    return 1;
}
GstPadProbeReturn Headunit::convert_probe(GstPad *pad, GstPadProbeInfo *info, void *user_data){
    GstEvent *event = GST_PAD_PROBE_INFO_EVENT(info);
    if (GST_PAD_PROBE_INFO_TYPE(info) & GST_PAD_PROBE_TYPE_EVENT_DOWNSTREAM) {
        if (GST_EVENT_TYPE (event) == GST_EVENT_SEGMENT) {
            GstCaps *caps  = gst_pad_get_current_caps(pad);
            if(caps != NULL){
                GstVideoInfo *vinfo = gst_video_info_new ();
                gst_video_info_from_caps (vinfo, caps);
                Headunit *headunit = static_cast<Headunit *>(user_data);
                headunit->setVideoWidth(vinfo->width);
                headunit->setVideoHeight(vinfo->height);
            }
            return GST_PAD_PROBE_REMOVE;
        }
    }
    return GST_PAD_PROBE_OK;
}

int Headunit::initGst(){
    GstBus *bus;

    GError *error = NULL;

    gst_init(NULL, NULL);

    /*
     * Initialize Video pipeline
     */

    const char* vid_launch_str = "appsrc name=mysrc is-live=true block=false max-latency=100 do-timestamp=true stream-type=stream ! "
                                 "queue ! "
                                 "h264parse ! "
                       #ifdef RPI
                                 "omxh264dec ! "
                       #else
                                 "avdec_h264 lowres=1 skip-frame=2 ! "
                       #endif
                                 "capsfilter caps=video/x-raw name=mycapsfilter";
    vid_pipeline = gst_parse_launch(vid_launch_str, &error);

    bus = gst_pipeline_get_bus(GST_PIPELINE(vid_pipeline));
    gst_bus_add_watch(bus, (GstBusFunc) Headunit::bus_callback, this);
    gst_object_unref(bus);


    GstElement *sink = QGlib::RefPointer<QGst::Element>(m_videoSink);
    g_object_set (sink, "force-aspect-ratio", true, nullptr);
    g_object_set (sink, "sync", false, nullptr);
    g_object_set (sink, "async", false, nullptr);
    GstElement *capsfilter = gst_bin_get_by_name(GST_BIN(vid_pipeline), "mycapsfilter");
    gst_bin_add(GST_BIN(vid_pipeline), GST_ELEMENT(sink));
    gst_element_link(capsfilter, GST_ELEMENT(sink));

    vid_src = GST_APP_SRC(gst_bin_get_by_name(GST_BIN(vid_pipeline), "mysrc"));
    gst_app_src_set_stream_type(vid_src, GST_APP_STREAM_TYPE_STREAM);

    /*
     * Initialize Music pipeline
     */

    aud_pipeline = gst_parse_launch("appsrc name=audsrc is-live=true block=false max-latency=100000 do-timestamp=true ! "
                                    "audio/x-raw, signed=true, endianness=1234, depth=16, width=16, rate=48000, channels=2, format=S16LE ! "
                                    "pulsesink buffer-time=400000 sync=false client-name=\"Android Auto Music\""
                                    , &error);

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
                                    "pulsesink buffer-time=400000 sync=false client-name=\"Android Auto Voice\"", &error);

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

    mic_pipeline = gst_parse_launch("pulsesrc name=micsrc client-name=\"Android Auto Voice\" ! audioconvert ! "
                                    "audio/x-raw, signed=true, endianness=1234, depth=16, width=16, channels=1, rate=16000 ! "
                                    "queue ! "
                                    "appsink name=micsink emit-signals=true async=false blocksize=8192", &error);

    if (error != NULL) {
        qDebug("could not construct pipeline: %s", error->message);
        g_clear_error(&error);
        return -1;
    }

    mic_sink = gst_bin_get_by_name(GST_BIN(mic_pipeline), "micsink");

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
gboolean Headunit::bus_callback(GstBus */* unused*/, GstMessage *message, gpointer *ptr) {
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
    default:
        break;
    }

    return TRUE;
}
void Headunit::setGstState(QString state){

    GstState gst_state;
    if(state == "play"){
        gst_state = GST_STATE_PLAYING;
        huStarted = true;
        GstElement *capsfilter = gst_bin_get_by_name(GST_BIN(vid_pipeline), "mycapsfilter");
        GstPad *convert_pad = gst_element_get_static_pad(capsfilter, "sink");
        gst_pad_add_probe (convert_pad,GST_PAD_PROBE_TYPE_EVENT_DOWNSTREAM,convert_probe, this, NULL);

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

bool Headunit::keyEvent(QString /*key*/){
    return true;
}

void Headunit::touchEvent(HU::TouchInfo::TOUCH_ACTION action, QPoint *point) {
    float normx = float(point->x()) / float(m_outputWidth);
    float normy = float(point->y()) / float(m_outputHeight);

    unsigned int x = (unsigned int) (normx * m_videoWidth);
    unsigned int y = (unsigned int) (normy * m_videoHeight);

    if(huStarted){
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
                                      {"text",QString("%1 connected\nStarting Android Auto").arg(dev)}}.toVariantMap());
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
    //emit deviceConnected(QJsonObject {{"image","icons/eject.png"},{"title","USB device removed"},{"text",""}}.toVariantMap());
}
void Headunit::setOutputWidth(const int a){
    if(a != m_outputWidth){
        m_outputWidth = a;
        emit outputResized();
    }
}
void Headunit::setOutputHeight(const int a){
    if(a != m_outputHeight){
        m_outputHeight = a;
        emit outputResized();
    }
}

void Headunit::setVideoWidth(const int a){
    m_videoWidth = a;
    emit videoResized();
}

void Headunit::setVideoHeight(const int a){
    m_videoHeight = a;
    emit videoResized();
}

int Headunit::outputWidth() {
    return m_outputWidth;
}
int Headunit::outputHeight() {
    return m_outputHeight;
}
int Headunit::videoWidth() {
    return m_videoWidth;
}
int Headunit::videoHeight() {
    return m_videoHeight;
}
int DesktopEventCallbacks::MediaPacket(int chan, uint64_t /* unused */, const byte * buf, int len) {
    GstAppSrc* gst_src = nullptr;
    //GstElement* gst_pipe = nullptr;
    if (chan == AA_CH_VID) {
        gst_src = headunit->vid_src;
        //gst_pipe = headunit->vid_pipeline;
    } else if (chan == AA_CH_AUD) {
        gst_src = headunit->aud_src;
        //gst_pipe = headunit->aud_pipeline;
    } else if (chan == AA_CH_AU1) {
        gst_src = headunit->au1_src;
        //gst_pipe = headunit->au1_pipeline;
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

    HU::SensorEvent sensorEvent;
    sensorEvent.add_location_data()->set_speed(0);
    headunit->g_hu->hu_queue_command([sensorEvent](IHUConnectionThreadInterface& s)
    {
        s.hu_aap_enc_send_message(0, AA_CH_SEN, HU_SENSOR_CHANNEL_MESSAGE::SensorEvent, sensorEvent);
    });

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

void DesktopEventCallbacks::CustomizeOutputChannel(int /* unused */, HU::ChannelDescriptor::OutputStreamChannel& /* unused */) {

}
void DesktopEventCallbacks::MediaSetupComplete(int chan) {
    if (chan == AA_CH_VID) {
        VideoFocusHappened(true, true);
    }
}
void DesktopEventCallbacks::AudioFocusRequest(int chan, const HU::AudioFocusRequest &request)  {
    run_on_main_thread([this, chan, request](){
        HU::AudioFocusResponse response;
        if (request.focus_type() == HU::AudioFocusRequest::AUDIO_FOCUS_RELEASE) {
            //audioOutput.reset();
            response.set_focus_type(HU::AudioFocusResponse::AUDIO_FOCUS_STATE_LOSS);
            audioFocus = false;
        } else {
            /*if (!audioOutput) {
                audioOutput.reset(new AudioOutput());
            }*/
            response.set_focus_type(HU::AudioFocusResponse::AUDIO_FOCUS_STATE_GAIN);
            audioFocus = true;
        }

        headunit->g_hu->hu_queue_command([chan, response](IHUConnectionThreadInterface & s) {
            s.hu_aap_enc_send_message(0, chan, HU_PROTOCOL_MESSAGE::AudioFocusResponse, response);
        });
        return false;
    });
}

void DesktopEventCallbacks::VideoFocusRequest(int /* unused */, const HU::VideoFocusRequest &request) {
    VideoFocusHappened(request.mode() == HU::VIDEO_FOCUS_MODE_FOCUSED, false);
}

void DesktopEventCallbacks::VideoFocusHappened(bool hasFocus, bool unrequested) {
    run_on_main_thread([this, hasFocus, unrequested](){
        /*if ((bool)videoOutput != hasFocus) {
            videoOutput.reset(hasFocus ? new VideoOutput(this) : nullptr);
        }*/
        videoFocus = hasFocus;
        headunit->g_hu->hu_queue_command([hasFocus, unrequested](IHUConnectionThreadInterface & s) {
            HU::VideoFocus videoFocusGained;
            videoFocusGained.set_mode(hasFocus ? HU::VIDEO_FOCUS_MODE_FOCUSED : HU::VIDEO_FOCUS_MODE_UNFOCUSED);
            videoFocusGained.set_unrequested(unrequested);
            s.hu_aap_enc_send_message(0, AA_CH_VID, HU_MEDIA_CHANNEL_MESSAGE::VideoFocus, videoFocusGained);
        });
        return false;
    });
}
std::string DesktopEventCallbacks::GetCarBluetoothAddress(){
    QList<QBluetoothHostInfo> localAdapters = QBluetoothLocalDevice::allDevices();
    if(localAdapters.size() > 0){
        return localAdapters.at(0).address().toString().toStdString();
    }
    return std::string();
}
void DesktopEventCallbacks::PhoneBluetoothReceived(std::string address){
    emit headunit->btConnectionRequest(QString::fromStdString(address));
}
