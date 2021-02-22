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

Headunit::Headunit(QObject *parent) : QObject(parent),
    callbacks(this)
#ifdef QTGSTREAMER
   , m_videoSurface(this)
#endif
{
#ifdef BUILD_GSTQT
    qmlRegisterType<QtGLVideoItem> ("org.freedesktop.gstreamer.GLVideoItem", 1, 0, "GstGLVideoItem");
#endif
#ifdef BUILD_QTGSTREAMER
    qmlRegisterType<QGst::Quick::VideoItem>("QtGStreamer", 1, 0, "VideoItem");
    qmlRegisterUncreatableType<QGst::Quick::VideoSurface>("QtGStreamer", 1, 0, "VideoSurface",
        QLatin1String("Creating a QGst::Quick::VideoSurface from QML is not supported"));
#endif

    connect(this, &Headunit::receivedVideoFrame, this, &Headunit::videoFrameHandler);
}

Headunit::~Headunit() {
    stopPipelines();

    gst_object_unref(vid_pipeline);
    gst_object_unref(mic_pipeline);
    gst_object_unref(aud_pipeline);
    gst_object_unref(au1_pipeline);


    gst_object_unref(m_vid_src);
    gst_object_unref(m_aud_src);
    gst_object_unref(m_au1_src);

    if(headunit){
        delete(headunit);
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
        huStarted = true;
        setStatus(Headunit::VIDEO_WAITING);
    }
    return 1;
}

#if defined(BUILD_QTGSTREAMER) || defined(BUILD_GSTQT)
GST_DEBUG_CATEGORY_STATIC (hu_aa);
#define GST_CAT_DEFAULT hu_aa
static gboolean gstreamer_plugin_init(GstPlugin *plugin)
{
    GST_DEBUG_CATEGORY_INIT(hu_aa, "hu_aa", 0, "Debug category for hu_aa qml elements");
    bool registered = false;
#if defined(BUILD_QTGSTREAMER)
    qDebug() << "Registering qtquick2videosink plugin";
    registered = gst_element_register(plugin, "qtquick2videosink", GST_RANK_NONE, GST_TYPE_QT_QUICK2_VIDEO_SINK);
#elif defined(BUILD_GSTQT)
    qDebug() << "Registering qmlglsink plugin";
    registered = gst_element_register(plugin, "qmlglsink", GST_RANK_NONE, GST_TYPE_QT_SINK);
#endif
    if (registered) {
        qDebug() << "Registered gstreamer plugin";
    } else {
        qWarning() << "Failed to register gstreamer plugin";
        return false;
    }

    return true;
}

#define PACKAGE "headunitDesktop"

GST_PLUGIN_DEFINE (
        GST_VERSION_MAJOR,
        GST_VERSION_MINOR,
#ifdef BUILD_QTGSTREAMER
        qt5videosink,
#elif defined(BUILD_GSTQT)
        qmlgl,
#endif
        "QML GStreamer sink",
        gstreamer_plugin_init,
        "1.0",
        "LGPL",
        "Qt5GStreamer",
        "http://gstreamer.freedesktop.org/")

#ifdef BUILD_QTGSTREAMER
    GST_PLUGIN_STATIC_DECLARE(qt5videosink);
#elif defined(BUILD_GSTQT)
    GST_PLUGIN_STATIC_DECLARE(qmlgl);
#endif

#endif
#if defined(GSTQT)
void Headunit::setVideoItem(QQuickItem * videoItem){

    GstElement *vid_sink = gst_element_factory_make ("qmlglsink", "vid_qmlsink");
    g_object_set (vid_sink, "widget", videoItem, NULL);


    GstElement *glsinkbin = gst_bin_get_by_name(GST_BIN(vid_pipeline), "vid_glsinkbin");
    g_object_set (glsinkbin, "sink", vid_sink, NULL);

    gst_object_unref(vid_sink);
    gst_object_unref(glsinkbin);

    startHU();
}
#endif

