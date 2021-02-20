#pragma once

#include <string.h>
#include "../common.h"
#include "../HAL/hal.h"

#include <stdio.h>

typedef enum ProtocolFrame {
    CommandByte,
    SizeByte,
    DataBuffer,
    CrcBuffer,
    EndOfFrame
} ProtocolFrame;

class HUDSerial
{
public:
    HUDSerial() ;
    void receiveByte(char receivedByte);
    void sendMessage(CommandTypes messageType, uint8_t length, char *message);
    void loop();
    void sendBusMessage(BusNumber bus, uint32_t id, uint8_t len, uint8_t *buf);
    void sendClimateControlCommand(ClimateControlCommandFrame controlFrame);
    void sendButtonInputCommand(Keys key);
    void sendCustomCommand(CustomCommandFrame commandFrame);
    void sendDebugMessageCommand(const char * message);
    virtual void setCallbacks(PlatformCallbacks *callbacks){
        m_callbacks = callbacks;
    }
private:
    PlatformCallbacks *m_callbacks;
    uint16_t timerCount;
    void sendClimateControlCommand();
    void sendCustomCommand();
    ClimateControlCommandFrame m_acControlFrame;
    CustomCommandFrame m_commandFrame;
    CommandTypes m_currentCommand;
    ProtocolFrame m_receiverState;
    uint8_t m_frameSize;
    uint8_t m_dataBufferIndex;
    char m_receivedBuffer[64];
    uint8_t m_crcIndex;
    uint16_t m_crc;
    CustomCommandFrame m_receivedCustomCommandFrame;
    ClimateControlCommandFrame m_receivedAcControlFrame;
    char m_debugMessage[64] = {0};
};
