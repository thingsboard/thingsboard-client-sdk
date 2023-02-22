// Header include.
#include <ThingsBoardDefaultLogger.h>

// Local includes.
#include "Configuration.h"
#include "Constants.h"

// Library include.
#if THINGSBOARD_ENABLE_PROGMEM
#include <WString.h>
#endif // THINGSBOARD_ENABLE_PROGMEM
#include <HardwareSerial.h>

void ThingsBoardDefaultLogger::log(const char *msg) {
#if THINGSBOARD_ENABLE_PROGMEM
  Serial.print(F("[TB] "));
#else
  Serial.print("[TB] ");
#endif // THINGSBOARD_ENABLE_PROGMEM
  Serial.println(msg);
}
