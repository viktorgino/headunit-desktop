#pragma once

#include <string.h>
#include "../common.h"
#include "../HAL/hal.h"

#include <stdio.h>

#define MESSAGE_MAX_LENGTH 128

namespace HUDSerial{
typedef enum ProtocolFrame {
    CommandByte,
    SizeByte,
    DataBuffer,
    CrcBuffer,
    EndOfFrame
} ProtocolFrame;

typedef enum CommandTypes {
    NoCommand,
    Acknowledge,
    UpdateRequest,
    ClimateControlCommand,
    ButtonInputCommand,
    BodyControlCommand,
    DriveTrainControlCommand,
    DebugMessageCommand,
    CustomCommand,
    CommandsCount
} CommandTypes;

class HUDSerial
{
public:
    HUDSerial() ;
    void receiveByte(char receivedByte);
    void sendMessage(CommandTypes messageType, uint8_t length, char *message);
    void loop();
    void sendClimateControlCommand(const ClimateControlCommandFrame &controlFrame);
    void sendButtonInputCommand(const Keys key);
    void sendCustomCommand(const CustomCommandFrame &commandFrame);
    void sendDebugMessageCommand(const char * message);
    void sendBodyControlCommand(const BodyControlCommandFrame &controlFrame);
    void sendDriveTrainControlCommand(const DriveTrainControlCommandFrame &controlFrame);
    void sendUpdateRequest();
    void sendAcknowledge(CommandTypes command);
    virtual void setCallbacks(PlatformCallbacks *callbacks){
        m_callbacks = callbacks;
    }
private:
    PlatformCallbacks *m_callbacks;
    uint16_t timerCount;

    void sendClimateControlCommand();
    void sendCustomCommand();
    void sendBodyControlCommand();
    void sendDriveTrainControlCommand();

    //Transmiting Command Frames
    ClimateControlCommandFrame m_acControlFrame;
    CustomCommandFrame m_customCommandFrame;
    BodyControlCommandFrame m_bodyControlCommandFrame;
    DriveTrainControlCommandFrame m_driveTrainControlCommandFrame;

    //Received Command Frames
    CustomCommandFrame m_receivedCustomCommandFrame;
    ClimateControlCommandFrame m_receivedAcControlFrame;
    BodyControlCommandFrame m_receivedBodyControlCommandFrame;
    DriveTrainControlCommandFrame m_receivedDriveTrainControlCommandFrame;

    //Receiver state machine
    CommandTypes m_currentCommand;
    ProtocolFrame m_receiverState;
    uint8_t m_frameSize;
    uint8_t m_dataBufferIndex;
    char m_receivedBuffer[MESSAGE_MAX_LENGTH];
    uint8_t m_crcIndex;
    uint16_t m_crc;
    
    //Acknowledge bits
    bool m_climateControlCommandAck;
    bool m_customCommandAck;
    bool m_bodyControlCommandAck;
    bool m_driveTrainControlCommandAck;
};
}
extern HUDSerial::HUDSerial hudSerial;