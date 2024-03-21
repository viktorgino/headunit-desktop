#include "headunitvideosource.h"

#include <gst/app/gstappsink.h>
#include <gst/app/gstappsrc.h>
#include <gst/gst.h>
#include <gst/video/video.h>

#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSettings>

#include "headunit/includes/AndroidAuto.h"

HeadunitVideoSource::HeadunitVideoSource(QObject* parent) : QObject(parent), m_eventHandler(this), m_mediaPipeline(this) {
    qRegisterMetaType<Headunit::Pipeline>();
    qRegisterMetaType<Headunit::PipelineStatus>();
    qRegisterMetaType<AndroidAuto::ServiceChannels>();
    qRegisterMetaType<HU::AudioFocusRequest>();

    m_eventHandler.setMediaDataHandler(&m_mediaPipeline);
    m_mediaPipeline.setMicrophoneDataHandler(&m_eventHandler);

    connect(&m_mediaPipeline, &HeadunitMediaPipeline::pipelineStatusChanged, this, &HeadunitVideoSource::onPipelineStatusChanged);
    connect(&m_mediaPipeline, &HeadunitMediaPipeline::receivedVideoFrame, this, &HeadunitVideoSource::videoFrameHandler);

    connect(&m_eventHandler, &HeadunitEventHandler::phoneDisconnected, this, &HeadunitVideoSource::onPhoneDisconnected);
}

HeadunitVideoSource::~HeadunitVideoSource() {
    if (headunit) {
        delete (headunit);
        qDebug("HeadunitVideoSurfaceItem::~Headunit() called hu_aap_shutdown()");
    }
}

void HeadunitVideoSource::init() {
    m_mediaPipeline.init();
}

void HeadunitVideoSource::startHU() {
    QSettings settings;
    std::map<std::string, std::string> aa_settings;

    qDebug() << "Starting headunit";
    if (settings.childGroups().contains("AndroidAuto")) {
        settings.beginGroup("AndroidAuto");
        QStringList keys = settings.childKeys();

        for (int i = 0; i < keys.size(); ++i) {
            aa_settings[keys.at(i).toStdString()] = settings.value(keys.at(i)).toString().toStdString();
        }
    }

    if (settings.childKeys().contains("resolution")) {
        QVariant res = settings.value("resolution");
        if (res.canConvert(QMetaType::Int)) {
            switch (res.toInt()) {
                case 1:
                    setVideoWidth(800);
                    setVideoHeight(480);
                    break;
                case 2:
                    setVideoWidth(1280);
                    setVideoHeight(720);
                    break;
                case 3:
                    setVideoWidth(1920);
                    setVideoHeight(1080);
                    break;
            }
        }
    }
    aa_settings["ts_height"] = std::to_string(m_videoHeight);
    aa_settings["ts_width"] = std::to_string(m_videoWidth);

    headunit = new AndroidAuto::HUServer(m_eventHandler, aa_settings);
    int ret = headunit->start();
    if (ret >= 0) {
        g_hu = &headunit->GetAnyThreadInterface();
        m_eventHandler.setHUThreadInterface(g_hu);

        huStarted = true;
        setStatus(HeadunitVideoSource::VIDEO_WAITING);
    }
}

void HeadunitVideoSource::setVideoSurface(QAbstractVideoSurface* surface) {
    qDebug() << "Setting video surface";
    if (m_surface != surface && m_surface && m_surface->isActive()) {
        m_surface->stop();
    }
    m_surface = surface;
}

void HeadunitVideoSource::videoFrameHandler(const QVideoFrame& frame) {
    if (m_surface != nullptr) {
        if (!m_videoStarted) {
            m_videoStarted = true;
            QVideoSurfaceFormat format(frame.size(), QVideoFrame::Format_YUV420P);
            m_surface->start(format);
        }
        m_surface->present(frame);
    }
}

void HeadunitVideoSource::onPipelineStatusChanged(const Headunit::Pipeline& pipeline, const Headunit::PipelineStatus& status) {
    switch (pipeline) {
        case Headunit::VideoPipeline:
            if (status == Headunit::PipelinePlaying) {
                setStatus(RUNNING);
            } else {
                setStatus(VIDEO_WAITING);
            }
            break;
        case Headunit::AudioPipeline:
            if (status == Headunit::PipelinePlaying) {
                emit playbackStarted();
            }
            break;
        case Headunit::VoicePipeline:
        case Headunit::MicrophonePipeline:
        default:
            break;
    }
}

