#ifndef HEADUNITPLAYER_H
#define HEADUNITPLAYER_H

#include <gst/app/gstappsink.h>
#include <gst/app/gstappsrc.h>
#include <gst/gst.h>
#include <QBluetoothLocalDevice>
#include <QObject>
#include <QPoint>
#include <QQuickItem>
#include <QSettings>
#include <QString>
#include <QVariant>
#include <atomic>

#include <QAbstractVideoBuffer>
#include <QAbstractVideoSurface>
#include <QVideoSurfaceFormat>

#include "qgstvideobuffer.h"

#include "glib_utils.h"
#include "hu_aap.h"
#include "hu_uti.h"

class Headunit;

class DesktopEventCallbacks : public AndroidAuto::IHUConnectionThreadEventCallbacks {
private:
    Headunit *headunit;

public:
    DesktopEventCallbacks(Headunit *hu) { headunit = hu; }
    int MediaPacket(AndroidAuto::ServiceChannels chan, uint64_t timestamp, const byte *buf,
                    int len) override;
    int MediaStart(AndroidAuto::ServiceChannels chan) override;
    int MediaStop(AndroidAuto::ServiceChannels chan) override;
    void MediaSetupComplete(AndroidAuto::ServiceChannels chan) override;
    void DisconnectionOrError() override;
    void CustomizeOutputChannel(
        AndroidAuto::ServiceChannels chan,
        HU::ChannelDescriptor::OutputStreamChannel &streamChannel) override;
    void AudioFocusRequest(
        AndroidAuto::ServiceChannels chan, const HU::AudioFocusRequest &request) override;
    void VideoFocusRequest(
        AndroidAuto::ServiceChannels chan, const HU::VideoFocusRequest &request) override;
    std::string GetCarBluetoothAddress() override;
    void PhoneBluetoothReceived(std::string address) override;

    void VideoFocusHappened(bool hasFocus, bool unrequested);
    void HandlePhoneStatus(AndroidAuto::IHUConnectionThreadInterface& stream,
                           const HU::PhoneStatus& phoneStatus) override;
    void HandleNaviStatus(AndroidAuto::IHUConnectionThreadInterface& stream,
                          const HU::NAVMessagesStatus& request) override;
    void HandleNaviTurn(AndroidAuto::IHUConnectionThreadInterface& stream,
                        const HU::NAVTurnMessage& request) override;
    void HandleNaviTurnDistance(AndroidAuto::IHUConnectionThreadInterface& stream,
                                const HU::NAVDistanceMessage& request) override;
};

class Headunit : public QObject {
    Q_OBJECT
    Q_PROPERTY(int outputWidth MEMBER m_outputWidth NOTIFY outputResized)
    Q_PROPERTY(int outputHeight MEMBER m_outputHeight NOTIFY outputResized)
    Q_PROPERTY(int videoWidth READ videoWidth NOTIFY videoResized)
    Q_PROPERTY(int videoHeight READ videoHeight NOTIFY videoResized)
    Q_PROPERTY(hu_status status READ status NOTIFY statusChanged)
    Q_PROPERTY(QAbstractVideoSurface *videoSurface READ videoSurface WRITE
                   setVideoSurface)

public:
    Headunit(QObject *parent = nullptr);
    ~Headunit();
    int startHU();
    int init();

    enum hu_status { NO_CONNECTION, VIDEO_WAITING, RUNNING };

    Q_ENUMS(hu_status)
    void stopPipelines();
    void setVideoWidth(const int a);
    void setVideoHeight(const int a);
    void setStatus(hu_status status);
    int videoWidth();
    int videoHeight();
    hu_status status();

    void startMedia();
    void stopMedia();
    void prevTrack();
    void nextTrack();
    void setMediaVolume(uint8_t volume);
    void setVoiceVolume(uint8_t volume);
    void setNigthmode(bool night);

    QAbstractVideoSurface *videoSurface() const { return m_surface; }
    void setVideoSurface(QAbstractVideoSurface *surface);

    GstElement *mic_pipeline = nullptr;
    GstElement *aud_pipeline = nullptr;
    GstElement *au1_pipeline = nullptr;
    GstElement *vid_pipeline = nullptr;
    GstAppSrc *m_vid_src = nullptr;
    GstAppSrc *m_aud_src = nullptr;
    GstAppSrc *m_au1_src = nullptr;

    uint8_t m_mediaPipelineVolume = 100;
    uint8_t m_voicePipelineVolume = 100;

    AndroidAuto::IHUAnyThreadInterface *g_hu = nullptr;
signals:
    void outputResized();
    void videoResized();
    void deviceConnected(QVariantMap notification);
    void btConnectionRequest(QString address);
    void statusChanged();

    void receivedVideoFrame(const QVideoFrame &frame);

    void playbackStarted();

public slots:
    bool mouseDown(QPoint point);
    bool mouseMove(QPoint point);
    bool mouseUp(QPoint point);
    bool keyEvent(QString key);

    void videoFrameHandler(const QVideoFrame &frame);

private:
    AndroidAuto::HUServer *headunit;
    DesktopEventCallbacks callbacks;
    HU::TouchInfo::TOUCH_ACTION lastAction =
        HU::TouchInfo::TOUCH_ACTION_RELEASE;
    int m_videoWidth = 800;
    int m_videoHeight = 480;
    int m_outputWidth = 800;
    int m_outputHeight = 480;
    bool huStarted = false;
    hu_status m_status = NO_CONNECTION;
    static GstFlowReturn read_mic_data(GstElement *appsink, Headunit *_this);
    static gboolean bus_callback(GstBus *bus, GstMessage *message,
                                 gpointer *ptr);
    void touchEvent(HU::TouchInfo::TOUCH_ACTION action, QPoint *point);
    static uint64_t get_cur_timestamp();
    static GstFlowReturn newVideoSample(GstElement *appsink, Headunit *_this);

    QAbstractVideoSurface *m_surface = nullptr;
    QVideoSurfaceFormat m_format;
    bool m_videoStarted = false;
};
#endif  // HEADUNITPLAYER_H
