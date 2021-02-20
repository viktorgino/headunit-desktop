#pragma once
#include <stdint.h>
namespace HAL {

void init();
void loop();
void print(const char *buffer);
uint16_t calculateCRC16(char *buffer, int length);
extern bool TimerExpired;
}
