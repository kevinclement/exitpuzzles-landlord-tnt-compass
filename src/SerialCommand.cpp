#include "Arduino.h"
#include "SerialCommand.h"

SerialCommand::SerialCommand()
{
}

SerialCommand::SerialCommand(String command, char sCommand, void (*cb)(int), String args, String desc) :
  command(command),
  sCommand(sCommand),
  cb(cb),
  args(args),
  desc(desc)
{
}
