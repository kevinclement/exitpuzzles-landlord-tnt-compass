#pragma once
#include "SerialCommand.h"

class SerialManager {
public:
  SerialManager();
  void setup();
  void registerCommand(SerialCommand cmd);
  void handle();
  void printHelp();

private:
  SerialCommand commands[20]; // max 20 for now, i dont like malloc
  unsigned int cmdIndex = 0;
  unsigned int longestCmd = 0;
};

