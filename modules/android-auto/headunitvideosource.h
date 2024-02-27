#ifndef HEADUNITPLAYER_H
#define HEADUNITPLAYER_H

#include <gst/app/gstappsink.h>
#include <gst/app/gstappsrc.h>
#include <gst/gst.h>

#include <QAbstractVideoBuffer>
#include <QAbstractVideoSurface>
#include <QBluetoothLocalDevice>
#include <QObject>
#include <QPoint>
#include <QQuickItem>
#include <QSettings>
#include <QString>
#include <QVariant>
#include <QVideoSurfaceFormat>
#include <atomic>

#include "headuniteventhandler.h"
#include "headunitmediapipeline.h"
#include "hu_aap.h"

class HeadunitVideoSource : public QObject {
    Q_OBJECT
    Q_PROPERTY(int outputWidth MEMBER m_outputWidth NOTIFY outputResized)
    Q_PROPERTY(int outputHeight MEMBER m_outputHeight NOTIFY outputResized)
    Q_PROPERTY(int videoWidth READ videoWidth NOTIFY videoResized)
    Q_PROPERTY(int videoHeight READ videoHeight NOTIFY videoResized)
    Q_PROPERTY(hu_status status READ status NOTIFY statusChanged)
    Q_PROPERTY(QAbstractVideoSurface *videoSurface READ videoSurface WRITE setVideoSurface CONSTANT)

public:
    HeadunitVideoSource(QObject *parent = nullptr);
    ~HeadunitVideoSource();
    void startHU();
    void init();

    enum hu_status { NO_CONNECTION, VIDEO_WAITING, RUNNING };

    Q_ENUM(hu_status)
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

    QAbstractVideoSurface *videoSurface() const {
        return m_surface;
    }
    void setVideoSurface(QAbstractVideoSurface *surface);

    uint8_t m_mediaPipelineVolume = 100;
    uint8_t m_voicePipelineVolume = 100;

    AndroidAuto::IHUAnyThreadInterface *g_hu = nullptr;
signals:
    void outputResized();
    void videoResized();
    void deviceConnected(QVariantMap notification);
    void statusChanged();

    void playbackStarted();

public slots:
    bool mouseDown(QPoint point);
    bool mouseMove(QPoint point);
    bool mouseUp(QPoint point);
    bool keyEvent(QString key);

    void videoFrameHandler(const QVideoFrame &frame);
    void onPhoneDisconnected();
    void onPipelineStatusChanged(const Headunit::Pipeline &pipeline, const Headunit::PipelineStatus &status);

private:
    AndroidAuto::HUServer *headunit;
    HeadunitEventHandler m_eventHandler;
    HU::TouchInfo::TOUCH_ACTION lastAction = HU::TouchInfo::TOUCH_ACTION_RELEASE;
    int m_videoWidth = 800;
    int m_videoHeight = 480;
    int m_outputWidth = 800;
    int m_outputHeight = 480;
    bool huStarted = false;
    hu_status m_status = NO_CONNECTION;

    void touchEvent(HU::TouchInfo::TOUCH_ACTION action, QPoint *point);
    uint64_t get_cur_timestamp();

    QAbstractVideoSurface *m_surface = nullptr;
    QVideoSurfaceFormat m_format;
    bool m_videoStarted = false;
    HeadunitMediaPipeline m_mediaPipeline;
};

#endif  // HEADUNITPLAYER_H
