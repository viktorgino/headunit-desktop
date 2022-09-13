#include "HUDSerial.h"
#include "../HAL/hal.h"
namespace HUDSerial {

HUDSerial::HUDSerial()
    : m_acControlFrame(),
      m_customCommandFrame(),
      m_receivedCustomCommandFrame(),
      m_receivedAcControlFrame(),
      m_receivedBodyControlCommandFrame(),
      m_receivedDriveTrainControlCommandFrame(),
      m_currentCommand(NoCommand),
      m_receiverState(CommandByte),
      m_frameSize(0),
      m_dataBufferIndex(0),
      m_receivedBuffer(),
      m_crcIndex(0),
      m_crc(0) {}


//TODO : Break this down to individual functions
void HUDSerial::receiveByte(char receivedByte) {
    switch (m_receiverState) {
    case CommandByte: {
        if (receivedByte <= CustomCommand) {
            m_currentCommand = static_cast<CommandTypes>(receivedByte);
            m_receiverState = SizeByte;
        }
        break;
    }
    case SizeByte: {
        m_frameSize = receivedByte;
        m_dataBufferIndex = 0;
        memset(m_receivedBuffer, 0, sizeof m_receivedBuffer);

        m_crcIndex = 0;
        m_crc = 0;
        if (m_frameSize > MESSAGE_MAX_LENGTH) {
            m_frameSize = MESSAGE_MAX_LENGTH;
        } else if (m_frameSize > 0) {
            m_receiverState = DataBuffer;
        } else {
            m_receiverState = CrcBuffer;
        }
        break;
    }
    case DataBuffer: {
        if (m_dataBufferIndex < m_frameSize) {
            m_receivedBuffer[m_dataBufferIndex] = receivedByte;
            switch (m_currentCommand) {
            case Acknowledge: {
                switch (receivedByte) {
                case ClimateControlCommand:
                    m_climateControlCommandAck = false;
                    break;
                case CustomCommand:
                    m_customCommandAck = false;
                    break;
                case DriveTrainControlCommand:
                    m_driveTrainControlCommandAck = false;
                    break;
                case BodyControlCommand:
                    m_bodyControlCommandAck = false;
                    break;
                default:
                    break;
                }

                m_receiverState = CrcBuffer;
            } break;
            case ClimateControlCommand: {
                switch (m_dataBufferIndex) {
                case 0: {
                    m_receivedAcControlFrame.Front.Left.Direction.Up = receivedByte & 1;
                    m_receivedAcControlFrame.Front.Left.Direction.Center = receivedByte & (1 << 1);
                    m_receivedAcControlFrame.Front.Left.Direction.Down = receivedByte & (1 << 2);
                    m_receivedAcControlFrame.Front.Left.Direction.Auto = receivedByte & (1 << 3);
                    m_receivedAcControlFrame.Front.Right.Direction.Up = receivedByte & (1 << 4);
                    m_receivedAcControlFrame.Front.Right.Direction.Center = receivedByte & (1 << 5);
                    m_receivedAcControlFrame.Front.Right.Direction.Down = receivedByte & (1 << 6);
                    m_receivedAcControlFrame.Front.Right.Direction.Auto = receivedByte & (1 << 7);
                    break;
                }
                case 1: {
                    m_receivedAcControlFrame.Rear.Left.Direction.Up = receivedByte & 1;
                    m_receivedAcControlFrame.Rear.Left.Direction.Center = receivedByte & (1 << 1);
                    m_receivedAcControlFrame.Rear.Left.Direction.Down = receivedByte & (1 << 2);
                    m_receivedAcControlFrame.Rear.Left.Direction.Auto = receivedByte & (1 << 3);
                    m_receivedAcControlFrame.Rear.Right.Direction.Up = receivedByte & (1 << 4);
                    m_receivedAcControlFrame.Rear.Right.Direction.Center = receivedByte & (1 << 5);
                    m_receivedAcControlFrame.Rear.Right.Direction.Down = receivedByte & (1 << 6);
                    m_receivedAcControlFrame.Rear.Right.Direction.Auto = receivedByte & (1 << 7);
                    break;
                }
                case 2: {
                    m_receivedAcControlFrame.Front.Left.Fan = receivedByte;
                } break;
                case 3: {
                    m_receivedAcControlFrame.Front.Right.Fan = receivedByte;
                } break;
                case 4: {
                    m_receivedAcControlFrame.Rear.Left.Fan = receivedByte;
                } break;
                case 5: {
                    m_receivedAcControlFrame.Rear.Right.Fan = receivedByte;
                } break;
                case 6: {
                    m_receivedAcControlFrame.Front.Left.Temperature = receivedByte;
                } break;
                case 7: {
                    m_receivedAcControlFrame.Front.Right.Temperature = receivedByte;
                } break;
                case 8: {
                    m_receivedAcControlFrame.Rear.Left.Temperature = receivedByte;
                } break;
                case 9: {
                    m_receivedAcControlFrame.Rear.Right.Temperature = receivedByte;
                } break;
                case 10: {
                    m_receivedAcControlFrame.Front.Left.SeatHeating = receivedByte;
                } break;
                case 11: {
                    m_receivedAcControlFrame.Front.Right.SeatHeating = receivedByte;
                } break;
                case 12: {
                    m_receivedAcControlFrame.Rear.Left.SeatHeating = receivedByte;
                } break;
                case 13: {
                    m_receivedAcControlFrame.Rear.Right.SeatHeating = receivedByte;
                } break;
                case 14: {
                    m_receivedAcControlFrame.TempSelectLeft = receivedByte & 1;
                    m_receivedAcControlFrame.TempSelectRight = receivedByte & (1 << 1);
                    m_receivedAcControlFrame.FanSelectLeft = receivedByte & (1 << 2);
                    m_receivedAcControlFrame.FanSelectRight = receivedByte & (1 << 3);
                    m_receivedAcControlFrame.ProgAuto = receivedByte & (1 << 4);
                    m_receivedAcControlFrame.ProgAutoFanFront = receivedByte & (1 << 5);
                    m_receivedAcControlFrame.ProgAutoFanRear = receivedByte & (1 << 6);
                    m_receivedAcControlFrame.ProgWindscreen = receivedByte & (1 << 7);
                    break;
                }
                case 15: {
                    m_receivedAcControlFrame.Recirculate = receivedByte & 1;
                    m_receivedAcControlFrame.RearDefrost = receivedByte & (1 << 1);
                    m_receivedAcControlFrame.AC = receivedByte & (1 << 2);
                    m_receiverState = CrcBuffer;
                    break;
                }
                }
            } break;
            case CustomCommand: {
                switch (m_dataBufferIndex) {
                case 0: {
                    m_receivedCustomCommandFrame.Bits[0] = receivedByte & 1;
                    m_receivedCustomCommandFrame.Bits[1] = receivedByte & (1 << 1);
                    m_receivedCustomCommandFrame.Bits[2] = receivedByte & (1 << 2);
                    m_receivedCustomCommandFrame.Bits[3] = receivedByte & (1 << 3);
                    m_receivedCustomCommandFrame.Bits[4] = receivedByte & (1 << 4);
                    m_receivedCustomCommandFrame.Bits[5] = receivedByte & (1 << 5);
                    m_receivedCustomCommandFrame.Bits[6] = receivedByte & (1 << 6);
                    m_receivedCustomCommandFrame.Bits[7] = receivedByte & (1 << 7);
                    break;
                }
                case 1: {
                    m_receivedCustomCommandFrame.Bits[8] = receivedByte & 1;
                    m_receivedCustomCommandFrame.Bits[9] = receivedByte & (1 << 1);
                    m_receivedCustomCommandFrame.Bits[10] = receivedByte & (1 << 2);
                    m_receivedCustomCommandFrame.Bits[11] = receivedByte & (1 << 3);
                    m_receivedCustomCommandFrame.Bits[12] = receivedByte & (1 << 4);
                    m_receivedCustomCommandFrame.Bits[13] = receivedByte & (1 << 5);
                    m_receivedCustomCommandFrame.Bits[14] = receivedByte & (1 << 6);
                    m_receivedCustomCommandFrame.Bits[15] = receivedByte & (1 << 7);
                    break;
                }
                case 2: {
                    m_receivedCustomCommandFrame.Bytes[0] = receivedByte;
                    break;
                }
                case 3: {
                    m_receivedCustomCommandFrame.Bytes[1] = receivedByte;
                    break;
                }
                case 4: {
                    m_receivedCustomCommandFrame.Bytes[2] = receivedByte;
                    break;
                }
                case 5: {
                    m_receivedCustomCommandFrame.Bytes[3] = receivedByte;
                    break;
                }
                case 6: {
                    m_receivedCustomCommandFrame.Bytes[4] = receivedByte;
                    break;
                }
                case 7: {
                    m_receivedCustomCommandFrame.Bytes[5] = receivedByte;
                    m_receiverState = CrcBuffer;
                    break;
                }
                }
            } break;
            case BodyControlCommand: {
                switch (m_dataBufferIndex) {
                case 0: {
                    m_receivedBodyControlCommandFrame.IndicatorLeft = receivedByte & 1;
                    m_receivedBodyControlCommandFrame.IndicatorRight = receivedByte & (1 << 1);
                    m_receivedBodyControlCommandFrame.Braking = receivedByte & (1 << 2);
                    m_receivedBodyControlCommandFrame.Reversing = receivedByte & (1 << 3);
                    m_receivedBodyControlCommandFrame.HandBrake = receivedByte & (1 << 4);
                    m_receivedBodyControlCommandFrame.SeatBelt = receivedByte & (1 << 5);
                    // m_receivedBodyControlCommandFrame.Bits = receivedByte & (1 << 6); // Reserved
                    // m_receivedBodyControlCommandFrame.Bits = receivedByte & (1 << 7); // Reserved
                    break;
                }
                case 1: {
                    m_receivedBodyControlCommandFrame.PassengerSeatOccupied = receivedByte & 1;
                    m_receivedBodyControlCommandFrame.RearLeftOccupied = receivedByte & (1 << 1);
                    m_receivedBodyControlCommandFrame.RearMiddleOccupied = receivedByte & (1 << 2);
                    m_receivedBodyControlCommandFrame.RearRightOccupied = receivedByte & (1 << 3);
                    m_receivedBodyControlCommandFrame.PassengerSeatBelt = receivedByte & (1 << 4);
                    m_receivedBodyControlCommandFrame.RearLeftSeatBelt = receivedByte & (1 << 5);
                    m_receivedBodyControlCommandFrame.RearMiddleSeatBelt = receivedByte & (1 << 6);
                    m_receivedBodyControlCommandFrame.RearRightSeatBelt = receivedByte & (1 << 7);
                    break;
                }
                case 2: {
                    m_receivedBodyControlCommandFrame.DashBrightness = receivedByte;
                    break;
                }
                case 3: {
                    m_receivedBodyControlCommandFrame.ExternalBrightness = receivedByte;
                    break;
                }
                }
            } break;
            case DriveTrainControlCommand: {
                switch (m_dataBufferIndex) {
                case 0: {
                    m_receivedDriveTrainControlCommandFrame.speed = receivedByte;
                    break;
                }
                case 1: {
                    m_receivedDriveTrainControlCommandFrame.speed |= receivedByte << 8;
                    break;
                }
                case 2: {
                    m_receivedDriveTrainControlCommandFrame.engineRpm = receivedByte;
                    break;
                }
                case 3: {
                    m_receivedDriveTrainControlCommandFrame.engineRpm |= receivedByte << 8;
                    break;
                }
                case 4: {
                    m_receivedDriveTrainControlCommandFrame.frontLeftWheelSpeed = receivedByte;
                    break;
                }
                case 5: {
                    m_receivedDriveTrainControlCommandFrame.frontLeftWheelSpeed |= receivedByte << 8;
                    break;
                }
                case 6: {
                    m_receivedDriveTrainControlCommandFrame.frontRightWheelSpeed = receivedByte;
                    break;
                }
                case 7: {
                    m_receivedDriveTrainControlCommandFrame.frontRightWheelSpeed |= receivedByte << 8;
                    break;
                }
                case 8: {
                    m_receivedDriveTrainControlCommandFrame.rearLeftWheelSpeed = receivedByte;
                    break;
                }
                case 9: {
                    m_receivedDriveTrainControlCommandFrame.rearLeftWheelSpeed |= receivedByte << 8;
                    break;
                }
                case 10: {
                    m_receivedDriveTrainControlCommandFrame.rearRightWheelSpeed = receivedByte;
                    break;
                }
                case 11: {
                    m_receivedDriveTrainControlCommandFrame.rearRightWheelSpeed |= receivedByte << 8;
                    break;
                }
                }
            } break;
            case ButtonInputCommand: {
                m_callbacks->ButtonInputCommandCallback((Keys)receivedByte);
                break;
            }
            default:
                break;
            }

            m_dataBufferIndex++;
        }
        if (m_dataBufferIndex >= m_frameSize) {
            m_receiverState = CrcBuffer;
        }
        break;
    }
    case CrcBuffer: {
        switch (m_crcIndex++) {
        case 0: {
            m_crc = ((uint8_t)receivedByte) << 8;
        } break;
        case 1: {
            m_crc = ((uint8_t)receivedByte) | m_crc;
            m_receiverState = CommandByte;
            uint16_t crc = HAL::calculateCRC16(m_receivedBuffer, m_dataBufferIndex);
            
            if (crc == m_crc) {
                switch (m_currentCommand) {
                case ClimateControlCommand: {
                    m_callbacks->ClimateControlCallback(m_receivedAcControlFrame);
                    sendAcknowledge(ClimateControlCommand);
                } break;
                case CustomCommand: {
                    m_callbacks->CustomCommandCallback(m_receivedCustomCommandFrame);
                    sendAcknowledge(CustomCommand);
                } break;
                case DebugMessageCommand: {
                    m_callbacks->PrintString(m_receivedBuffer, m_dataBufferIndex);
                } break;
                case UpdateRequest: {
                    sendClimateControlCommand();
                    sendCustomCommand();
                    sendBodyControlCommand();
                    sendDriveTrainControlCommand();
                    sendDebugMessageCommand("----------------------------");
                    sendDebugMessageCommand("HUD Serial 1.0");
                    sendDebugMessageCommand("by viktorgino");
                    sendDebugMessageCommand("build id: #00001 VOLVO P1 v1");
                    sendDebugMessageCommand("----------------------------");
                } break;
                case BodyControlCommand: {
                    m_callbacks->BodyControlCommandCallback(m_receivedBodyControlCommandFrame);
                    sendAcknowledge(BodyControlCommand);
                } break;
                case DriveTrainControlCommand: {
                    m_callbacks->DriveTrainControlCommandCallback(m_receivedDriveTrainControlCommandFrame);
                    sendAcknowledge(DriveTrainControlCommand);
                } break;
                default:
                    break;
                }
            }
        }
        default: {
        } break;
        }

    } break;
    default:
        break;
    }
}

void HUDSerial::sendMessage(CommandTypes messageType, uint8_t length, char *message) {
    uint16_t crc = HAL::calculateCRC16(message, length);
    uint8_t crcMSB = crc >> 8;
    uint8_t crcLSB = crc & 0b0000000011111111;

    m_callbacks->SendMessageCallback(messageType);
    m_callbacks->SendMessageCallback(length);
    for (int i = 0; i < length; i++) {
        m_callbacks->SendMessageCallback(message[i]);
    }

    m_callbacks->SendMessageCallback(crcMSB);
    m_callbacks->SendMessageCallback(crcLSB);
}

void HUDSerial::sendUpdateRequest() { sendMessage(UpdateRequest, 0, nullptr); }

void HUDSerial::sendAcknowledge(CommandTypes command) {
    char buf[1];
    switch (command) {
    case ClimateControlCommand:
    case CustomCommand:
    case DriveTrainControlCommand:
    case BodyControlCommand:
        buf[0] = command;
        sendMessage(Acknowledge, 1, buf);
        break;
    default:
        return;
    }
}

void HUDSerial::sendClimateControlCommand(const ClimateControlCommandFrame &controlFrame) {
    memcpy(&m_acControlFrame, &controlFrame, sizeof(ClimateControlCommandFrame));

    sendClimateControlCommand();
}

void HUDSerial::sendButtonInputCommand(const Keys key) {
    char buffer[1];
    buffer[0] = key;

    sendMessage(ButtonInputCommand, 1, buffer);
}

void HUDSerial::sendDebugMessageCommand(const char *message) {
    int len = strlen(message);
    sendMessage(DebugMessageCommand, len, (char *)message);
}

void HUDSerial::sendCustomCommand(const CustomCommandFrame &commandFrame) {
    memcpy(&m_customCommandFrame, &commandFrame, sizeof(CustomCommandFrame));

    sendCustomCommand();
}

void HUDSerial::sendBodyControlCommand(const BodyControlCommandFrame &controlFrame) {
    memcpy(&m_bodyControlCommandFrame, &controlFrame, sizeof(BodyControlCommandFrame));

    sendBodyControlCommand();
}
void HUDSerial::sendDriveTrainControlCommand(const DriveTrainControlCommandFrame &controlFrame) {
    memcpy(&m_driveTrainControlCommandFrame, &controlFrame, sizeof(DriveTrainControlCommandFrame));

    sendDriveTrainControlCommand();
}

void HUDSerial::sendClimateControlCommand() {
    char ACControlBuffer[16];
    ACControlBuffer[0] =
        m_acControlFrame.Front.Left.Direction.Up | m_acControlFrame.Front.Left.Direction.Center << 1 |
        m_acControlFrame.Front.Left.Direction.Down << 2 | m_acControlFrame.Front.Left.Direction.Auto << 3 |
        m_acControlFrame.Front.Right.Direction.Up << 4 | m_acControlFrame.Front.Right.Direction.Center << 5 |
        m_acControlFrame.Front.Right.Direction.Down << 6 | m_acControlFrame.Front.Right.Direction.Auto << 7;
    ACControlBuffer[1] =
        m_acControlFrame.Rear.Left.Direction.Up | m_acControlFrame.Rear.Left.Direction.Center << 1 |
        m_acControlFrame.Rear.Left.Direction.Down << 2 | m_acControlFrame.Rear.Left.Direction.Auto << 3 |
        m_acControlFrame.Rear.Right.Direction.Up << 4 | m_acControlFrame.Rear.Right.Direction.Center << 5 |
        m_acControlFrame.Rear.Right.Direction.Down << 6 | m_acControlFrame.Rear.Right.Direction.Auto << 7;
    ACControlBuffer[2] = m_acControlFrame.Front.Left.Fan;
    ACControlBuffer[3] = m_acControlFrame.Front.Right.Fan;
    ACControlBuffer[4] = m_acControlFrame.Rear.Left.Fan;
    ACControlBuffer[5] = m_acControlFrame.Rear.Right.Fan;
    ACControlBuffer[6] = m_acControlFrame.Front.Left.Temperature;
    ACControlBuffer[7] = m_acControlFrame.Front.Right.Temperature;
    ACControlBuffer[8] = m_acControlFrame.Rear.Left.Temperature;
    ACControlBuffer[9] = m_acControlFrame.Rear.Right.Temperature;
    ACControlBuffer[10] = m_acControlFrame.Front.Left.SeatHeating;
    ACControlBuffer[11] = m_acControlFrame.Front.Right.SeatHeating;
    ACControlBuffer[12] = m_acControlFrame.Rear.Left.SeatHeating;
    ACControlBuffer[13] = m_acControlFrame.Rear.Right.SeatHeating;
    ACControlBuffer[14] = m_acControlFrame.TempSelectLeft | m_acControlFrame.TempSelectRight << 1 |
                          m_acControlFrame.FanSelectLeft << 2 | m_acControlFrame.FanSelectRight << 3 |
                          m_acControlFrame.ProgAuto << 4 | m_acControlFrame.ProgAutoFanFront << 5 |
                          m_acControlFrame.ProgAutoFanRear << 6 | m_acControlFrame.ProgWindscreen << 7;
    ACControlBuffer[15] = m_acControlFrame.Recirculate | m_acControlFrame.RearDefrost << 1 | m_acControlFrame.AC << 2;

    sendMessage(ClimateControlCommand, 16, ACControlBuffer);
    m_climateControlCommandAck = true;
}

void HUDSerial::sendCustomCommand() {
    char CustomCommandBuffer[8];
    CustomCommandBuffer[0] = m_customCommandFrame.Bits[0] | m_customCommandFrame.Bits[1] << 1 |
                             m_customCommandFrame.Bits[2] << 2 | m_customCommandFrame.Bits[3] << 3 |
                             m_customCommandFrame.Bits[4] << 4 | m_customCommandFrame.Bits[5] << 5 |
                             m_customCommandFrame.Bits[6] << 6 | m_customCommandFrame.Bits[7] << 7;
    CustomCommandBuffer[1] = m_customCommandFrame.Bits[8] | m_customCommandFrame.Bits[9] << 1 |
                             m_customCommandFrame.Bits[10] << 2 | m_customCommandFrame.Bits[11] << 3 |
                             m_customCommandFrame.Bits[12] << 4 | m_customCommandFrame.Bits[13] << 5 |
                             m_customCommandFrame.Bits[14] << 6 | m_customCommandFrame.Bits[15] << 7;
    CustomCommandBuffer[2] = m_customCommandFrame.Bytes[0];
    CustomCommandBuffer[3] = m_customCommandFrame.Bytes[1];
    CustomCommandBuffer[4] = m_customCommandFrame.Bytes[2];
    CustomCommandBuffer[5] = m_customCommandFrame.Bytes[3];
    CustomCommandBuffer[6] = m_customCommandFrame.Bytes[4];
    CustomCommandBuffer[7] = m_customCommandFrame.Bytes[5];

    sendMessage(CustomCommand, 8, CustomCommandBuffer);
    m_customCommandAck = true;
}

void HUDSerial::sendBodyControlCommand() {
    char BodyControlCommandBuffer[4];
    BodyControlCommandBuffer[0] = m_bodyControlCommandFrame.IndicatorLeft |
                                  m_bodyControlCommandFrame.IndicatorRight << 1 |
                                  m_bodyControlCommandFrame.Braking << 2 | m_bodyControlCommandFrame.Reversing << 3 |
                                  m_bodyControlCommandFrame.HandBrake << 4 | m_bodyControlCommandFrame.SeatBelt << 5;
    BodyControlCommandBuffer[1] =
        m_bodyControlCommandFrame.PassengerSeatOccupied | m_bodyControlCommandFrame.RearLeftOccupied << 1 |
        m_bodyControlCommandFrame.RearMiddleOccupied << 2 | m_bodyControlCommandFrame.RearRightOccupied << 3 |
        m_bodyControlCommandFrame.PassengerSeatBelt << 4 | m_bodyControlCommandFrame.RearLeftSeatBelt << 5 |
        m_bodyControlCommandFrame.RearMiddleSeatBelt << 6 | m_bodyControlCommandFrame.RearRightSeatBelt << 7;
    BodyControlCommandBuffer[2] = m_bodyControlCommandFrame.DashBrightness;
    BodyControlCommandBuffer[3] = m_bodyControlCommandFrame.ExternalBrightness;
    sendMessage(BodyControlCommand, 4, BodyControlCommandBuffer);
    m_bodyControlCommandAck = true;
}

void HUDSerial::sendDriveTrainControlCommand() {
    char BodyControlCommandBuffer[12];

    BodyControlCommandBuffer[0] = m_driveTrainControlCommandFrame.speed & 0x00FF;
    BodyControlCommandBuffer[1] = (m_driveTrainControlCommandFrame.speed & 0XFF00) >> 8;

    BodyControlCommandBuffer[2] = m_driveTrainControlCommandFrame.engineRpm & 0x00FF;
    BodyControlCommandBuffer[3] = (m_driveTrainControlCommandFrame.engineRpm & 0XFF00) >> 8;

    BodyControlCommandBuffer[4] = m_driveTrainControlCommandFrame.frontLeftWheelSpeed & 0x00FF;
    BodyControlCommandBuffer[5] = (m_driveTrainControlCommandFrame.frontLeftWheelSpeed & 0XFF00) >> 8;

    BodyControlCommandBuffer[6] = m_driveTrainControlCommandFrame.frontRightWheelSpeed & 0x00FF;
    BodyControlCommandBuffer[7] = (m_driveTrainControlCommandFrame.frontRightWheelSpeed & 0XFF00) >> 8;

    BodyControlCommandBuffer[8] = m_driveTrainControlCommandFrame.rearLeftWheelSpeed & 0x00FF;
    BodyControlCommandBuffer[9] = (m_driveTrainControlCommandFrame.rearLeftWheelSpeed & 0XFF00) >> 8;

    BodyControlCommandBuffer[10] = m_driveTrainControlCommandFrame.rearRightWheelSpeed & 0x00FF;
    BodyControlCommandBuffer[11] = (m_driveTrainControlCommandFrame.rearRightWheelSpeed & 0XFF00) >> 8;

    sendMessage(DriveTrainControlCommand, 12, BodyControlCommandBuffer);
    m_driveTrainControlCommandAck = true;
}

void HUDSerial::loop() {
    switch (timerCount++) {
    case 0:
        if (m_climateControlCommandAck) {
            sendClimateControlCommand();
        }
        break;
    case 50:
        if (m_customCommandAck) {
            sendCustomCommand();
        }
        break;
    case 100:
        if (m_driveTrainControlCommandAck) {
            sendDriveTrainControlCommand();
        }
        break;
    case 150:
        if (m_bodyControlCommandAck) {
            sendBodyControlCommand();
        }
        break;
    default:
        break;
    }
    if (timerCount >= 200) {
        timerCount = 0;
    }
}
}  // namespace HUDSerial
