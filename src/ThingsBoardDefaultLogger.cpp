// Header include.
#include <ThingsBoardDefaultLogger.h>

// Library include.
#include <HardwareSerial.h>

void ThingsBoardDefaultLogger::log(const char *msg) {
  Serial.print(F("[TB] "));
  Serial.println(msg);
}
