#include "headuniteventhandler.h"

#include <QDebug>
#include <QJsonObject>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QSettings>
#include <QFileInfo>
#include <QBluetoothLocalDevice>
#include <gst/gst.h>
#include <gst/app/gstappsrc.h>
#include <gst/app/gstappsink.h>
#include <gst/video/video.h>
#include "hu_uti.h"
#include "hu_aap.h"


HeadunitEventHandler::HeadunitEventHandler(QObject *parent): QObject(parent) {
    connect(this, &HeadunitEventHandler::AudioFocusRequest, this, &HeadunitEventHandler::onAudioFocusRequest);
    connect(this, &HeadunitEventHandler::VideoFocusHappened, this, &HeadunitEventHandler::onVideoFocusHappened);
}

void HeadunitEventHandler::setHUThreadInterface(AndroidAuto::IHUAnyThreadInterface* huThreadInterface)
{
    m_huThreadInterface = huThreadInterface;
}
void HeadunitEventHandler::setMediaDataHandler(HeadunitMediaDataHandlerInterface* mediaDataHandler)
{
    m_mediaDataHandler = mediaDataHandler;
}

void HeadunitEventHandler::handleMicrophoneData(uint64_t timestamp, const unsigned char* bufferData, int bufferSize)
{
    if (m_huThreadInterface == nullptr) {
        qWarning() << "Headunit thread interface not set!";
        return;
    }

    m_huThreadInterface->queueCommand([timestamp, bufferData, bufferSize](AndroidAuto::IHUConnectionThreadInterface& s) {
        int ret = s.sendEncodedMediaPacket(1, AndroidAuto::MicrophoneChannel, AndroidAuto::HU_PROTOCOL_MESSAGE::MediaDataWithTimestamp, timestamp, bufferData, bufferSize);
        if (ret < 0) {
            qDebug("read_mic_data(): hu_aap_enc_send() failed with (%d)", ret);
        }
    });
}

int HeadunitEventHandler::MediaPacket(AndroidAuto::ServiceChannels chan, uint64_t timestamp, const byte* buf, int len)
{
    Headunit::Pipeline pipeline = Headunit::mediaChannelToPipeline(chan);
    if (pipeline == Headunit::InvalidPipeline) {
        qDebug() << "Unknown channel : " << chan;
        return -1;
    }
    m_mediaDataHandler->handleMediaData(pipeline, timestamp, buf, len);
    return 0;
}
int HeadunitEventHandler::MediaStart(AndroidAuto::ServiceChannels chan)
{
    Headunit::Pipeline pipeline = Headunit::mediaChannelToPipeline(chan);
    if (pipeline == Headunit::InvalidPipeline) {
        qDebug() << "Unknown channel : " << chan;
        return -1;
    }
    m_mediaDataHandler->mediaStart(pipeline);
    return 0;
}

int HeadunitEventHandler::MediaStop(AndroidAuto::ServiceChannels chan)
{
    Headunit::Pipeline pipeline = Headunit::mediaChannelToPipeline(chan);
    if (pipeline == Headunit::InvalidPipeline) {
        qDebug() << "Unknown channel : " << chan;
        return -1;
    }
    m_mediaDataHandler->mediaStop(pipeline);
    return 0;
}

void HeadunitEventHandler::DisconnectionOrError()
{
    qDebug("Android Device disconnected, pausing gstreamer");
    emit phoneDisconnected();
}

void HeadunitEventHandler::CustomizeOutputChannel(AndroidAuto::ServiceChannels /* unused */, HU::ChannelDescriptor::OutputStreamChannel& /* unused */)
{
}
void HeadunitEventHandler::MediaSetupComplete(AndroidAuto::ServiceChannels chan)
{
    if (chan == AndroidAuto::VideoChannel) {
        emit VideoFocusHappened(true, true);
    }
}

void HeadunitEventHandler::VideoFocusRequest(AndroidAuto::ServiceChannels /* unused */, const HU::VideoFocusRequest& request)
{
    emit VideoFocusHappened(request.mode() == HU::VIDEO_FOCUS_MODE_FOCUSED, false);
}

std::string HeadunitEventHandler::GetCarBluetoothAddress()
{
    QList<QBluetoothHostInfo> localAdapters = QBluetoothLocalDevice::allDevices();
    if (localAdapters.size() > 0) {
        return localAdapters.at(0).address().toString().toStdString();
    }
    return std::string();
}

void HeadunitEventHandler::PhoneBluetoothReceived(std::string address)
{
    emit bluetoothConnectionRequest(QString::fromStdString(address));
}

