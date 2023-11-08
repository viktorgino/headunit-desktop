#ifndef HEADUNITEVENTHANDLER_H
#define HEADUNITEVENTHANDLER_H

#include <QObject>
#include <QPoint>

#include <gst/gstbuffer.h>
#include "hu_aap.h"
#include "hu_uti.h"

#include "headunit_common.h"

class HeadunitEventHandler : public QObject, public HeadunitMicrophoneDataHandlerInterface, public AndroidAuto::IHUConnectionThreadEventCallbacks {
    Q_OBJECT
public:
    explicit HeadunitEventHandler(QObject* parent = nullptr);

    int MediaPacket(AndroidAuto::ServiceChannels chan, uint64_t timestamp, const byte* buf, int len) override;
    void MediaSetupComplete(AndroidAuto::ServiceChannels chan) override;
    void DisconnectionOrError() override;
    void CustomizeOutputChannel(AndroidAuto::ServiceChannels chan, HU::ChannelDescriptor::OutputStreamChannel& streamChannel) override;

    std::string GetCarBluetoothAddress() override;

    void VideoFocusRequest(AndroidAuto::ServiceChannels chan, const HU::VideoFocusRequest& request) override;
    void PhoneBluetoothReceived(std::string address) override;
    void HandlePhoneStatus(AndroidAuto::IHUConnectionThreadInterface& stream, const HU::PhoneStatus& phoneStatus) override;
    void HandleNaviStatus(AndroidAuto::IHUConnectionThreadInterface& stream, const HU::NAVMessagesStatus& request) override;
    void HandleNaviTurn(AndroidAuto::IHUConnectionThreadInterface& stream, const HU::NAVTurnMessage& request) override;
    void HandleNaviTurnDistance(AndroidAuto::IHUConnectionThreadInterface& stream, const HU::NAVDistanceMessage& request) override;
    int MediaStart(AndroidAuto::ServiceChannels chan) override;
    int MediaStop(AndroidAuto::ServiceChannels chan) override;

    void startMedia();
    void stopMedia();
    void prevTrack();
    void nextTrack();
    void touchEvent(HU::TouchInfo::TOUCH_ACTION action, const QPoint& point);

    void handleMicrophoneData(uint64_t timestamp, const unsigned char* bufferData, int bufferSize) override;

    void setHUThreadInterface(AndroidAuto::IHUAnyThreadInterface* huThreadInterface);
    void setMediaDataHandler(HeadunitMediaDataHandlerInterface* mediaDataHandler);

signals:
    void bluetoothConnectionRequest(QString address);
    void VideoFocusHappened(bool hasFocus, bool unrequested);
    void AudioFocusRequest(const AndroidAuto::ServiceChannels chan, const HU::AudioFocusRequest& request) override;

    void phoneDisconnected();


private slots:
    void onVideoFocusHappened(bool hasFocus, bool unrequested);
    void onAudioFocusRequest(AndroidAuto::ServiceChannels chan, const HU::AudioFocusRequest& request);

private:
    AndroidAuto::IHUAnyThreadInterface* m_huThreadInterface = nullptr;
    HeadunitMediaDataHandlerInterface* m_mediaDataHandler = nullptr;

    static uint64_t get_cur_timestamp();
};

#endif // HEADUNITEVENTHANDLER_H
