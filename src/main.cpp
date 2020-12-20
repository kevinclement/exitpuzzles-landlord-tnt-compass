#include <Arduino.h>

#include <Wire.h>
#include <SD.h>      // need to include the SD library
#include <TMRpcm.h>  // also need to include this library...

// PIN Locations
#define MAG1       2  // mag1 (red)
#define MAG2       3  // mag2 (green)
#define MAG3       4  // mag3 (blue)
#define MAG4       5  // mag4 (yellow)
#define LED1       6  // LED1 (red)
#define LED2       7  // LED2 (green)
#define LED3       8  // LED3 (blue)
#define LED4       14 // LED4 (yellow)

#define SD_PIN     10 // CS PIN for SD reader
#define SPEAKER    9  // PIN for speaker

// Globals
bool enabled               = true;
unsigned long previousTime = 0;
int stateChangeInterval    = 12000; // how much to wait between state chang (ms)
bool ledState[]            = {false, false, false, false};
bool magState[]            = {false, false, false, false};  // prob only need one array
int stateIndex             = 0;
int numberOfStates         = sizeof(ledState) / sizeof(bool);

TMRpcm audio;   // create an object for use in this sketch
char mychar;

void setup() {
  setupSerial();
  setupLed();
  setupMags();
  setupCommunication();
  setupSpeaker();
  
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

void setupCommunication() {
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onReceive(receiveEvent); // register event
}

void beep(unsigned char speakerPin, int frequencyInHertz, long timeInMilliseconds)     // the sound producing function  
{
  int x;   
  long delayAmount = (long)(1000000/frequencyInHertz); 
  long loopTime = (long)((timeInMilliseconds*1000)/(delayAmount*2)); 
  for (x=0;x<loopTime;x++)   
  {   
      digitalWrite(SPEAKER,HIGH); 
      delayMicroseconds(delayAmount); 
      digitalWrite(SPEAKER,LOW); 
      delayMicroseconds(delayAmount); 
  }
}

void uhoh() {
  beep(SPEAKER,415,100);
  delay(80);
  beep(SPEAKER,279,100);
  delay(80);
}

void setupSpeaker() {
  audio.speakerPin = SPEAKER;
  audio.setVolume(5);
  audio.quality(0); 

  pinMode(SPEAKER, OUTPUT);
  digitalWrite(SPEAKER, LOW); //Buzzer Off 

  // TMP: take out before shipping
  uhoh();
  
  if (!SD.begin(SD_PIN)) {  // see if the card is present and can be initialized:
    Serial.println("SD fail");  
    return;   // don't do anything more if not
  }
  else {
    Serial.println("SD init");
  }

}

// function that executes whenever data is received from master
void receiveEvent(int numBytes) {
  // assumes only one byte sent
  int x = Wire.read();

  // value indicates behavior
  switch(x) {
    // bad
    case 0: 
      audio.play("AhAhAh4.wav");
      break;

    // fail
    case 1:
      audio.play("e1m166.wav");
      enabled = false;
      break;

    // win
    case 2:
      audio.play("sd10.wav");
      enabled = false;
      break;

    // reset/start
    case 3: 
      enabled = true;
      break;
  
  }
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
        //audio.play("1.wav");
        //audio.play("e1m166.wav");
        //audio.play("sd10.wav");
        
        break;
      case 'b':
        Serial.println("serial: b");
        audio.play("2.wav");
        break;
      case 'u':
        Serial.println("serial: uhoh");
        uhoh();
        break;

      case 'v':
        Serial.println("serial: volume");
        audio.volume(1);
        break;

      case 'j':
        Serial.println("serial: jurassic");
        audio.play("AhAhAh.wav");
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