void HeadunitEventHandler::HandlePhoneStatus(AndroidAuto::IHUConnectionThreadInterface& stream, const HU::PhoneStatus& phoneStatus)
{
}
void HeadunitEventHandler::HandleNaviStatus(AndroidAuto::IHUConnectionThreadInterface& stream, const HU::NAVMessagesStatus& request)
{
}
void HeadunitEventHandler::HandleNaviTurn(AndroidAuto::IHUConnectionThreadInterface& stream, const HU::NAVTurnMessage& request)
{
}
void HeadunitEventHandler::HandleNaviTurnDistance(AndroidAuto::IHUConnectionThreadInterface& stream, const HU::NAVDistanceMessage& request)
{
}

uint64_t HeadunitEventHandler::get_cur_timestamp()
{
    struct timespec tp;
    /* Fetch the time stamp */
    clock_gettime(CLOCK_REALTIME, &tp);

    return tp.tv_sec * 1000000 + tp.tv_nsec / 1000;
}

void HeadunitEventHandler::onAudioFocusRequest(AndroidAuto::ServiceChannels chan, const HU::AudioFocusRequest& request)
{
    HU::AudioFocusResponse response;
    if (request.focus_type() == HU::AudioFocusRequest::AUDIO_FOCUS_RELEASE) {
        response.set_focus_type(HU::AudioFocusResponse::AUDIO_FOCUS_STATE_LOSS);
    } else {
        response.set_focus_type(HU::AudioFocusResponse::AUDIO_FOCUS_STATE_GAIN);
    }

    if (m_huThreadInterface == nullptr) {
        qWarning() << "Headunit thread interface not set!";
        return;
    }

    m_huThreadInterface->queueCommand([chan, response](AndroidAuto::IHUConnectionThreadInterface& s) {
        s.sendEncodedMessage(0, chan, AndroidAuto::HU_PROTOCOL_MESSAGE::AudioFocusResponse, response);
    });
}

void HeadunitEventHandler::onVideoFocusHappened(bool hasFocus, bool unrequested)
{
    if (m_huThreadInterface == nullptr) {
        qWarning() << "Headunit thread interface not set!";
        return;
    }

    m_huThreadInterface->queueCommand([hasFocus, unrequested](AndroidAuto::IHUConnectionThreadInterface& s) {
        HU::VideoFocus videoFocusGained;
        videoFocusGained.set_mode(hasFocus ? HU::VIDEO_FOCUS_MODE_FOCUSED : HU::VIDEO_FOCUS_MODE_UNFOCUSED);
        videoFocusGained.set_unrequested(unrequested);
        s.sendEncodedMessage(0, AndroidAuto::VideoChannel, AndroidAuto::HU_MEDIA_CHANNEL_MESSAGE::VideoFocus, videoFocusGained);
    });

    // Set speed to 0
    HU::SensorEvent sensorEvent;
    sensorEvent.add_location_data()->set_speed(0);
    m_huThreadInterface->queueCommand([sensorEvent](AndroidAuto::IHUConnectionThreadInterface& s) {
        s.sendEncodedMessage(0, AndroidAuto::SensorChannel, AndroidAuto::HU_SENSOR_CHANNEL_MESSAGE::SensorEvent, sensorEvent);
    });
}

void HeadunitEventHandler::startMedia()
{
    if (m_huThreadInterface == nullptr) {
        qWarning() << "Headunit thread interface not set!";
        return;
    }

    m_huThreadInterface->queueCommand([](AndroidAuto::IHUConnectionThreadInterface& s) {
        HU::InputEvent inputEvent;
        inputEvent.set_timestamp(HeadunitEventHandler::get_cur_timestamp());
        HU::ButtonInfo* buttonInfo = inputEvent.mutable_button()->add_button();
        buttonInfo->set_is_pressed(true);
        buttonInfo->set_meta(0);
        buttonInfo->set_long_press(false);
        buttonInfo->set_scan_code(AndroidAuto::HUIB_START);

        s.sendEncodedMessage(0, AndroidAuto::TouchChannel, AndroidAuto::HU_INPUT_CHANNEL_MESSAGE::InputEvent, inputEvent);
        buttonInfo->set_is_pressed(false);
        s.sendEncodedMessage(0, AndroidAuto::TouchChannel, AndroidAuto::HU_INPUT_CHANNEL_MESSAGE::InputEvent, inputEvent);
    });
}

