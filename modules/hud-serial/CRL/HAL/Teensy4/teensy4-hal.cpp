
#ifdef __IMXRT1062__

#include "teensy4-hal.h"
#include <FastCRC.h>
FastCRC16 CRC16;
namespace HAL
{

void print(char *buffer){

}

uint16_t calculateCRC16(char *buffer, int length){
    return CRC16.ccitt((uint8_t*) buffer, length);    
}
    
} // namespace HAL

#endif