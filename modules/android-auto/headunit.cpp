#include "headunit.h"

#include <QDebug>
#include <QJsonObject>
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

Headunit::Headunit():
    callbacks(this)
{
    initGst();
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

    qDebug() << "Starting headunit";
    if(settings.childGroups().contains("AndroidAuto")){
        settings.beginGroup("AndroidAuto");
        QStringList keys = settings.childKeys();

        for (int i = 0; i < keys.size(); ++i){
            aa_settings[keys.at(i).toStdString()] = settings.value(keys.at(i)).toString().toStdString();
        }
    }

    if(settings.childKeys().contains("resolution")){
        QVariant res = settings.value("resolution");
        if(res.canConvert(QMetaType::Int)){
            switch(res.toInt()){
            case 1:
                setVideoWidth(800);
                setVideoHeight(480);
                break;
            case 2:
                setVideoWidth(1280);
                setVideoHeight(720);
                break;
            case 3:
                setVideoWidth(1920);
                setVideoHeight(1080);
                break;
            }
        }
    }
    aa_settings["ts_height"] = std::to_string(m_videoHeight);
    aa_settings["ts_width"] = std::to_string(m_videoWidth);

    headunit = new HUServer(callbacks, aa_settings);

    int ret = headunit->hu_aap_start(false, false);
    if ( ret >= 0) {
        g_hu = &headunit->GetAnyThreadInterface();
        return 1;
    } else {
        stopHU();
        return 0;
    }
}


