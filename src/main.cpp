#include <Arduino.h>
#include "version.h"
#include "led.h"
#include "magnets.h"
#include "SerialManager.h"

// TODO: Add back serial code to enable/disable from usb

LED led;
Magnets magnets;
SerialManager sm;

// Globals
bool enabled               = true;
unsigned long previousTime = 0;
int stateChangeInterval    = 12000; // how much to wait between state chang (ms)
int stateIndex             = 0;
int numberOfStates         = sizeof(magnets.state) / sizeof(bool);

void status() {
  Serial.println("status called");
}

void disable() {
  Serial.println("disable called");
}

void debug() {
  Serial.println("debug called");
}

void reboot() {
  Serial.println("reboot called");
}

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(10);

  Serial.println("Landlord compass device by kevinc...");
  Serial.println(getFullVersion("landlord-tnt-compass"));
  
  led.setup();
  magnets.setup();

  sm.registerCommand(SerialCommand("status",  's', &status,   "gets the status of device"));
  sm.registerCommand(SerialCommand("disable", 'd', &disable,  "turn off magnets and lights.  reboot to re-enable."));
  sm.registerCommand(SerialCommand("debug",   'x', &debug,    "toggle debug of device"));
  sm.registerCommand(SerialCommand("reboot",  'r', &reboot, "software reboot the device"));

  sm.printHelp();

  // TODO:
  //status();
}

void loop() {
  sm.handle();

  if (!enabled) {
    return;
  }

  // figure out if we need to change state
  unsigned long currentTime = millis();
  if (currentTime - previousTime > stateChangeInterval) {
    previousTime = currentTime;
   
    for(int i=0; i < numberOfStates; i++) {
      led.state[i] = i == stateIndex;
      magnets.state[i] = i == stateIndex;
    }

    // move the index for the next round
    stateIndex = (stateIndex == numberOfStates - 1) ? 0 : stateIndex + 1;
  }
  
  led.handle();
  magnets.handle();
}

// void serialLoop() {
//   // check if we need to read from serial
//   if (Serial.available()) {
//     char ch = Serial.read();

//     switch (ch) {

//       case 'a':
//         Serial.println("serial: a");        
//         break;      
//       case 'x':
//         Serial.println("serial: exit");
//         enabled = false;
//         break;
      
//       default:        
//       break;
//     }
//   }
// }