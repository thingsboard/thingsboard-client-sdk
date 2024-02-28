#ifndef Espressif_Updater_h
#define Espressif_Updater_h

// Local include.
#include "Configuration.h"

#if THINGSBOARD_ENABLE_OTA && THINGSBOARD_USE_ESP_PARTITION

// Local include.
#include "IUpdater.h"


/// @brief IUpdater implementation that uses the Over the Air Update API from Espressif (https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/ota.html)
/// under the hood to write the given binary firmware data into flash memory so we can restart with newly received firmware
class Espressif_Updater : public IUpdater {
  public:
    Espressif_Updater() = default;

    bool begin(size_t const & firmware_size) override;
  
    size_t write(uint8_t * payload, size_t const & total_bytes) override;

    void reset() override;
  
    bool end() override;

  private:
    uint32_t m_ota_handle;           // ESP OTA hanle that is used to to access the underlying updater
    void const * m_update_partition; // Non active OTA partition that we write our data into
};

#endif // THINGSBOARD_ENABLE_OTA && THINGSBOARD_USE_ESP_PARTITION

#endif // Espressif_Updater_h
