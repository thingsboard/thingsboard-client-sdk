// Header include.
#include "ThingsBoardDefaultLogger.h"

void ThingsBoardDefaultLogger::log(const char *msg) {
  Serial.print(F("[TB] "));
  Serial.println(msg);
}
