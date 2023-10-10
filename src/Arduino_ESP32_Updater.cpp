// Header include.
#include "Arduino_ESP32_Updater.h"

#if THINGSBOARD_ENABLE_OTA

#if defined(ESP32) && defined(ARDUINO)

// Library include.
#include <Update.h>

bool Arduino_ESP32_Updater::begin(const size_t& firmware_size) {
    return Update.begin(firmware_size);
}

size_t Arduino_ESP32_Updater::write(uint8_t* payload, const size_t& total_bytes) {
    return Update.write(payload, total_bytes);
}

void Arduino_ESP32_Updater::reset() {
    Update.abort();
}

bool Arduino_ESP32_Updater::end() {
    return Update.end();
}

#endif // defined(ESP32) && defined(ARDUINO)

#endif // THINGSBOARD_ENABLE_OTA
