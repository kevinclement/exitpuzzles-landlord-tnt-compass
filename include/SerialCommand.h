#pragma once
#include "Arduino.h"

class SerialCommand {
  public:
    SerialCommand();
    SerialCommand(String command, char sCommand, void (*cb)(), String desc);
    String command;
    char sCommand;
    void (*cb)(int);
    String desc;

  private:
};

