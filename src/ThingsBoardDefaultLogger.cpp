// Header include.
#include <ThingsBoardDefaultLogger.h>

// Library include.
#include <Arduino.h>
#include <HardwareSerial.h>

void ThingsBoardDefaultLogger::log(const char *msg) {
#if THINGSBOARD_ENABLE_PROGMEM
  Serial.print(F("[TB] "));
#else
  Serial.print("[TB] ");
#endif // THINGSBOARD_ENABLE_PROGMEM
  Serial.println(msg);
}
