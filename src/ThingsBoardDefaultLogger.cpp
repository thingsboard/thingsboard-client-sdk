// Header include.
#include <ThingsBoardDefaultLogger.h>

// Library include.
#include <HardwareSerial.h>

// Local includes.
#include "Configuration.h"

void ThingsBoardDefaultLogger::log(const char *msg) {
#if THINGSBOARD_ENABLE_PROGMEM
  Serial.print(F("[TB] "));
#else
  Serial.print("[TB] ");
#endif // THINGSBOARD_ENABLE_PROGMEM
  Serial.println(msg);
}
