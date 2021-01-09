#pragma once
#include "Arduino.h"
#include <avr/wdt.h>

void status();
void disable();
void enable();
void off();
void reboot();
void setState(int);

// Software Reset
#define soft_restart()      \
do                          \
{                           \
    wdt_enable(WDTO_15MS);  \
    for(;;)                 \
    {                       \
    }                       \
} while(0)
