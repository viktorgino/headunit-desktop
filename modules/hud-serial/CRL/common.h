#pragma once

#include <stdint.h>

typedef enum {
    Bus_Can0 = 0,
    Bus_Can1,
    Bus_Can2,
    Bus_Lin0,
    Bus_Lin1,
    Bus_None
} BusNumber;

#ifndef QT_VERSION
typedef struct ClimateControlZoneControl
{
    struct {
        bool Down = false;
        bool Center = false;
        bool Up = false;
        bool Auto = false;
    } Direction;
    uint8_t Fan = 0;
    uint8_t Temperature = 0;
    uint8_t SeatHeating = 0;
} ClimateControlZoneControl;

typedef struct ClimateControlCommandFrame {
    struct {
        ClimateControlZoneControl Left;
        ClimateControlZoneControl Right;
    } Front;
    struct {
        ClimateControlZoneControl Left;
        ClimateControlZoneControl Right;
    } Rear;
    bool TempSelectLeft = false;
    bool TempSelectRight = false;
    bool FanSelectLeft = false;
    bool FanSelectRight = false;

    bool ProgAuto = false;
    bool ProgAutoFanFront = false;
    bool ProgAutoFanRear = false;
    bool ProgWindscreen = false;

    bool Recirculate = false;
    bool RearDefrost = false;
    bool AC = false;
} ClimateControlCommandFrame;
#endif
typedef enum CommandTypes {
    NoCommand,
    ClimateControlCommand,
    MediaInputCommand,
    ButtonInputCommand,
    WriteBackCommand,
    DebugMessageCommand,
    CustomCommand
} CommandTypes;

typedef struct CustomCommandFrame {
    bool Bits[16] = { false };
    uint8_t Bytes[6] = { 0 };
} CustomCommandFrame;

typedef enum Keys {
    //Numbers
    Key_1 = 0,
    Key_2,
    Key_3,
    Key_4,
    Key_5,
    Key_6,
    Key_7,
    Key_8,
    Key_9,
    Key_0,
    //Letters
    Key_A,
    Key_B,
    Key_C,
    Key_D,
    Key_E,
    Key_F,
    Key_G,
    Key_H,
    Key_I,
    Key_J,
    Key_K,
    Key_L,
    Key_M,
    Key_N,
    Key_O,
    Key_P,
    Key_Q,
    Key_R,
    Key_S,
    Key_T,
    Key_U,
    Key_V,
    Key_W,
    Key_X,
    Key_Y,
    Key_Z,
    //Navigational buttons
    Key_Enter = 50,
    Key_Exit,
    Key_Back,
    Key_Menu,
    Key_Power,
    Key_Left,
    Key_Right,
    Key_Up,
    Key_Down,
    //Media/Radio Buttons
    Key_VolumeUp = 100,
    Key_VolumeDown,
    Key_TuneUp,
    Key_TuneDown,
    Key_Sound,
    Key_AMFM,
    Key_CD,
    Key_Eject,
    Key_Auto,
    Key_Scan
} Keys;

class PlatformCallbacks {
public:
    virtual void ClimateControlCallback(ClimateControlCommandFrame) = 0;
    virtual void CustomCommandCallback(CustomCommandFrame) = 0;
    virtual void ButtonInputCommandCallback(Keys) = 0;
    virtual void SendMessageCallback(uint8_t) = 0;
    virtual void PrintString(char *) = 0;
};
