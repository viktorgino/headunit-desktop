#ifndef HVACCOMMON_H
#define HVACCOMMON_H

#include <stdint.h>
#include <QObject>

typedef struct ClimateControlZoneControl
{
    Q_GADGET
public:
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
    Q_GADGET
public:
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

Q_DECLARE_METATYPE(ClimateControlCommandFrame)
Q_DECLARE_METATYPE(ClimateControlZoneControl)
#endif // HVACCOMMON_H
