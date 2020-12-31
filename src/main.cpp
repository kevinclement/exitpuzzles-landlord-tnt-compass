#include <Arduino.h>
#include "version.h"
#include "led.h"
#include "magnets.h"

// TODO: Add back serial code to enable/disable from usb

// NOTE: last time I did rewire I didn't study port mapping well enough
// so its not purely sequential, mag 3 and 4 I had to swap to map properly
//
#define MAG1       4  // red    | South/SouthWest | Password "1" 
#define MAG2       5  // green  | West            | Password "2" 
#define MAG3       7  // blue   | North           | Password "8"
#define MAG4       6  // yellow | East            | Password "4"

LED led;

// Globals
bool enabled               = true;
unsigned long previousTime = 0;
int stateChangeInterval    = 12000; // how much to wait between state chang (ms)
bool magState[]            = {false, false, false, false};  // prob only need one array
int stateIndex             = 0;
int numberOfStates         = sizeof(magState) / sizeof(bool);

char mychar;

// TODO: move to header
void setupMags();
void magLoop();
void serialLoop();

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(10);

  Serial.println("Landlord compass device by kevinc...");
  Serial.println(getFullVersion("landlord-tnt-compass"));
  
  led.setup();
  setupMags();

  // TODO:
  //status();
}

void setupMags() {  
  pinMode(MAG1, OUTPUT);
  pinMode(MAG2, OUTPUT);
  pinMode(MAG3, OUTPUT);
  pinMode(MAG4, OUTPUT);
}

void loop() {

  if (!enabled) {
    return;
  }

  // figure out if we need to change state
  unsigned long currentTime = millis();
  if (currentTime - previousTime > stateChangeInterval) {
    previousTime = currentTime;
   
    for(int i=0; i < numberOfStates; i++) {
      led.state[i] = i == stateIndex;
      magState[i] = i == stateIndex;
    }

    // move the index for the next round
    stateIndex = (stateIndex == numberOfStates - 1) ? 0 : stateIndex + 1;
  }
  
  led.handle();
  magLoop();
  serialLoop();
}

void magLoop() {
  
  digitalWrite(MAG1, magState[0]);
  digitalWrite(MAG2, magState[1]);
  digitalWrite(MAG3, magState[2]);
  digitalWrite(MAG4, magState[3]);
}

void serialLoop() {
  // check if we need to read from serial
  if (Serial.available()) {
    char ch = Serial.read();

    switch (ch) {

      case 'a':
        Serial.println("serial: a");        
        break;      
      case 'x':
        Serial.println("serial: exit");
        enabled = false;
        break;
      
      default:        
      break;
    }
  }
}