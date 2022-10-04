#include "reversing-camera.h"


struct VideoFormat
{
    QVideoFrame::PixelFormat pixelFormat;
    GstVideoFormat gstFormat;
};

static const VideoFormat qt_videoFormatLookup[] =
{
    { QVideoFrame::Format_YUV420P, GST_VIDEO_FORMAT_I420 },
    { QVideoFrame::Format_YUV422P, GST_VIDEO_FORMAT_Y42B },
    { QVideoFrame::Format_YV12   , GST_VIDEO_FORMAT_YV12 },
    { QVideoFrame::Format_UYVY   , GST_VIDEO_FORMAT_UYVY },
    { QVideoFrame::Format_YUYV   , GST_VIDEO_FORMAT_YUY2 },
    { QVideoFrame::Format_NV12   , GST_VIDEO_FORMAT_NV12 },
    { QVideoFrame::Format_NV21   , GST_VIDEO_FORMAT_NV21 },
    { QVideoFrame::Format_AYUV444   , GST_VIDEO_FORMAT_AYUV },
    { QVideoFrame::Format_Y8     , GST_VIDEO_FORMAT_GRAY8 },
#if Q_BYTE_ORDER == Q_LITTLE_ENDIAN
    { QVideoFrame::Format_Y16 , GST_VIDEO_FORMAT_GRAY16_LE },
#else
    { QVideoFrame::Format_Y16 , GST_VIDEO_FORMAT_GRAY16_BE },
    { QVideoFrame::Format_P010 , GST_VIDEO_FORMAT_P010_10BE },
#endif
};

static int indexOfVideoFormat(GstVideoFormat format)
{
    for (int i = 0; i < sizeof(qt_videoFormatLookup); ++i)
        if (qt_videoFormatLookup[i].gstFormat == format)
            return i;

    return -1;
}

ReversingCamera::ReversingCamera(QObject *parent) : QObject (parent)
{
    connect(this, &ReversingCamera::receivedVideoFrame, this, &ReversingCamera::videoFrameHandler);
}

void ReversingCamera::init() {
    if (!gst_is_initialized ()){
        gst_init(NULL, NULL);
    }
    setupPipeline();
}

void ReversingCamera::setupPipeline() {
    if(m_vid_pipeline){
        gst_element_set_state(m_vid_pipeline, GST_STATE_NULL);
        gst_object_unref(m_vid_pipeline);
        m_vid_pipeline = nullptr;
    }
    GError *error = NULL;    
    QString pipelineSource;

    if(m_settings["camera"] == "auto"){
        pipelineSource = "v4l2src ! videoconvert";
    } else if(m_settings["camera"] == "rpi"){
        pipelineSource = "v4l2src ! video/x-raw,format=UYVY,colorimetry=bt601";
    } else if(m_settings["camera"] == "custom"){
        pipelineSource = m_settings["custom_pipeline"].toString();
    } 

    QString pipeline = QString("%1 ! appsink emit-signals=true sync=false name=vid_sink").arg(pipelineSource);
    qDebug() << pipeline;
    m_vid_pipeline = gst_parse_launch(pipeline.toLocal8Bit().data(), &error);

    GstElement *vid_sink = gst_bin_get_by_name(GST_BIN(m_vid_pipeline), "vid_sink");

    g_signal_connect(vid_sink, "new-sample", G_CALLBACK(&ReversingCamera::newVideoSample), this);

    if (error != NULL) {
        qDebug("Could not construct video pipeline: %s", error->message);
        g_clear_error(&error);
    }

    gst_element_set_state(m_vid_pipeline, GST_STATE_PLAYING);
}

void ReversingCamera::onSettingsPageDestroyed(){
    setupPipeline();
}

QObject *ReversingCamera::getContextProperty(){
    return this;
}

void ReversingCamera::setVideoSurface(QAbstractVideoSurface *surface)
{
    qDebug() << "Setting video surface";
    if (m_surface != surface && m_surface && m_surface->isActive()) {
        m_surface->stop();
    }
    m_surface = surface;
}

void ReversingCamera::videoFrameHandler(const QVideoFrame &frame)
{
    if (m_surface != nullptr) {
        if(!m_videoStarted){
            m_videoStarted = true;
            QVideoSurfaceFormat format(frame.size(), frame.pixelFormat());
            m_surface->start(format);
            qDebug() << frame.pixelFormat();
        }
        m_surface->present(frame);
    }
}

GstFlowReturn ReversingCamera::newVideoSample (GstElement * appsink, ReversingCamera * _this){
    GstSample *gstsample = gst_app_sink_pull_sample(GST_APP_SINK(appsink));
    if (!gstsample) {
        return GST_FLOW_ERROR;
    }

    GstBuffer *gstbuf = gst_sample_get_buffer(gstsample);
    if(!gstbuf){
        gst_sample_unref(gstsample);
        return GST_FLOW_ERROR;
    }

    GstCaps* caps = gst_sample_get_caps (gstsample);
    GstVideoInfo info;
    gst_video_info_from_caps (&info, caps);

    int index = indexOfVideoFormat(info.finfo->format);

    QGstVideoBuffer *qgstBuf = new QGstVideoBuffer(gstbuf,info);
    QVideoFrame frame(static_cast<QAbstractVideoBuffer *>(qgstBuf), QSize(info.width,info.height), qt_videoFormatLookup[index].pixelFormat);

    emit _this->receivedVideoFrame(frame);

    gst_sample_unref(gstsample);
    return GST_FLOW_OK;
}
