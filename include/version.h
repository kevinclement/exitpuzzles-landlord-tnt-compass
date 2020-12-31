#pragma once
#include "Arduino.h"

// just so that intellisense doesn't barf
#ifndef GIT_HASH
  #define GIT_HASH "311"
#endif
#ifndef GIT_DATE
  #define GIT_DATE "1979-12-21 08:41:00"
#endif
#ifndef DATE_NOW
  #define DATE_NOW "1979-12-21 08:42:00"
#endif

const char * git_hash = GIT_HASH;
const char * git_date = GIT_DATE;
const char * date_now = DATE_NOW;

// e.g. museum-hands-detector vb7e8fa7-dirty (2009-11-10 11:09)
String getFullVersion(const char* appName) {
    char data[100];
    sprintf(data, "info: %s v%s (%s)", appName, git_hash, date_now);
    return data;
}
