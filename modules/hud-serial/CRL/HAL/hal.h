#pragma once

#ifdef __AVR__
#include "AVR/avr-hal.h"
#elif defined(__linux) || defined(__WIN32)
#include "x86/x86-hal.h"
#elif defined(__IMXRT1062__) /*T4*/ || defined(__MK64FX512__) /*T35_36*/ || defined(__MK66FX1M0__) /*T35_36*/ || defined(__MK20DX256__) /*T31_32*/
#include "Teensy4/teensy4-hal.h"
#endif
