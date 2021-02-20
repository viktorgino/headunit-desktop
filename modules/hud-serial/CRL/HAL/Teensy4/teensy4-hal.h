#pragma once

#include <stdint.h>
#include <string.h>
namespace HAL {

void print(char *buffer);
uint16_t calculateCRC16(char *buffer, int length);
}