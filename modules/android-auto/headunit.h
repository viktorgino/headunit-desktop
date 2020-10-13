#ifndef HEADUNITPLAYER_H
#define HEADUNITPLAYER_H

#include <atomic>
#include <QObject>
#include <QString>
#include <QPoint>
#include <QVariant>
#include <QSettings>
#include <QQuickItem>
#include <QBluetoothLocalDevice>
#include <gst/gst.h>
#include <gst/app/gstappsrc.h>


#include "hu_uti.h"
#include "hu_aap.h"
#include "glib_utils.h"

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
    virtual void MediaSetupComplete(int chan) override;
    virtual void DisconnectionOrError() override;
    virtual void CustomizeOutputChannel(int chan, HU::ChannelDescriptor::OutputStreamChannel& streamChannel) override;
    virtual void AudioFocusRequest(int chan, const HU::AudioFocusRequest& request) override;
    virtual void VideoFocusRequest(int chan, const HU::VideoFocusRequest& request) override;
    virtual std::string GetCarBluetoothAddress() override;
    virtual void PhoneBluetoothReceived(std::string address) override;

    void VideoFocusHappened(bool hasFocus, bool unrequested);

    std::atomic<bool> connected;
    std::atomic<bool> videoFocus;
    std::atomic<bool> audioFocus;
};

class Headunit : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int outputWidth READ outputWidth WRITE setOutputWidth NOTIFY outputResized)
    Q_PROPERTY(int outputHeight READ outputHeight WRITE setOutputHeight NOTIFY outputResized)
    Q_PROPERTY(int videoWidth READ videoWidth NOTIFY videoResized)
    Q_PROPERTY(int videoHeight READ videoHeight NOTIFY videoResized)
public:
    Headunit();
    ~Headunit();
    int startHU();
    Q_INVOKABLE int restartHU();
    void stop();
    void exit();
    void setGstState(QString state);
    void setOutputWidth(const int a);
    void setOutputHeight(const int a);
    void setVideoWidth(const int a);
    void setVideoHeight(const int a);
    int outputWidth();
    int outputHeight();
    int videoWidth();
    int videoHeight();

    GstElement *mic_pipeline = nullptr;
    GstElement *mic_sink = nullptr;
    GstElement *aud_pipeline = nullptr;
    GstAppSrc *aud_src = nullptr;
    GstElement *au1_pipeline = nullptr;
    GstAppSrc *au1_src = nullptr;
    GstElement *vid_pipeline = nullptr;
    GstAppSrc *vid_src = nullptr;
    IHUAnyThreadInterface* g_hu = nullptr;
signals:
    void outputResized();
    void videoResized();
    void deviceConnected(QVariantMap notification);
    void btConnectionRequest(QString address);
    void videoSurfaceChanged();
public slots:
    bool mouseDown(QPoint point);
    bool mouseMove(QPoint point);
    bool mouseUp(QPoint point);
    bool keyEvent(QString key);
    void setVideoItem(QQuickItem *videoItem);
    int stopHU();
private:
    HUServer *headunit;
    DesktopEventCallbacks callbacks;
    HU::TouchInfo::TOUCH_ACTION lastAction = HU::TouchInfo::TOUCH_ACTION_RELEASE;
    int m_videoWidth = 1280;
    int m_videoHeight = 720;
    int m_outputWidth = 800;
    int m_outputHeight = 480;
    int initGst();
    static void read_mic_data(Headunit * _this);
    static gboolean bus_callback(GstBus *bus, GstMessage *message, gpointer *ptr);
    void touchEvent(HU::TouchInfo::TOUCH_ACTION action, QPoint *point);
    static uint64_t get_cur_timestamp();
    static GstPadProbeReturn convert_probe(GstPad *pad, GstPadProbeInfo *info, void *user_data);
    bool huStarted = false;
    byte ep_in_addr = -2;
    byte ep_out_addr = -2;
    QQuickItem *m_videoItem;
    GstElement *m_videoSink;
};
#endif // HEADUNITPLAYER_H
