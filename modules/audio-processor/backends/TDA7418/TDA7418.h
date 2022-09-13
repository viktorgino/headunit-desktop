#ifndef tda7418_h
#define tda7418_h

#include <QString>
#include <QDebug>
extern "C" {
#include <linux/i2c-dev.h>
#include <i2c/smbus.h>
}
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include  <asm-generic/errno.h>
#include "../audioprocessorinterface.h"

// #define TDA7418_MASK_ATT	0x1F
// #define TDA7418_MASK_ATTPOS	0x1F
// #define TDA7418_MASK_ATTNEG	0x0F
// #define TDA7418_VOL_OFFSET 0x10
#define TDA7418_I2C_ADDRESS 0x44
#define TDA_PWRON_DEFAULT 0xFE
#define TDA_AUTOINCREMENT 0x20
// #define TDA7418_MASK_INPUTGAIN 0x78
// #define TDA7418_MASK_MUTE 0x78
// #define TDA_MASK_INPUT 0x07

class TDA7418 : public AudioProcessorInterface
{

public:
    TDA7418();
    
    enum TDA7418_Registers
    {
        TDA7418_Register_Input_Selector,
        TDA7418_Register_Loudness,
        TDA7418_Register_Volume,
        TDA7418_Register_Treble,
        TDA7418_Register_Middle,
        TDA7418_Register_Bass,
        TDA7418_Register_Middle_Bass_FC,
        TDA7418_Register_Attenuator_FL,
        TDA7418_Register_Attenuator_RL,
        TDA7418_Register_Attenuator_RR,
        TDA7418_Register_Attenuator_FR,
        TDA7418_Register_Attenuator_SUB,
        TDA7418_Register_Soft_Mute,
        TDA7418_Register_Audio_Test
    };

    enum TDA7418_Inputs{
        TDA7418_Input_PD,
        TDA7418_Input_SE1,
        TDA7418_Input_SE2,
        TDA7418_Input_SE3,
    };

    enum TDA7418_EQ_Bands
    {
        TDA7418_EQ_Band_Treble = 0x03,
        TDA7418_EQ_Band_Middle = 0x04,
        TDA7418_EQ_Band_Bass   = 0x05
    };

    void setInput(Audio_Input_Channels channel) override;
    void setVolume(int volume) override;
    void setMute(bool mute) override;
    void setInputGain(int level) override;
    void setOutputChannelLevel(Audio_Output_Channels channel, int level) override;
    void setEqBandLevel(Audio_EQ_Bands band, int level) override;


private :
    QString m_i2cDevice = "/dev/i2c-1";
    void writeByte(char registerAddress, unsigned char byteBuffer);
    void writeBytes(char registerAddress, int len, unsigned char* byteBuffer);
    char m_inputSelectorData = 0;
};

#endif
