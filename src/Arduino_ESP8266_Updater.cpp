// Header include.
#include "Arduino_ESP8266_Updater.h"

#if THINGSBOARD_ENABLE_OTA

#if defined(ESP8266) && defined(ARDUINO)

// Library include.
#include <Updater.h>

bool Arduino_ESP8266_Updater::begin(const size_t& firmware_size) {
    return Update.begin(firmware_size);
}

size_t Arduino_ESP8266_Updater::write(uint8_t* payload, const size_t& total_bytes) {
    return Update.write(payload, total_bytes);
}

void Arduino_ESP8266_Updater::reset() {
    // Nothing to do
}

bool Arduino_ESP8266_Updater::end() {
    return Update.end();
}

#endif // defined(ESP8266) && defined(ARDUINO)

#endif // THINGSBOARD_ENABLE_OTA
