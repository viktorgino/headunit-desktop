#ifndef HEADUNIT_COMMON_H
#define HEADUNIT_COMMON_H

#include "QObject"
#include "headunit/includes/AndroidAuto.h"
#include <stdint.h>

namespace Headunit {

typedef enum Pipeline {
    VideoPipeline,
    AudioPipeline,
    VoicePipeline,
    MicrophonePipeline,
    InvalidPipeline
} Pipeline;

enum PipelineStatus {
    PipelinePlaying,
    PipelineStopped,
};

inline Pipeline mediaChannelToPipeline(AndroidAuto::ServiceChannels serviceChannel)
{
    switch (serviceChannel) {
    case AndroidAuto::VideoChannel:
        return VideoPipeline;
    case AndroidAuto::MediaAudioChannel:
        return AudioPipeline;
    case AndroidAuto::Audio1Channel:
        return VoicePipeline;
    case AndroidAuto::MicrophoneChannel:
        return MicrophonePipeline;
    default:
        return InvalidPipeline;
    }
}
}

class HeadunitMicrophoneDataHandlerInterface {
public:
    virtual void handleMicrophoneData(uint64_t timestamp, const unsigned char* bufferData, int bufferSize) = 0;
};

class HeadunitMediaDataHandlerInterface {
public:
    virtual void handleMediaData(Headunit::Pipeline pipeline, uint64_t timestamp, const unsigned char* bufferData, int bufferSize) = 0;
    virtual void mediaStart(Headunit::Pipeline pipeline) = 0;
    virtual void mediaStop(Headunit::Pipeline pipeline) = 0;
};

Q_DECLARE_METATYPE(Headunit::Pipeline)
Q_DECLARE_METATYPE(Headunit::PipelineStatus)
Q_DECLARE_METATYPE(AndroidAuto::ServiceChannels)
Q_DECLARE_METATYPE(HU::AudioFocusRequest)

#endif // HEADUNIT_COMMON_H
