#include "Arduino.h"
#include "magnets.h"

Magnets::Magnets()
{  
}

void Magnets::setup() {
    pinMode(MAG1_PIN, OUTPUT);
    pinMode(MAG2_PIN, OUTPUT);
    pinMode(MAG3_PIN, OUTPUT);
    pinMode(MAG4_PIN, OUTPUT);
}

void Magnets::handle() {
    digitalWrite(MAG1_PIN, state[0]);
    digitalWrite(MAG2_PIN, state[1]);
    digitalWrite(MAG3_PIN, state[2]);
    digitalWrite(MAG4_PIN, state[3]);
}