void HeadunitEventHandler::stopMedia()
{
    if (m_huThreadInterface == nullptr) {
        qWarning() << "Headunit thread interface not set!";
        return;
    }

    m_huThreadInterface->queueCommand([](AndroidAuto::IHUConnectionThreadInterface& s) {
        HU::InputEvent inputEvent;
        inputEvent.set_timestamp(HeadunitEventHandler::get_cur_timestamp());
        HU::ButtonInfo* buttonInfo = inputEvent.mutable_button()->add_button();
        buttonInfo->set_is_pressed(true);
        buttonInfo->set_meta(0);
        buttonInfo->set_long_press(false);
        buttonInfo->set_scan_code(AndroidAuto::HUIB_STOP);

        s.sendEncodedMessage(0, AndroidAuto::TouchChannel, AndroidAuto::HU_INPUT_CHANNEL_MESSAGE::InputEvent, inputEvent);
        buttonInfo->set_is_pressed(false);
        s.sendEncodedMessage(0, AndroidAuto::TouchChannel, AndroidAuto::HU_INPUT_CHANNEL_MESSAGE::InputEvent, inputEvent);
    });
}

void HeadunitEventHandler::prevTrack()
{
    if (m_huThreadInterface == nullptr) {
        qWarning() << "Headunit thread interface not set!";
        return;
    }

    m_huThreadInterface->queueCommand([](AndroidAuto::IHUConnectionThreadInterface& s) {
        HU::InputEvent inputEvent;
        inputEvent.set_timestamp(HeadunitEventHandler::get_cur_timestamp());
        HU::ButtonInfo* buttonInfo = inputEvent.mutable_button()->add_button();
        buttonInfo->set_is_pressed(true);
        buttonInfo->set_meta(0);
        buttonInfo->set_long_press(false);
        buttonInfo->set_scan_code(AndroidAuto::HUIB_PREV);

        s.sendEncodedMessage(0, AndroidAuto::TouchChannel, AndroidAuto::HU_INPUT_CHANNEL_MESSAGE::InputEvent, inputEvent);
        buttonInfo->set_is_pressed(false);
        s.sendEncodedMessage(0, AndroidAuto::TouchChannel, AndroidAuto::HU_INPUT_CHANNEL_MESSAGE::InputEvent, inputEvent);
    });
}

void HeadunitEventHandler::nextTrack()
{
    if (m_huThreadInterface == nullptr) {
        qWarning() << "Headunit thread interface not set!";
        return;
    }

    m_huThreadInterface->queueCommand([](AndroidAuto::IHUConnectionThreadInterface& s) {
        HU::InputEvent inputEvent;
        inputEvent.set_timestamp(HeadunitEventHandler::get_cur_timestamp());
        HU::ButtonInfo* buttonInfo = inputEvent.mutable_button()->add_button();
        buttonInfo->set_is_pressed(true);
        buttonInfo->set_meta(0);
        buttonInfo->set_long_press(false);
        buttonInfo->set_scan_code(AndroidAuto::HUIB_NEXT);

        s.sendEncodedMessage(0, AndroidAuto::TouchChannel, AndroidAuto::HU_INPUT_CHANNEL_MESSAGE::InputEvent, inputEvent);
        buttonInfo->set_is_pressed(false);
        s.sendEncodedMessage(0, AndroidAuto::TouchChannel, AndroidAuto::HU_INPUT_CHANNEL_MESSAGE::InputEvent, inputEvent);
    });
}

void HeadunitEventHandler::touchEvent(HU::TouchInfo::TOUCH_ACTION action, const QPoint& point)
{
    if (m_huThreadInterface == nullptr) {
        qWarning() << "Headunit thread interface not set!";
        return;
    }

    m_huThreadInterface->queueCommand([action, point](AndroidAuto::IHUConnectionThreadInterface& s) {
        HU::InputEvent inputEvent;
        inputEvent.set_timestamp(HeadunitEventHandler::get_cur_timestamp());
        HU::TouchInfo* touchEvent = inputEvent.mutable_touch();
        touchEvent->set_action(action);
        HU::TouchInfo::Location* touchLocation = touchEvent->add_location();
        touchLocation->set_x(point.x());
        touchLocation->set_y(point.y());
        touchLocation->set_pointer_id(0);

        /* Send touch event */

        int ret = s.sendEncodedMessage(0, AndroidAuto::TouchChannel, AndroidAuto::HU_INPUT_CHANNEL_MESSAGE::InputEvent, inputEvent);
        if (ret < 0) {
            qDebug("aa_touch_event(): hu_aap_enc_send() failed with (%d)", ret);
        }
    });
}
