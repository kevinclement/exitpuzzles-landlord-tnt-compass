#include "Arduino.h"
#include "led.h"

LED::LED()
{  
}

void LED::setup() {
    pinMode(LED1_PIN, OUTPUT);
    pinMode(LED2_PIN, OUTPUT);
    pinMode(LED3_PIN, OUTPUT);  
    pinMode(LED4_PIN, OUTPUT);
}

void LED::handle() {
    digitalWrite(LED1_PIN, state[0]);
    digitalWrite(LED2_PIN, state[1]);
    digitalWrite(LED3_PIN, state[2]);
    digitalWrite(LED4_PIN, state[3]);
}
