#include <Arduino.h>
#include "version.h"
#include "main.h"
#include "led.h"
#include "magnets.h"
#include "SerialManager.h"

LED led;
Magnets magnets;
SerialManager sm;

// Globals
bool enabled               = true;
unsigned long previousTime = 0;
int stateChangeInterval    = 12000; // how much to wait between state chang (ms)
int stateIndex             = 0;
int numberOfStates         = sizeof(magnets.state) / sizeof(bool);

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(10);

  Serial.println("Landlord compass device by kevinc...");
  Serial.println(getFullVersion("landlord-tnt-compass"));
  
  led.setup();
  magnets.setup();

  sm.registerCommand(SerialCommand("status",  's', &status,   "gets the status of device"));
  sm.registerCommand(SerialCommand("enable",  'e', &enable,   "turn on magnets and lights.  will take off from where it was when disabled."));
  sm.registerCommand(SerialCommand("disable", 'd', &disable,  "turn off magnets and lights"));
  sm.registerCommand(SerialCommand("reboot",  'r', &reboot,   "software reboot the device"));

  sm.printHelp();

  status();
}

void loop() {
  sm.handle();

  // figure out if we need to change state
  if (enabled) {
    unsigned long currentTime = millis();
    if (currentTime - previousTime > stateChangeInterval) {
      previousTime = currentTime;
    
      setState(stateIndex);

      // move the index for the next round
      stateIndex = (stateIndex == numberOfStates - 1) ? 0 : stateIndex + 1;
    }
  }
  
  led.handle();
  magnets.handle();
}

void status() {
  char cMsg[254];

  sprintf(cMsg, 
    "status="
      "version:%s,"
      "gitDate:%s,"
      "buildDate:%s,"

      "enabled:%s,"

      "\r\n"
    , GIT_HASH,
      GIT_DATE,
      DATE_NOW,
      enabled ? "true" : "false"
);

  sm.print(cMsg);
}

void disable() {
  Serial.println("Disabling device...");
  enabled = false;
  setState(-1);
  status();
}

void enable() {
  Serial.println("Enabling device...");
  
  // NOTE: intentionally not resetting state, it will take over from where it left off last
  enabled = true;

  status();
}

void reboot() {
  Serial.println("Rebooting...");
  soft_restart();
}

void setState(int index) {
  for(int i=0; i < numberOfStates; i++) {
    led.state[i] = i == index;
    magnets.state[i] = i == index;
  }
}
