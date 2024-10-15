// Header include.
#include "Arduino_ESP8266_Updater.h"

#if defined(ESP8266) && defined(ARDUINO)

// Library include.
#include <Updater.h>

bool Arduino_ESP8266_Updater::begin(size_t const & firmware_size) {
    return Update.begin(firmware_size);
}

size_t Arduino_ESP8266_Updater::write(uint8_t * payload, size_t const & total_bytes) {
    return Update.write(payload, total_bytes);
}

void Arduino_ESP8266_Updater::reset() {
    // Nothing to do
}

bool Arduino_ESP8266_Updater::end() {
    return Update.end();
}

#endif // defined(ESP8266) && defined(ARDUINO)
