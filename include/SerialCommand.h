#pragma once
#include "Arduino.h"

class SerialCommand {
  public:
    SerialCommand();
    SerialCommand(String command, char sCommand, void (*cb)(int), String args, String desc);
    String command;
    char sCommand;
    void (*cb)(int);
    String args;
    String desc;

  private:
};