void HeadunitVideoSource::onPhoneDisconnected() {
    setStatus(HeadunitVideoSource::NO_CONNECTION);
}

bool HeadunitVideoSource::mouseDown(QPoint point) {
    lastAction = HU::TouchInfo::TOUCH_ACTION_PRESS;
    touchEvent(HU::TouchInfo::TOUCH_ACTION_PRESS, &point);
    return true;
}

bool HeadunitVideoSource::mouseMove(QPoint point) {
    if (lastAction == HU::TouchInfo::TOUCH_ACTION_PRESS) {
        if (point.x() > m_outputWidth) {
            QPoint falsepoint = QPoint(m_outputWidth, point.y());
            return mouseUp(falsepoint);
        }
        if (point.y() > m_outputHeight) {
            QPoint falsepoint = QPoint(point.x(), m_outputHeight);
            mouseUp(falsepoint);
        }
        if (point.x() < 1) {
            QPoint falsepoint = QPoint(1, point.y());
            return mouseUp(falsepoint);
        }
        if (point.y() < 1) {
            QPoint falsepoint = QPoint(point.x(), 1);
            return mouseUp(falsepoint);
        }
        touchEvent(HU::TouchInfo::TOUCH_ACTION_DRAG, &point);
    }
    return true;
}
bool HeadunitVideoSource::mouseUp(QPoint point) {
    if (lastAction == HU::TouchInfo::TOUCH_ACTION_PRESS) {
        lastAction = HU::TouchInfo::TOUCH_ACTION_RELEASE;
        touchEvent(HU::TouchInfo::TOUCH_ACTION_RELEASE, &point);
    }
    return true;
}

bool HeadunitVideoSource::keyEvent(QString /*key*/) {
    return true;
}

void HeadunitVideoSource::setVideoWidth(const int a) {
    m_videoWidth = a;
    emit videoResized();
}

void HeadunitVideoSource::setVideoHeight(const int a) {
    m_videoHeight = a;
    emit videoResized();
}

int HeadunitVideoSource::videoWidth() {
    return m_videoWidth;
}

int HeadunitVideoSource::videoHeight() {
    return m_videoHeight;
}

HeadunitVideoSource::hu_status HeadunitVideoSource::status() {
    return m_status;
}

void HeadunitVideoSource::setStatus(hu_status status) {
    m_status = status;
    emit statusChanged();
}

void HeadunitVideoSource::touchEvent(HU::TouchInfo::TOUCH_ACTION action, QPoint* point) {
    float normx = float(point->x()) / float(m_outputWidth);
    float normy = float(point->y()) / float(m_outputHeight);

    unsigned int x = (unsigned int)(normx * m_videoWidth);
    unsigned int y = (unsigned int)(normy * m_videoHeight);

    if (huStarted) {
        m_eventHandler.touchEvent(action, QPoint(x, y));
    }
}

void HeadunitVideoSource::startMedia() {
    if (huStarted) {
        m_eventHandler.startMedia();
    }
}

void HeadunitVideoSource::stopMedia() {
    if (huStarted) {
        m_eventHandler.stopMedia();
    }
}

void HeadunitVideoSource::prevTrack() {
    if (huStarted) {
        m_eventHandler.prevTrack();
    }
}

void HeadunitVideoSource::nextTrack() {
    if (huStarted) {
        m_eventHandler.nextTrack();
    }
}

void HeadunitVideoSource::setMediaVolume(uint8_t volume) {
    m_mediaPipelineVolume = volume;
    // TODO: LINK to pipeline
}

void HeadunitVideoSource::setVoiceVolume(uint8_t volume) {
    m_voicePipelineVolume = volume;
    // TODO: LINK to pipeline
}
void HeadunitVideoSource::setNigthmode(bool night) {
    if (huStarted) {
        HU::SensorEvent sensorEvent;
        sensorEvent.add_night_mode()->set_is_night(night);
        g_hu->queueCommand([sensorEvent](AndroidAuto::IHUConnectionThreadInterface& s) {
            s.sendEncodedMessage(0, AndroidAuto::SensorChannel, AndroidAuto::SENSOR_CHANNEL_MESSAGE::SensorEvent, sensorEvent);
        });
    }
}
