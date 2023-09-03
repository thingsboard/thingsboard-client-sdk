// Header include.
#include "ESP8266_Updater.h"

#if THINGSBOARD_ENABLE_OTA

#ifdef ESP8266

// Library include.
#include <Updater.h>

bool ESP8266_Updater::begin(const size_t& firmware_size) {
    return Update.begin(firmware_size);
}

size_t ESP8266_Updater::write(uint8_t* payload, const size_t& total_bytes) {
    return Update.write(payload, total_bytes);
}

bool ESP8266_Updater::reset() {
    return true;
}

bool ESP8266_Updater::end() {
    return Update.end();
}

#endif // ESP8266

#endif // THINGSBOARD_ENABLE_OTA
