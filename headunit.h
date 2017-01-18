#ifndef HEADUNITPLAYER_H
#define HEADUNITPLAYER_H

#include <QObject>
#include <QString>
#include <QPoint>
#include <QGst/Pipeline>
#include <QGst/Message>
#include <QGst/Utils/ApplicationSource>
#include <gst/gst.h>
#include <gst/app/gstappsrc.h>

#include "hu_uti.h"
#include "hu_aap.h"

#define MOUSE_DOWN 0
#define MOUSE_UP 1
#define MOUSE_MOVE 2

class Headunit;

class DesktopEventCallbacks : public IHUConnectionThreadEventCallbacks {
private:
    Headunit * headunit;
public:
    DesktopEventCallbacks(Headunit *hu){
        headunit = hu;
    }

    virtual int MediaPacket(int chan, uint64_t timestamp, const byte * buf, int len) override;
    virtual int MediaStart(int chan) override;
    virtual int MediaStop(int chan) override;
    virtual void DisconnectionOrError() override;
    virtual void CustomizeOutputChannel(int chan, HU::ChannelDescriptor::OutputStreamChannel& streamChannel) override;
};

class Headunit : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int outputWidth READ outputWidth WRITE setOutputWidth NOTIFY outputResized)
    Q_PROPERTY(int outputHeight READ outputHeight WRITE setOutputHeight NOTIFY outputResized)
public:
    Headunit(const QGst::ElementPtr & sink);
    ~Headunit() { stop(); headunit.hu_aap_shutdown(); }
    void start();
    void stop();
    void exit();
    void setOutputWidth(const int a){
        if(a != m_outputWidth){
            m_outputWidth = a;
            emit outputResized();
        }
    }
    void setOutputHeight(const int a){
        if(a != m_outputHeight){
            m_outputHeight = a;
            emit outputResized();
        }
    }
    int outputWidth() const {
        return m_outputWidth;
    }
    int outputHeight() const {
        return m_outputHeight;
    }
    GstElement *mic_pipeline = nullptr;
    GstElement *mic_sink = nullptr;
    GstElement *aud_pipeline = nullptr;
    GstAppSrc *aud_src = nullptr;
    GstElement *au1_pipeline = nullptr;
    GstAppSrc *au1_src = nullptr;
    GstElement *vid_pipeline = nullptr;
    GstAppSrc *vid_src = nullptr;
signals:
    void outputResized();
public slots:
    bool mouseDown(QPoint point);
    bool mouseMove(QPoint point);
    bool mouseUp(QPoint point);
    bool keyEvent(QString key);
private:
    IHUAnyThreadInterface* g_hu = nullptr;
    QGst::ElementPtr m_videoSink;
    HUServer headunit;
    DesktopEventCallbacks callbacks;
    HU::TouchInfo::TOUCH_ACTION lastAction = HU::TouchInfo::TOUCH_ACTION_RELEASE;
    int videoWidth = 800;
    int videoHeight = 480;
    int m_outputWidth = 800;
    int m_outputHeight = 480;
    int initGst();
    void read_mic_data(GstElement * sink);
    static gboolean bus_callback(GstBus *bus, GstMessage *message, gpointer *ptr);
    void touchEvent(HU::TouchInfo::TOUCH_ACTION action, QPoint *point);
    static uint64_t get_cur_timestamp();
};
#endif // HEADUNITPLAYER_H
