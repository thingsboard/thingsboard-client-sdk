// Header include.
#include "Espressif_Updater.h"

#if THINGSBOARD_USE_ESP_PARTITION

// Library include.
#include <esp_ota_ops.h>

bool Espressif_Updater::begin(size_t const & firmware_size) {
    esp_partition_t const * running = esp_ota_get_running_partition();
    esp_partition_t const * configured = esp_ota_get_boot_partition();

    if (configured != running) {
        return false;
    }

    esp_partition_t const * update_partition = esp_ota_get_next_update_partition(nullptr);

    if (update_partition == nullptr) {
        return false;
    }

    // Temporary handle is used, because it allows using a void* as the actual ota_handle,
    // allowing us to only include the esp_ota_ops header in the defintion (.cpp) file,
    // instead of also needing to declare it in the declaration (.h) header file
    esp_ota_handle_t ota_handle;
    esp_err_t const error = esp_ota_begin(update_partition, firmware_size, &ota_handle);

    if (error != ESP_OK) {
        return false;
    }

    m_ota_handle = ota_handle;
    m_update_partition = update_partition;
    return true;
}

size_t Espressif_Updater::write(uint8_t * payload, size_t const & total_bytes) {
    esp_err_t const error = esp_ota_write(m_ota_handle, payload, total_bytes);
    size_t const written_bytes = (error == ESP_OK) ? total_bytes : 0U;
    return written_bytes;
}

void Espressif_Updater::reset() {
#if defined(ESP8266) || (ESP_IDF_VERSION_MAJOR == 4 && ESP_IDF_VERSION_MINOR < 3) || ESP_IDF_VERSION_MAJOR < 4
    (void)end();
#else
    (void)esp_ota_abort(m_ota_handle);
#endif
}

bool Espressif_Updater::end() {
    esp_err_t error = esp_ota_end(m_ota_handle);
    if (error != ESP_OK) {
        return false;
    }

    error = esp_ota_set_boot_partition(static_cast<const esp_partition_t*>(m_update_partition));
    return error == ESP_OK;
}

#endif // THINGSBOARD_USE_ESP_PARTITION
