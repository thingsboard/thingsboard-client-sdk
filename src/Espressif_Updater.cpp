// Header include.
#include "Espressif_Updater.h"

#if THINGSBOARD_ENABLE_OTA

#if THINGSBOARD_USE_ESP_PARTITION

// Library include.
#include <esp_ota_ops.h>


Espressif_Updater::Espressif_Updater() :
    m_ota_handle(0U),
    m_update_partition(nullptr)
{
    // Nothing to do
}

bool Espressif_Updater::begin(const size_t& firmware_size) {
    const esp_partition_t *running = esp_ota_get_running_partition();
    const esp_partition_t *configured = esp_ota_get_boot_partition();

    if (configured != running) {
        return false;
    }

    const esp_partition_t *update_partition = esp_ota_get_next_update_partition(nullptr);

    if (update_partition == nullptr) {
        return false;
    }

    // Temporary handle is used, because it allows using a void* as the actual ota_handle,
    // allowing us to only include the esp_ota_ops header in the defintion (.cpp) file,
    // instead of also needing to declare it in the declaration (.h) header file
    esp_ota_handle_t ota_handle;
    const esp_err_t error = esp_ota_begin(update_partition, firmware_size, &ota_handle);

    if (error != ESP_OK) {
        return false;
    }

    m_ota_handle = ota_handle;
    m_update_partition = update_partition;
    return true;
}

size_t Espressif_Updater::write(uint8_t* payload, const size_t& total_bytes) {
    const esp_err_t error = esp_ota_write(m_ota_handle, payload, total_bytes);
    const size_t written_bytes = (error == ESP_OK) ? total_bytes : 0U;
    return written_bytes;
}

void Espressif_Updater::reset() {
    (void)esp_ota_abort(m_ota_handle);
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

#endif // THINGSBOARD_ENABLE_OTA
