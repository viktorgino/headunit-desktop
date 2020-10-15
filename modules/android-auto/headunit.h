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
#include <gst/app/gstappsink.h>


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
};

class Headunit : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int outputWidth MEMBER m_outputWidth NOTIFY outputResized)
    Q_PROPERTY(int outputHeight MEMBER m_outputHeight NOTIFY outputResized)
    Q_PROPERTY(int videoWidth READ videoWidth NOTIFY videoResized)
    Q_PROPERTY(int videoHeight READ videoHeight NOTIFY videoResized)
public:
    Headunit();
    ~Headunit();
    int startHU();
    void stop();
    void exit();
    void stopPipelines();
    void setVideoWidth(const int a);
    void setVideoHeight(const int a);
    int videoWidth();
    int videoHeight();

    GstElement *mic_pipeline = nullptr;
    GstElement *aud_pipeline = nullptr;
    GstElement *au1_pipeline = nullptr;
    GstElement *vid_pipeline = nullptr;

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

private:
    HUServer *headunit;
    DesktopEventCallbacks callbacks;
    HU::TouchInfo::TOUCH_ACTION lastAction = HU::TouchInfo::TOUCH_ACTION_RELEASE;
    int m_videoWidth = 800;
    int m_videoHeight = 480;
    int m_outputWidth = 800;
    int m_outputHeight = 480;
    bool huStarted = false;
    int initGst();
    static GstFlowReturn read_mic_data(GstElement *appsink, Headunit * _this);
    static gboolean bus_callback(GstBus *bus, GstMessage *message, gpointer *ptr);
    void touchEvent(HU::TouchInfo::TOUCH_ACTION action, QPoint *point);
    static uint64_t get_cur_timestamp();
};
#endif // HEADUNITPLAYER_H
