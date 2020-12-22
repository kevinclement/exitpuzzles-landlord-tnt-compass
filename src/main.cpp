#include <Arduino.h>

// Old audio and wire commands:
//  0: penalty/badSwitch: 
//     audio.play("AhAhAh4.wav");
//  1: fail:
//     audio.play("e1m166.wav");
//     enabled = false;
//  2: win: 
//     audio.play("sd10.wav");
//     enabled = false;
//  3: other arduino boots and wire connects
//     enable = true

// TODO: Add back serial code to enable/disable from usb

// NOTE: last time I did rewire I didn't study port mapping well enough
// so its not purely sequential, mag 3 and 4 I had to swap to map properly
//
#define MAG1       4  // red    | South/SouthWest | Password "1" 
#define MAG2       5  // green  | West            | Password "2" 
#define MAG3       7  // blue   | North           | Password "8"
#define MAG4       6  // yellow | East            | Password "4"

#define LED1       8  // LED1 (red)
#define LED2       9  // LED2 (green)
#define LED3       10 // LED3 (blue)
#define LED4       11 // LED4 (yellow)

// Globals
bool enabled               = true;
unsigned long previousTime = 0;
int stateChangeInterval    = 12000; // how much to wait between state chang (ms)
bool ledState[]            = {false, false, false, false};
bool magState[]            = {false, false, false, false};  // prob only need one array
int stateIndex             = 0;
int numberOfStates         = sizeof(ledState) / sizeof(bool);

char mychar;

// TODO: move to header
void setupSerial();
void setupLed();
void setupMags();
void ledLoop();
void magLoop();
void serialLoop();

void setup() {
  setupSerial();
  setupLed();
  setupMags();
}

void setupSerial() {
  Serial.begin(9600);
  while (!Serial); // Wait untilSerial is ready 
}

void setupLed() {
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);  
  pinMode(LED4, OUTPUT);
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
      ledState[i] = i == stateIndex;
      magState[i] = i == stateIndex;
    }

    // move the index for the next round
    stateIndex = (stateIndex == numberOfStates - 1) ? 0 : stateIndex + 1;
  }
  
  ledLoop();
  magLoop();
  serialLoop();
}

void ledLoop() {
  
  digitalWrite(LED1, ledState[0]);
  digitalWrite(LED2, ledState[1]);
  digitalWrite(LED3, ledState[2]);
  digitalWrite(LED4, ledState[3]);
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