#pragma once

#include "Arduino.h"

#define LED1_PIN 8  // LED1 (red)
#define LED2_PIN 9  // LED2 (green)
#define LED3_PIN 10 // LED3 (blue)
#define LED4_PIN 11 // LED4 (yellow)

class LED {
  public:
    LED();
    void setup();
    void handle();

    bool state[4] = {false};

  private:
};