#include "Arduino.h"
#include "SerialCommand.h"

SerialCommand::SerialCommand()
{
}

SerialCommand::SerialCommand(String command, char sCommand, void (*cb)(), String desc) :
  command(command),
  sCommand(sCommand),
  cb(cb),
  desc(desc)
{
}
