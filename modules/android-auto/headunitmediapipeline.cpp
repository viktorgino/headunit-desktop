#include "headunitmediapipeline.h"

#include "qgstvideobuffer.h"
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSettings>

HeadunitMediaPipeline::HeadunitMediaPipeline(QObject* parent)
    : QObject { parent }
{
}

HeadunitMediaPipeline::~HeadunitMediaPipeline()
{
    stopPipelines();

    gst_object_unref(vid_pipeline);
    gst_object_unref(mic_pipeline);
    gst_object_unref(aud_pipeline);
    gst_object_unref(au1_pipeline);

    gst_object_unref(m_vid_src);
    gst_object_unref(m_aud_src);
    gst_object_unref(m_au1_src);
}

void HeadunitMediaPipeline::setMicrophoneDataHandler(HeadunitMicrophoneDataHandlerInterface* microphoneDataHandler)
{
    m_microphoneDataHandler = microphoneDataHandler;
}

int HeadunitMediaPipeline::init()
{
    GstBus* bus;
    GError* error = NULL;

    /*
     * Initialize Video pipeline
     */

    const char* vid_launch_str = "appsrc name=vid_src is-live=true block=false min-latency=0 max-latency=-1 do-timestamp=false format=3 ! "
                                 "h264parse ! "
#ifdef RPI
                                 "v4l2h264dec ! "
#else
                                 "avdec_h264 ! "
#endif
                                 "appsink caps=\"video/x-raw,format=I420\" emit-signals=true sync=false name=vid_sink";

    vid_pipeline = gst_parse_launch(vid_launch_str, &error);

    bus = gst_pipeline_get_bus(GST_PIPELINE(vid_pipeline));
    gst_bus_add_watch(bus, (GstBusFunc)HeadunitMediaPipeline::bus_callback, this);
    gst_object_unref(bus);

    GstElement* vid_sink = gst_bin_get_by_name(GST_BIN(vid_pipeline), "vid_sink");

    g_signal_connect(vid_sink, "new-sample", G_CALLBACK(&HeadunitMediaPipeline::newVideoSample), this);

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
                                    "alsasink buffer-time=400000 sync=false"
#else
                                    "pulsesink name=mediasink sync=true client-name=\"Android Auto Music\" "
                                    "stream-properties=\"props,media.name=AndroidAutoMusic,media.role=music\""
#endif
        ,
        &error);
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
                                    "alsasink buffer-time=400000 sync=false"
#else
                                    "volume volume=0.5 !"
                                    "pulsesink name=voicesink sync=true client-name=\"Android Auto Voice\""
                                    "stream-properties=\"props,media.name=AndroidAutoVoice,media.role=music\""
#endif
        ,
        &error);

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
        "alsasrc name=micsrc ! audioconvert ! "
#else
        "pulsesrc name=micsrc client-name=\"Android Auto Voice\" ! audioconvert ! "
#endif
        "queue ! "
        "appsink name=micsink emit-signals=true async=false caps=\"audio/x-raw, signed=true, endianness=1234, depth=16, width=16, channels=1, rate=16000\" blocksize=8192",
        &error);

    if (error != NULL) {
        qDebug("Could not construct mic pipeline: %s", error->message);
        g_clear_error(&error);
        return -1;
    }

    GstElement* mic_sink = gst_bin_get_by_name(GST_BIN(mic_pipeline), "micsink");
    g_signal_connect(mic_sink, "new-sample", G_CALLBACK(&HeadunitMediaPipeline::newMicrophoneSample), this);
    gst_object_unref(mic_sink);

    m_vid_src = GST_APP_SRC(gst_bin_get_by_name(GST_BIN(vid_pipeline), "vid_src"));
    m_aud_src = GST_APP_SRC(gst_bin_get_by_name(GST_BIN(aud_pipeline), "audsrc"));
    m_au1_src = GST_APP_SRC(gst_bin_get_by_name(GST_BIN(au1_pipeline), "au1src"));

    gst_element_set_state(mic_pipeline, GST_STATE_PAUSED);
    gst_element_set_state(vid_pipeline, GST_STATE_PAUSED);
    gst_element_set_state(aud_pipeline, GST_STATE_PAUSED);
    gst_element_set_state(au1_pipeline, GST_STATE_PAUSED);

    return 0;
}

GstFlowReturn HeadunitMediaPipeline::newVideoSample(GstElement* appsink, HeadunitMediaPipeline* _this)
{

    GstSample* gstsample = gst_app_sink_pull_sample(GST_APP_SINK(appsink));
    if (!gstsample) {
        return GST_FLOW_ERROR;
    }

    GstBuffer* gstbuf = gst_sample_get_buffer(gstsample);
    if (!gstbuf) {
        gst_sample_unref(gstsample);
        return GST_FLOW_ERROR;
    }

    GstMapInfo mapInfo;
    gst_buffer_map(gstbuf, &mapInfo, GST_MAP_READ);

    GstCaps* caps = gst_sample_get_caps(gstsample);
    GstVideoInfo info;
    gst_video_info_from_caps(&info, caps);

    QGstVideoBuffer* qgstBuf = new QGstVideoBuffer(gstbuf, info);
    QVideoFrame frame(static_cast<QAbstractVideoBuffer*>(qgstBuf), QSize(info.width, info.height), QVideoFrame::Format_YUV420P);

    emit _this->receivedVideoFrame(frame);

    gst_buffer_unmap(gstbuf, const_cast<GstMapInfo*>(&mapInfo));
    gst_sample_unref(gstsample);
    return GST_FLOW_OK;
}

