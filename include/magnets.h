#pragma once

#include "Arduino.h"

// NOTE: last time I did rewire I didn't study port mapping well enough
// so its not purely sequential, mag 3 and 4 I had to swap to map properly
//
#define MAG1_PIN       4  // red    | South/SouthWest | Password "1" 
#define MAG2_PIN       5  // green  | West            | Password "2" 
#define MAG3_PIN       7  // blue   | North           | Password "8"
#define MAG4_PIN       6  // yellow | East            | Password "4"

class Magnets {
  public:
    Magnets();
    void setup();
    void handle();

    bool state[4] = {false};

  private:
};