void Headunit::setVideoItem(QQuickItem * videoItem){
    m_videoItem = videoItem;

    g_object_set (m_videoSink, "widget", m_videoItem, NULL);

    startHU();
}
int Headunit::stopHU(){
    setGstState("");
    if(headunit){
        qDebug() << "Stopping headunit";
        //headunit->hu_aap_shutdown();
        //delete(headunit);
        //headunit = NULL;
    }
    return 1;
}
int Headunit::restartHU(){
    stopHU();
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

    const char* vid_launch_str = "appsrc name=vid_src is-live=true block=false min-latency=0 max-latency=15000000 do-timestamp=true format=3 ! "
                                 "h264parse ! "
        #ifdef RPI
                                 "omxh264dec ! "
        #else
                                 "avdec_h264 ! "
        #endif
                                 "glsinkbin name=vid_glsinkbin ";
    vid_pipeline = gst_parse_launch(vid_launch_str, &error);

    bus = gst_pipeline_get_bus(GST_PIPELINE(vid_pipeline));
    gst_bus_add_watch(bus, (GstBusFunc) Headunit::bus_callback, this);
    gst_object_unref(bus);

    m_videoSink = gst_element_factory_make ("qmlglsink", NULL);

    GstElement *glsinkbin = gst_bin_get_by_name(GST_BIN(vid_pipeline), "vid_glsinkbin");

    g_object_set (glsinkbin, "sink", m_videoSink, NULL);

    vid_src = GST_APP_SRC(gst_bin_get_by_name(GST_BIN(vid_pipeline), "vid_src"));
    gst_app_src_set_stream_type(vid_src, GST_APP_STREAM_TYPE_STREAM);

    /*
     * Initialize Music pipeline
     */

    aud_pipeline = gst_parse_launch("appsrc name=audsrc is-live=true block=false min-latency=0 max-latency=-1 do-timestamp=true format=3 ! "
                                    "audio/x-raw, signed=true, endianness=1234, depth=16, width=16, rate=48000, channels=2, format=S16LE ! "
                                    "queue min-threshold-buffers=1024 flush-on-eos=true ! "
                                #ifdef RPI
                                    "alsasink buffer-time=400000 sync=false device-name=\"Android Auto Music\""
                                #else
                                    "pulsesink buffer-time=400000 sync=false client-name=\"Android Auto Music\""
                                #endif
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

    au1_pipeline = gst_parse_launch("appsrc name=au1src is-live=true block=false min-latency=0 max-latency=-1 do-timestamp=true  format=3 ! "
                                    "audio/x-raw, signed=true, endianness=1234, depth=16, width=16, rate=16000, channels=1, format=S16LE  !"
                                    "queue min-threshold-buffers=1024 ! "
                                #ifdef RPI
                                    "alsasink buffer-time=400000 sync=false device-name=\"Android Auto Voice\""
                                #else
                                    "pulsesink buffer-time=400000 sync=false client-name=\"Android Auto Voice\""
                                #endif
                                    , &error);

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

    mic_pipeline = gst_parse_launch(
            #ifdef RPI
                "alsasrc name=micsrc device-name=\"Android Auto Voice\" ! audioconvert ! "
            #else
                "pulsesrc name=micsrc client-name=\"Android Auto Voice\" ! audioconvert ! "
            #endif
                "queue ! "
                "appsink name=micsink emit-signals=true async=false caps=\"audio/x-raw, signed=true, endianness=1234, depth=16, width=16, channels=1, rate=16000\" blocksize=8192", &error);

    if (error != NULL) {
        qDebug("could not construct pipeline: %s", error->message);
        g_clear_error(&error);
        return -1;
    }

    mic_sink = gst_bin_get_by_name(GST_BIN(mic_pipeline), "micsink");

    g_signal_connect(mic_sink, "new-sample", G_CALLBACK(&Headunit::read_mic_data), this);

    gst_element_set_state(mic_pipeline, GST_STATE_READY);

    return 0;
}

GstFlowReturn Headunit::read_mic_data(GstElement *appsink, Headunit *_this) {
    GstSample *gstsample;
    GstBuffer *gstbuf;

    gstsample = gst_app_sink_pull_sample(GST_APP_SINK(appsink));

    if (!gstsample) {
        return GST_FLOW_ERROR;
    }

    gstbuf = gst_sample_get_buffer(gstsample);
    if(!gstbuf){
        gst_sample_unref(gstsample);
        return GST_FLOW_ERROR;
    }

    GstMapInfo mapInfo;
    gst_buffer_map(gstbuf, &mapInfo, GST_MAP_READ);

    uint64_t bufTimestamp = GST_BUFFER_TIMESTAMP(gstbuf);
    uint64_t timestamp = GST_CLOCK_TIME_IS_VALID(bufTimestamp) ? (bufTimestamp / 1000) : get_cur_timestamp();

    _this->g_hu->hu_queue_command([timestamp, mapInfo](IHUConnectionThreadInterface & s) {
        int ret = s.hu_aap_enc_send_media_packet(1, AA_CH_MIC, HU_PROTOCOL_MESSAGE::MediaDataWithTimestamp, timestamp, mapInfo.data, mapInfo.size);
        if (ret < 0) {
            qDebug("read_mic_data(): hu_aap_enc_send() failed with (%d)", ret);
        }
    });

    gst_buffer_unmap(gstbuf, const_cast<GstMapInfo*> (&mapInfo));

    gst_sample_unref(gstsample);
    return GST_FLOW_OK;
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
        GstElement *glsinkbin = gst_bin_get_by_name(GST_BIN(vid_pipeline), "vid_glsinkbin");
        GstPad *convert_pad = gst_element_get_static_pad(glsinkbin, "sink");
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
    switch(chan){
    case AA_CH_VID:
        gst_element_set_state(headunit->vid_pipeline, GST_STATE_PLAYING);
        headunit->huStarted = true;
        break;
    case AA_CH_AUD:
        gst_element_set_state(headunit->aud_pipeline, GST_STATE_PLAYING);
        break;
    case AA_CH_AU1:
        gst_element_set_state(headunit->au1_pipeline, GST_STATE_PLAYING);
        break;
    case AA_CH_MIC:
        gst_element_set_state(headunit->mic_pipeline, GST_STATE_PLAYING);
        break;
    default:
        qDebug() << "Media Start Unknown chan : " << chan;
    }

    return 0;
}

int DesktopEventCallbacks::MediaStop(int chan) {
    switch(chan){
    case AA_CH_VID:
        gst_element_set_state(headunit->vid_pipeline, GST_STATE_READY);
        headunit->huStarted = false;
        break;
    case AA_CH_AUD:
        gst_element_set_state(headunit->aud_pipeline, GST_STATE_READY);
        break;
    case AA_CH_AU1:
        gst_element_set_state(headunit->au1_pipeline, GST_STATE_READY);
        break;
    case AA_CH_MIC:
        gst_element_set_state(headunit->mic_pipeline, GST_STATE_READY);
        break;
    default:
        qDebug() << "Media Stop Unknown chan : " << chan;
    }
    return 0;
}

void DesktopEventCallbacks::DisconnectionOrError() {
    qDebug("Android Device disconnected, pausing gstreamer");
    headunit->stopHU();
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
            response.set_focus_type(HU::AudioFocusResponse::AUDIO_FOCUS_STATE_LOSS);
        } else {
            response.set_focus_type(HU::AudioFocusResponse::AUDIO_FOCUS_STATE_GAIN);
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
        headunit->g_hu->hu_queue_command([hasFocus, unrequested](IHUConnectionThreadInterface & s) {
            HU::VideoFocus videoFocusGained;
            videoFocusGained.set_mode(hasFocus ? HU::VIDEO_FOCUS_MODE_FOCUSED : HU::VIDEO_FOCUS_MODE_UNFOCUSED);
            videoFocusGained.set_unrequested(unrequested);
            s.hu_aap_enc_send_message(0, AA_CH_VID, HU_MEDIA_CHANNEL_MESSAGE::VideoFocus, videoFocusGained);
        });

        //Set speed to 0
        HU::SensorEvent sensorEvent;
        sensorEvent.add_location_data()->set_speed(0);
        headunit->g_hu->hu_queue_command([sensorEvent](IHUConnectionThreadInterface& s)
        {
            s.hu_aap_enc_send_message(0, AA_CH_SEN, HU_SENSOR_CHANNEL_MESSAGE::SensorEvent, sensorEvent);
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
