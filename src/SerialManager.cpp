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
  commands[cmdIndex++] = cmd;

  // check for duplicate command or short command
  for (int i=0; i<cmdIndex-1; i++) {
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

  unsigned int cmdLen = cmd.args.length() + 1;
  if (cmdLen > longestCmd) {
    longestCmd = cmdLen;
  }
}

void SerialManager::print(const char *fmt, ...) {
  char buf[256];     // resulting string limited to 128 chars
  va_list args;
  va_start(args, fmt);
  vsnprintf(buf, 256, fmt, args);
  va_end(args);

  // print to serial
  Serial.print(buf);
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
  int value = -1;

  // check if we need to split on space for advance commands
  for (int i = 0; i <= msg.length(); i++) {
    if (msg.charAt(i) == ' ') {
      command = msg.substring(0, i);
      value = msg.substring(i+1, msg.length()).toInt();
    }
  }

  bool foundMatch = false;
  bool help = false;
  for (int i=0; i<cmdIndex; i++) {
    if (command == commands[i].command || (command.length() == 1 && command[0] == commands[i].sCommand)) {
      foundMatch = true;
      commands[i].cb(value);
    } else if (command == "help" || command == "?") {
      foundMatch = help = true;
    }
  }

  if (!foundMatch) {
    int str_len = command.length() + 1; 
    char char_array[str_len];
    command.toCharArray(char_array, str_len);
    print("unknown command: %s%s", char_array, CRLF);
  }

  if (help) {
    printHelp();
  }
}

void SerialManager::printHelp() {
  Serial.println("Available commands:");

  for (int i=0; i<cmdIndex; i++) {
    Serial.print("  ");
    Serial.print(commands[i].sCommand);
    Serial.print(", ");
    Serial.print(commands[i].args);
    for(int j=commands[i].args.length(); j<longestCmd; j++) {
      Serial.print(" ");
    }

    Serial.print("- ");
    Serial.println(commands[i].desc);
  }
}