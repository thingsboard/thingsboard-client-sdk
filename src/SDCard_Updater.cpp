// Header include.
#include "SDCard_Updater.h"

#if THINGSBOARD_ENABLE_OTA

// Library include.
#include <cstdio>


SDCard_Updater::SDCard_Updater(char const * const file_path)
  : m_path(file_path)
{
    // Nothing to do
}

bool SDCard_Updater::begin(size_t const & firmware_size) {
    FILE* file = fopen(m_path, "w");
    if (file == nullptr) {
        return false;
    }
    fclose(file);
    return true;
}

size_t SDCard_Updater::write(uint8_t * const payload, size_t const & total_bytes) {
    FILE* file = fopen(m_path, "a");
    if (file == nullptr) {
        return 0;
    }
    size_t const bytes_written = fwrite(payload, 1, total_bytes, file);
    fclose(file);
    return bytes_written;
}

void SDCard_Updater::reset() {
    end();
}

bool SDCard_Updater::end() {
    return remove(m_path) == 0;
}

#endif // THINGSBOARD_ENABLE_OTA
