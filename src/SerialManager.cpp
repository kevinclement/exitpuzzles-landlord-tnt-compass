#include "Arduino.h"
#include "SerialManager.h"

#define CRLF "\r\n"

SerialManager::SerialManager()
{  
}

void SerialManager::setup() {
  Serial.begin(9600);

    // set read timeout to something really low so we don't hang
  Serial.setTimeout(10);

  while (!Serial); // Wait untilSerial is ready 
}
void SerialManager::registerCommand(SerialCommand cmd) {
  commands[cmdIndex] = cmd;
  cmdIndex = cmdIndex + 1;

  // check for duplicate command or short command
  for (unsigned int i=0; i<cmdIndex-1; i++) {
    if (cmd.command == commands[i].command) {
      Serial.print("WARNING: registered two commands with the same command, '");
      Serial.print(cmd.command);
      Serial.println("'");
    }

    if (cmd.sCommand == commands[i].sCommand) {
      Serial.println("\r\n****************");
      Serial.print("WARNING: registered two commands with the same short command, '");
      Serial.print(cmd.sCommand);
      Serial.println("'");
      Serial.println("****************\r\n");
    }
  }

  unsigned int cmdLen = cmd.command.length() + 1;
  if (cmdLen > longestCmd) {
    longestCmd = cmdLen;
  }
}

void SerialManager::handle() {
  if (!Serial.available()) {
    return;
  }

  // read and handle message from serial
  String msg = Serial.readStringUntil('\n');
  Serial.print("got '");
  Serial.print(msg);
  Serial.println("' command");

  String command = msg;
  
  bool foundMatch = false;
  bool help = false;
  for (unsigned int i=0; i<cmdIndex; i++) {
    if (command == commands[i].command || (command.length() == 1 && command[0] == commands[i].sCommand)) {
      foundMatch = true;
      commands[i].cb();
    } else if (command == "help" || command == "?") {
      foundMatch = help = true;
    }
  }

  if (!foundMatch) {
    Serial.print("unknown command: ");
    Serial.println(command);
  }

  if (help) {
    printHelp();
  }
}

void SerialManager::printHelp() {
  for (unsigned int i=0; i<cmdIndex; i++) {
    Serial.print("  ");
    Serial.print(commands[i].sCommand);
    Serial.print(", ");
    Serial.print(commands[i].command);

    for(unsigned int j=commands[i].command.length(); j<longestCmd; j++) {
      Serial.print(" ");
    }

    Serial.print("- ");
    Serial.println(commands[i].desc);
  }
}