// Header include.
#include <ThingsBoardDefaultLogger.h>

// Library include.
#include <HardwareSerial.h>

void ThingsBoardDefaultLogger::log(const char *msg) {
#if defined(ESP32)
  Serial.print(F("[TB] "));
#else
  Serial.print("[TB] ");
#endif // defined(ESP32)
  Serial.println(msg);
}
