#ifndef AUDIOPROCESSORINTERFACE_H
#define AUDIOPROCESSORINTERFACE_H

class AudioProcessorInterface
{
public:
    enum Audio_Output_Channels
    {
        Front_Left,
        Front_Right,
        Rear_Left,
        Rear_Right,
        Subwoofer
    };
    enum Audio_Input_Channels
    {
        Input_1,
        Input_2,
        Input_3,
        Input_4,
        Input_5,
        Input_6,
        Input_7,
        Input_8
    };
    enum Audio_EQ_Bands
    {
        EQ_Band_Bass,
        EQ_Band_Middle,
        EQ_Band_Treble
    };
    virtual void setInput(Audio_Input_Channels channel) = 0;
    virtual void setVolume(int volume) = 0;
    virtual void setMute(bool mute) = 0;
    virtual void setInputGain(int level) = 0;
    virtual void setOutputChannelLevel(Audio_Output_Channels channel, int level) = 0;
    virtual void setEqBandLevel(Audio_EQ_Bands band, int level) = 0;
};
#endif // AUDIOPROCESSORINTERFACE_H