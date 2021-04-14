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

#include <QAbstractVideoSurface>
#include <QVideoSurfaceFormat>
#include <QAbstractVideoBuffer>

#include "qgstvideobuffer.h"

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
    Q_PROPERTY(hu_status status READ status NOTIFY statusChanged)
    Q_PROPERTY(QAbstractVideoSurface *videoSurface READ videoSurface WRITE setVideoSurface)

public:
    Headunit(QObject *parent = nullptr);
    ~Headunit();
    int startHU();
    int init();

    enum hu_status{
        NO_CONNECTION,
        VIDEO_WAITING,
        RUNNING
    };

    Q_ENUMS(hu_status)
    void stopPipelines();
    void setVideoWidth(const int a);
    void setVideoHeight(const int a);
    void setStatus(hu_status status);
    int videoWidth();
    int videoHeight();
    hu_status status();

    QAbstractVideoSurface* videoSurface() const { return m_surface; }
    void setVideoSurface(QAbstractVideoSurface *surface);

    GstElement *mic_pipeline = nullptr;
    GstElement *aud_pipeline = nullptr;
    GstElement *au1_pipeline = nullptr;
    GstElement *vid_pipeline = nullptr;
    GstAppSrc * m_vid_src = nullptr;
    GstAppSrc * m_aud_src = nullptr;
    GstAppSrc * m_au1_src = nullptr;

    IHUAnyThreadInterface* g_hu = nullptr;
signals:
    void outputResized();
    void videoResized();
    void deviceConnected(QVariantMap notification);
    void btConnectionRequest(QString address);
    void statusChanged();

    void receivedVideoFrame(const QVideoFrame &frame);

public slots:
    bool mouseDown(QPoint point);
    bool mouseMove(QPoint point);
    bool mouseUp(QPoint point);
    bool keyEvent(QString key);

    void videoFrameHandler(const QVideoFrame &frame);

private:
    HUServer *headunit;
    DesktopEventCallbacks callbacks;
    HU::TouchInfo::TOUCH_ACTION lastAction = HU::TouchInfo::TOUCH_ACTION_RELEASE;
    int m_videoWidth = 800;
    int m_videoHeight = 480;
    int m_outputWidth = 800;
    int m_outputHeight = 480;
    bool huStarted = false;
    hu_status m_status = NO_CONNECTION;
    static GstFlowReturn read_mic_data(GstElement *appsink, Headunit * _this);
    static gboolean bus_callback(GstBus *bus, GstMessage *message, gpointer *ptr);
    void touchEvent(HU::TouchInfo::TOUCH_ACTION action, QPoint *point);
    static uint64_t get_cur_timestamp();
    static GstFlowReturn newVideoSample (GstElement * appsink, Headunit * _this);

    QAbstractVideoSurface *m_surface = nullptr;
    QVideoSurfaceFormat m_format;
    bool m_videoStarted = false;
};
#endif // HEADUNITPLAYER_H