void HeadunitMediaPipeline::handleMicrophoneData(const uint64_t timestamp, const unsigned char* bufferData, const int bufferSize)
{
    if (m_microphoneDataHandler == nullptr) {
        qWarning() << "microphone data handler not set!";
    }

    m_microphoneDataHandler->handleMicrophoneData(timestamp, bufferData, bufferSize);
}

GstFlowReturn HeadunitMediaPipeline::newMicrophoneSample(GstElement* appsink, HeadunitMediaPipeline* _this)
{
    GstSample* gstsample;
    GstBuffer* gstbuf;

    gstsample = gst_app_sink_pull_sample(GST_APP_SINK(appsink));

    if (!gstsample) {
        return GST_FLOW_ERROR;
    }

    gstbuf = gst_sample_get_buffer(gstsample);
    if (!gstbuf) {
        gst_sample_unref(gstsample);
        return GST_FLOW_ERROR;
    }

    GstMapInfo mapInfo;
    gst_buffer_map(gstbuf, &mapInfo, GST_MAP_READ);

    uint64_t bufTimestamp = GST_BUFFER_TIMESTAMP(gstbuf);
    uint64_t timestamp = GST_CLOCK_TIME_IS_VALID(bufTimestamp) ? (bufTimestamp / 1000) : get_cur_timestamp();

    _this->handleMicrophoneData(timestamp, mapInfo.data, mapInfo.size);

    gst_buffer_unmap(gstbuf, const_cast<GstMapInfo*>(&mapInfo));

    gst_sample_unref(gstsample);
    return GST_FLOW_OK;
}

gboolean HeadunitMediaPipeline::bus_callback(GstBus* /* unused*/, GstMessage* message, gpointer* ptr)
{
    HeadunitMediaPipeline* hu = (HeadunitMediaPipeline*)ptr;
    gchar* debug;
    GError* err;
    gchar* name;

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

        name = (gchar*)GST_MESSAGE_SRC_NAME(message);

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

void HeadunitMediaPipeline::stopPipelines()
{
    gst_element_set_state(vid_pipeline, GST_STATE_NULL);
    gst_element_set_state(aud_pipeline, GST_STATE_NULL);
    gst_element_set_state(au1_pipeline, GST_STATE_NULL);
};

void HeadunitMediaPipeline::mediaStart(const Headunit::Pipeline pipeline)
{
    switch (pipeline) {
    case Headunit::VideoPipeline:
        gst_element_set_state(vid_pipeline, GST_STATE_PLAYING);
        break;
    case Headunit::AudioPipeline:
        gst_element_set_state(aud_pipeline, GST_STATE_PLAYING);
        break;
    case Headunit::VoicePipeline:
        gst_element_set_state(au1_pipeline, GST_STATE_PLAYING);
        break;
    case Headunit::MicrophonePipeline:
        gst_element_set_state(mic_pipeline, GST_STATE_PLAYING);
        break;
    default:
        qDebug() << "Pipeline Start Unknown chan : " << pipeline;
    }

    emit pipelineStatusChanged(pipeline, Headunit::PipelinePlaying);
}

void HeadunitMediaPipeline::mediaStop(const Headunit::Pipeline pipeline)
{
    switch (pipeline) {
    case Headunit::VideoPipeline:
        gst_element_set_state(vid_pipeline, GST_STATE_PAUSED);
        break;
    case Headunit::AudioPipeline:
        gst_element_set_state(aud_pipeline, GST_STATE_PAUSED);
        break;
    case Headunit::VoicePipeline:
        gst_element_set_state(au1_pipeline, GST_STATE_PAUSED);
        break;
    case Headunit::MicrophonePipeline:
        gst_element_set_state(mic_pipeline, GST_STATE_PAUSED);
        break;
    default:
        qDebug() << "Pipeline Stop Unknown chan : " << pipeline;
    }

    emit pipelineStatusChanged(pipeline, Headunit::PipelineStopped);
}

void HeadunitMediaPipeline::handleMediaData(const Headunit::Pipeline pipeline, uint64_t timestamp, const unsigned char* bufferData, int bufferSize)
{
    GstAppSrc* gst_src = nullptr;

    switch (pipeline) {
    case Headunit::VideoPipeline:
        gst_src = m_vid_src;
        break;
    case Headunit::AudioPipeline:
        gst_src = m_aud_src;
        break;
    case Headunit::VoicePipeline:
        gst_src = m_au1_src;
        break;
    default:
        qWarning() << "Pipeline channel : " << pipeline;
        return;
    }

    if (gst_src) {
        GstBuffer* buffer = gst_buffer_new_and_alloc(bufferSize);
        GstCaps* reference = gst_caps_from_string("timestamp/x-metavision-stream");

        gst_buffer_add_reference_timestamp_meta(buffer, reference, timestamp * 1000, GST_CLOCK_TIME_NONE);
        gst_buffer_fill(buffer, 0, bufferData, bufferSize);

        int ret = gst_app_src_push_buffer((GstAppSrc*)gst_src, buffer);
        if (ret != GST_FLOW_OK) {
            int len = gst_buffer_get_size(buffer);
            qWarning("Error when pushing buffer %d for %d bytes ", ret, len);
        }
    }
}

void HeadunitMediaPipeline::setMediaVolume(uint8_t volume)
{
    if (aud_pipeline) {
        GstElement* media_sink = gst_bin_get_by_name(GST_BIN(aud_pipeline), "mediasink");
        g_object_set(media_sink, "volume", volume / 100.00, NULL);
        gst_object_unref(media_sink);
    }
}

void HeadunitMediaPipeline::setVoiceVolume(uint8_t volume)
{
    if (au1_pipeline) {
        GstElement* voice_sink = gst_bin_get_by_name(GST_BIN(au1_pipeline), "voicesink");
        g_object_set(voice_sink, "volume", volume / 100.00, NULL);
        gst_object_unref(voice_sink);
    }
}
