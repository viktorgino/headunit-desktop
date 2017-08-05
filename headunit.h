#ifndef HEADUNITPLAYER_H
#define HEADUNITPLAYER_H

#include <atomic>
#include <QObject>
#include <QString>
#include <QPoint>
#include <QGst/Pipeline>
#include <QGst/Message>
#include <QVariant>
#include <QSettings>
#include <QGst/Utils/ApplicationSource>
#include <gst/gst.h>
#include <gst/app/gstappsrc.h>

#include "hu_uti.h"
#include "hu_aap.h"
#include "glib_utils.h"
#include "usbconnectionlistener.h"

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
public:
    Headunit(const QGst::ElementPtr & sink);
    ~Headunit();
    int startHU();
    void stop();
    void exit();
    void setGstState(QString state);
    void setUsbConnectionListener(UsbConnectionListener *m_connectionListener);
    void setOutputWidth(const int a);
    void setOutputHeight(const int a);
    int outputWidth();
    int outputHeight();
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
    void deviceConnected(QVariantMap notification);
public slots:
    bool mouseDown(QPoint point);
    bool mouseMove(QPoint point);
    bool mouseUp(QPoint point);
    bool keyEvent(QString key);
    void slotDeviceAdded(const QString &dev);
    void slotAndroidDeviceAdded(const QString &dev);
    void slotDeviceRemoved(const QString &dev);
    void slotDeviceChanged(const QString &dev);
private:
    QGst::ElementPtr m_videoSink;
    HUServer *headunit;
    DesktopEventCallbacks callbacks;
    HU::TouchInfo::TOUCH_ACTION lastAction = HU::TouchInfo::TOUCH_ACTION_RELEASE;
    UsbConnectionListener *connectionListener;
    int videoWidth = 800;
    int videoHeight = 480;
    int m_outputWidth = 800;
    int m_outputHeight = 480;
    int initGst();
    void read_mic_data(GstElement * sink);
    static gboolean bus_callback(GstBus *bus, GstMessage *message, gpointer *ptr);
    void touchEvent(HU::TouchInfo::TOUCH_ACTION action, QPoint *point);
    static uint64_t get_cur_timestamp();
    bool huStarted = false;
    byte ep_in_addr = -2;
    byte ep_out_addr = -2;
    QSettings *settings;
};
#endif // HEADUNITPLAYER_H
