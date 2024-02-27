#ifndef HEADUNITMEDIAPIPELINE_H
#define HEADUNITMEDIAPIPELINE_H

#include <QObject>
#include <gst/gst.h>
#include <gst/app/gstappsrc.h>
#include <gst/app/gstappsink.h>
#include <gst/video/video.h>
#include <QAbstractVideoBuffer>
#include <QAbstractVideoSurface>
#include <QVideoSurfaceFormat>

#include "headunit_common.h"

class HeadunitMediaPipeline : public QObject, public HeadunitMediaDataHandlerInterface {
    Q_OBJECT
public:
    explicit HeadunitMediaPipeline(QObject *parent = nullptr);
    ~HeadunitMediaPipeline();
    int init();
    void setMicrophoneDataHandler(HeadunitMicrophoneDataHandlerInterface* microphoneDataHandler);

    void stopPipelines();

    void setMediaVolume(uint8_t volume);
    void setVoiceVolume(uint8_t volume);

    void mediaStop(const Headunit::Pipeline pipeline) override;
    void mediaStart(const Headunit::Pipeline pipeline) override;

    void handleMediaData(Headunit::Pipeline pipeline, uint64_t timestamp, const unsigned char* bufferData, int bufferSize) override;
    void handleMicrophoneData(const uint64_t timestamp, const unsigned char* bufferData, const int bufferSize);

signals:
    void receivedVideoFrame(const QVideoFrame& frame);
    void pipelineStatusChanged(const Headunit::Pipeline& pipeline, const Headunit::PipelineStatus& status);

private:
    GstElement* mic_pipeline = nullptr;
    GstElement *aud_pipeline = nullptr;
    GstElement *au1_pipeline = nullptr;
    GstElement *vid_pipeline = nullptr;
    GstAppSrc *m_vid_src = nullptr;
    GstAppSrc *m_aud_src = nullptr;
    GstAppSrc *m_au1_src = nullptr;

    HeadunitMicrophoneDataHandlerInterface* m_microphoneDataHandler = nullptr;

    static GstFlowReturn newMicrophoneSample(GstElement* appsink, HeadunitMediaPipeline* _this);
    static int bus_callback(GstBus* bus, GstMessage* message, gpointer* ptr);

    static GstFlowReturn newVideoSample(GstElement *appsink, HeadunitMediaPipeline *_this);
    static uint64_t get_cur_timestamp();
};

#endif // HEADUNITMEDIAPIPELINE_H