int Headunit::init(){

#ifdef BUILD_QTGSTREAMER
    GST_PLUGIN_STATIC_REGISTER (qt5videosink);
#elif defined(BUILD_GSTQT)
    GST_PLUGIN_STATIC_REGISTER (qmlgl);
#endif

    GstBus *bus;
    GError *error = NULL;    


    /*
     * Initialize Video pipeline
     */

    const char* vid_launch_str = "appsrc name=vid_src is-live=true block=false min-latency=0 max-latency=-1 do-timestamp=false format=3 ! "
                                 "h264parse ! "
        #ifdef RPI
                                 "omxh264dec ! "
        #else
                                 "avdec_h264 ! "
        #endif
#if defined(GSTQT)
                                 "glsinkbin sync=true name=vid_glsinkbin ";
#elif defined(QTGSTREAMER)
            "capsfilter caps=video/x-raw name=mycapsfilter";
#else
//                "glsinkbin sync=true name=vid_glsinkbin ";
//                "videoconvert !"
                "appsink emit-signals=true sync=false name=vid_sink";

#endif
    vid_pipeline = gst_parse_launch(vid_launch_str, &error);

    bus = gst_pipeline_get_bus(GST_PIPELINE(vid_pipeline));
    gst_bus_add_watch(bus, (GstBusFunc) Headunit::bus_callback, this);
    gst_object_unref(bus);


#if defined(GSTQT)
#elif defined(QTGSTREAMER)
    GstElement *vid_sink = QGlib::RefPointer<QGst::Element>(m_videoSurface.videoSink());
    GstElement *capsfilter = gst_bin_get_by_name(GST_BIN(vid_pipeline), "mycapsfilter");
    gst_bin_add(GST_BIN(vid_pipeline), GST_ELEMENT(vid_sink));
    gst_element_link(capsfilter, GST_ELEMENT(vid_sink));
    g_object_set (vid_sink, "force-aspect-ratio", true, nullptr);

    gst_object_unref(vid_sink);
    gst_object_unref(capsfilter);
#else
//    GstElement *glsinkbin = gst_bin_get_by_name(GST_BIN(vid_pipeline), "vid_glsinkbin");
    GstElement *vid_sink = gst_bin_get_by_name(GST_BIN(vid_pipeline), "vid_sink");

//    g_object_set (glsinkbin, "sink", vid_sink, NULL);
    g_signal_connect(vid_sink, "new-sample", G_CALLBACK(&Headunit::newVideoSample), this);
//    gst_object_unref(vid_sink);
//    gst_object_unref(glsinkbin);
#endif

//    GST_DEBUG_BIN_TO_DOT_FILE_WITH_TS(GST_BIN(vid_pipeline),GST_DEBUG_GRAPH_SHOW_VERBOSE, "vid_pipeline");

    if (error != NULL) {
        qDebug("Could not construct video pipeline: %s", error->message);
        g_clear_error(&error);
        return -1;
    }

    /*
     * Initialize Music pipeline
     */

    aud_pipeline = gst_parse_launch("appsrc name=audsrc is-live=true block=false min-latency=0 max-latency=-1 do-timestamp=false format=3 ! "
                                    "audio/x-raw, signed=true, endianness=1234, depth=16, width=16, rate=48000, channels=2, format=S16LE ! "
                                #ifdef RPI
                                    "alsasink buffer-time=400000 sync=false device-name=\"Android Auto Music\""
                                #else
                                    "pulsesink sync=true client-name=\"Android Auto Music\""
                                #endif
                                    , &error);
    if (error != NULL) {
        qDebug("Could not construct audio pipeline: %s", error->message);
        g_clear_error(&error);
        return -1;
    }

    /*
     * Initialize Voice pipeline
     */

    au1_pipeline = gst_parse_launch("appsrc name=au1src is-live=true block=false min-latency=0 max-latency=-1 do-timestamp=false format=3 ! "
                                    "audio/x-raw, signed=true, endianness=1234, depth=16, width=16, rate=16000, channels=1, format=S16LE  !"
                                #ifdef RPI
                                    "alsasink buffer-time=400000 sync=false device-name=\"Android Auto Voice\""
                                #else
                                    "pulsesink sync=true client-name=\"Android Auto Voice\""
                                #endif
                                    , &error);

    if (error != NULL) {
        qDebug("Could not construct voice pipeline: %s", error->message);
        g_clear_error(&error);
        return -1;
    }

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
        qDebug("Could not construct mic pipeline: %s", error->message);
        g_clear_error(&error);
        return -1;
    }

    GstElement *mic_sink = gst_bin_get_by_name(GST_BIN(mic_pipeline), "micsink");
    g_signal_connect(mic_sink, "new-sample", G_CALLBACK(&Headunit::read_mic_data), this);
    gst_object_unref(mic_sink);

    gst_element_set_state(mic_pipeline, GST_STATE_READY);


    m_vid_src = GST_APP_SRC(gst_bin_get_by_name(GST_BIN(vid_pipeline), "vid_src"));
    m_aud_src = GST_APP_SRC(gst_bin_get_by_name(GST_BIN(aud_pipeline), "audsrc"));
    m_au1_src = GST_APP_SRC(gst_bin_get_by_name(GST_BIN(au1_pipeline), "au1src"));

    return 0;
}


