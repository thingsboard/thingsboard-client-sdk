#ifndef Arduino_ESP32_Updater_h
#define Arduino_ESP32_Updater_h

// Local include.
#include "Configuration.h"

#if THINGSBOARD_ENABLE_OTA

#if defined(ESP32) && defined(ARDUINO)

// Local include.
#include "IUpdater.h"


/// @brief IUpdater implementation that uses the Arduino UpdaterClass (https://github.com/espressif/arduino-esp32/tree/master/libraries/Update),
/// under the hood to write the given binary firmware data into flash memory so we can restart with newly received firmware
class Arduino_ESP32_Updater : public IUpdater {
  public:
    bool begin(const size_t& firmware_size) override;
  
    size_t write(uint8_t* payload, const size_t& total_bytes) override;

    void reset() override;
  
    bool end() override;
};

#endif // defined(ESP32) && defined(ARDUINO)

#endif // THINGSBOARD_ENABLE_OTA

#endif // Arduino_ESP32_Updater_h
