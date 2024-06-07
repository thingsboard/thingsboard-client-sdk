#ifndef SDCard_Updater_h
#define SDCard_Updater_h

// Local include.
#include "Configuration.h"

#if THINGSBOARD_ENABLE_OTA

// Local include.
#include <IUpdater.h>

/// @brief IUpdater implementation that uses the c fopen function (https://cplusplus.com/reference/cstdio/fopen/),
/// under the hood to write the given binary firmware data into a file. Can be used to write the binary into an intermediate SD card instead of directly updating to flash memory.
class SDCard_Updater : public IUpdater {
  public:
    SDCard_Updater(char const * const file_path);

    bool begin(size_t const & firmware_size) override;
  
    size_t write(uint8_t * const payload, size_t const & total_bytes) override;

    void reset() override;
  
    bool end() override;

  private:
    char const * const m_path;
};

#endif // THINGSBOARD_ENABLE_OTA

#endif // SDCard_Updater_h