GstFlowReturn Headunit::newVideoSample (GstElement * appsink, Headunit * _this){

    GstSample *gstsample = gst_app_sink_pull_sample(GST_APP_SINK(appsink));
    if (!gstsample) {
        return GST_FLOW_ERROR;
    }

    GstBuffer *gstbuf = gst_sample_get_buffer(gstsample);
    if(!gstbuf){
        gst_sample_unref(gstsample);
        return GST_FLOW_ERROR;
    }

    GstMapInfo mapInfo;
    gst_buffer_map(gstbuf, &mapInfo, GST_MAP_READ);

    GstCaps* caps = gst_sample_get_caps (gstsample);
    GstVideoInfo info;
    gst_video_info_from_caps (&info, caps);
    GstStructure* structure = gst_caps_get_structure (caps, 0);


    gint width;
    gint height;
    gst_structure_get_int (structure, "width", &width);
    gst_structure_get_int (structure, "height", &height);
//    qDebug() <<width<<height<< gst_structure_get_string (structure, "format");

//    GstBuffer * newBuf = gst_buffer_copy_deep (gstbuf);
    QGstVideoBuffer *qgstBuf = new QGstVideoBuffer(gstbuf,info);
    QVideoFrame frame(static_cast<QAbstractVideoBuffer *>(qgstBuf), QSize(width,height),QVideoFrame::Format_YUV420P);

//    _this->receiveVideoFrame(frame);
    emit _this->receivedVideoFrame(frame);

    gst_buffer_unmap(gstbuf, const_cast<GstMapInfo*> (&mapInfo));
    gst_sample_unref (gstsample);
    return GST_FLOW_OK;
}

void Headunit::setVideoSurface(QAbstractVideoSurface *surface)
{
    qDebug() << "Setting video surface";
    if (m_surface != surface && m_surface && m_surface->isActive()) {
        m_surface->stop();
    }
    m_surface = surface;
//    if (m_surface)
//        m_surface->start(m_format);

    emit signalVideoSurfaceChanged();
}
void Headunit::videoFrameHandler(const QVideoFrame &frame)
{
    if (m_surface != nullptr) {
        if(!m_videoStarted){
            m_videoStarted = true;
            QVideoSurfaceFormat format(frame.size(), QVideoFrame::Format_YUV420P);
            m_surface->start(format);
        }
        m_surface->present(frame);
    }
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
        hu->stopPipelines();
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
    case GST_MESSAGE_STATE_CHANGED:
    default:
        break;
    }

    return TRUE;
}
void Headunit::stopPipelines(){
    gst_element_set_state(vid_pipeline, GST_STATE_NULL);
    gst_element_set_state(aud_pipeline, GST_STATE_NULL);
    gst_element_set_state(au1_pipeline, GST_STATE_NULL);
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

void Headunit::setVideoWidth(const int a){
    m_videoWidth = a;
    emit videoResized();
}

void Headunit::setVideoHeight(const int a){
    m_videoHeight = a;
    emit videoResized();
}

int Headunit::videoWidth() {
    return m_videoWidth;
}

int Headunit::videoHeight() {
    return m_videoHeight;
}

Headunit::hu_status Headunit::status() {
    return m_status;
}

void Headunit::setStatus(hu_status status){
    m_status = status;
    emit statusChanged();
}
//static uint64_t init_ts = 0;
int DesktopEventCallbacks::MediaPacket(int chan, uint64_t timestamp, const byte * buf, int len) {
    GstAppSrc* gst_src = nullptr;

    if (chan == AA_CH_VID) {
        gst_src = headunit->m_vid_src;
    } else if (chan == AA_CH_AUD) {
        gst_src = headunit->m_aud_src;
    } else if (chan == AA_CH_AU1) {
        gst_src = headunit->m_au1_src;
    }

    if (gst_src) {
        GstBuffer * buffer = gst_buffer_new_and_alloc(len);

        GstCaps *reference = gst_caps_from_string("timestamp/x-metavision-stream");
        gst_buffer_add_reference_timestamp_meta(buffer, reference, timestamp * 1000, GST_CLOCK_TIME_NONE);

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
        headunit->setStatus(Headunit::RUNNING);

        GST_DEBUG_BIN_TO_DOT_FILE_WITH_TS(GST_BIN(headunit->vid_pipeline),GST_DEBUG_GRAPH_SHOW_VERBOSE, "vid_pipeline");
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
        headunit->setStatus(Headunit::VIDEO_WAITING);
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
    headunit->setStatus(Headunit::NO_CONNECTION);
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
