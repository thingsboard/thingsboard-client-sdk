// Header include.
#include "Espressif_Updater.h"

#if THINGSBOARD_ENABLE_OTA

#if THINGSBOARD_USE_ESP_PARTITION

// Library include.
#include <esp_ota_ops.h>
#include <esp_spi_flash.h>
#include <string.h>


constexpr size_t SPI_FLASH_ALIGNMENT = SPI_FLASH_SEC_SIZE;
constexpr size_t SPI_FLASH_BLOCK_SIZE = SPI_FLASH_ALIGNMENT * SPI_FLASH_HEADER_SIZE;


Espressif_Updater::Espressif_Updater() :
    m_data_buffer(nullptr),
    m_start_data_buffer(),
    m_written_bytes_amount(0U),
    m_total_bytes_amount(0U),
    m_buffer_length(0U),
    m_ota_partition(nullptr)
{
    // Nothing to do
}

bool Espressif_Updater::begin(const size_t& firmware_size) {
    if (m_total_bytes_amount) {
        return false;
    }

    reset();

    const esp_partition_t *ota_parition = esp_ota_get_next_update_partition(nullptr);

    if (ota_parition == nullptr) {
        return false;
    }

    if (firmware_size > ota_parition->size) {
        return false;
    }

    m_total_bytes_amount = firmware_size;
    m_ota_partition = ota_parition;
    return true;
}

size_t Espressif_Updater::write(uint8_t* payload, const size_t& total_bytes) {
    if (!started_update()) {
        return 0U;
    }

    if (total_bytes > remaining_bytes()) {
        reset();
        return 0U;
    }

    size_t bytes_to_write = total_bytes;

    while ((m_buffer_length + bytes_to_write) > SPI_FLASH_ALIGNMENT) {
        size_t buffer_size = SPI_FLASH_ALIGNMENT - m_buffer_length;
        memcpy(m_data_buffer + m_buffer_length, payload + (total_bytes - bytes_to_write), buffer_size);
        m_buffer_length += buffer_size;
        if (!write_buffer()) {
            return total_bytes - bytes_to_write;
        }
        bytes_to_write -= buffer_size;
    }

    memcpy(m_data_buffer + m_buffer_length, payload + (total_bytes - bytes_to_write), bytes_to_write);
    m_buffer_length += bytes_to_write;

    if (m_buffer_length == remaining_bytes()) {
        if (!write_buffer()) {
            return total_bytes - bytes_to_write;
        }
    }
    return total_bytes;
}

void Espressif_Updater::reset() {
    delete[] m_data_buffer;
    m_written_bytes_amount = 0U;
    m_total_bytes_amount = 0U;
    m_buffer_length = 0U;
    m_ota_partition = nullptr;
}

bool Espressif_Updater::end() {
    if (m_total_bytes_amount == 0) {
        return false;
    }

    if (!is_finished()) {
        reset();
        return false;
    }

    return verify_end();
}

bool Espressif_Updater::write_buffer() {
    // First x amount of bytes of the firmware, which are initally not written to ensure a not completly written firmware is not bootable
    size_t skip_bytes_amount = 0U;

    if (m_written_bytes_amount == 0U) {
        // Check wheter the magic byte has been set or not
        if (m_data_buffer[0] != ESP_IMAGE_HEADER_MAGIC) {
            reset();
            return false;
        }

        // Stash the first 16 bytes of data and set the offset so they are
        // not written at this point so that partially written firmware
        // will not be bootable
        skip_bytes_amount = SPI_FLASH_HEADER_SIZE;
        memcpy(m_start_data_buffer, m_data_buffer, SPI_FLASH_HEADER_SIZE);
    }
    
    const esp_partition_t *partition = static_cast<const esp_partition_t*>(m_ota_partition);

    size_t offset = partition->address + m_written_bytes_amount;
    bool block_erase = (m_total_bytes_amount - m_written_bytes_amount >= SPI_FLASH_BLOCK_SIZE) && (offset % SPI_FLASH_BLOCK_SIZE == 0);             // if it's the block boundary, than erase the whole block from here
    bool part_head_sectors = partition->address % SPI_FLASH_BLOCK_SIZE && offset < (partition->address / SPI_FLASH_BLOCK_SIZE + 1) * SPI_FLASH_BLOCK_SIZE;    // sector belong to unaligned partition heading block
    bool part_tail_sectors = offset >= (partition->address + m_total_bytes_amount) / SPI_FLASH_BLOCK_SIZE * SPI_FLASH_BLOCK_SIZE;     // sector belong to unaligned partition tailing block
    if (block_erase || part_head_sectors || part_tail_sectors) {
        const esp_err_t error = esp_partition_erase_range(partition, m_written_bytes_amount, block_erase ? SPI_FLASH_BLOCK_SIZE : SPI_FLASH_ALIGNMENT);
        if (error != ESP_OK) {
            reset();
            return false;
        }
    }

    // try to skip empty blocks on unecrypted partitions
    size_t skip_block_amount = skip_bytes_amount / sizeof(uint32_t);
    if ((partition->encrypted || check_if_block_is_empty(m_data_buffer + skip_block_amount, m_buffer_length - skip_bytes_amount)) && esp_partition_write(partition, m_written_bytes_amount + skip_bytes_amount, reinterpret_cast<uint32_t*>(m_data_buffer) + skip_block_amount, m_buffer_length - skip_bytes_amount) != ESP_OK) {
        reset();
        return false;
    }

    // Restore magic byte
    if (m_written_bytes_amount == 0U) {
        m_data_buffer[0] = ESP_IMAGE_HEADER_MAGIC;
    }

    m_written_bytes_amount += m_buffer_length;
    m_buffer_length = 0;
    return true;
}

bool Espressif_Updater::verify_end() {
    if (!write_start_data() || !is_partition_bootable()) {
        reset();
        return false;
    }

    if (esp_ota_set_boot_partition(static_cast<const esp_partition_t*>(m_ota_partition))) {
        reset();
        return false;
    }

    reset();
    return true;
}

bool Espressif_Updater::started_update() const {
    return m_total_bytes_amount != 0U;
}

size_t Espressif_Updater::remaining_bytes() const {
    return m_total_bytes_amount - m_written_bytes_amount;
}

bool Espressif_Updater::is_partition_bootable() const {
    if (m_ota_partition == nullptr) {
        return false;
    }

    uint8_t buf[SPI_FLASH_HEADER_SIZE];
    const esp_err_t error = esp_partition_read(static_cast<const esp_partition_t*>(m_ota_partition), 0, (uint32_t*)buf, SPI_FLASH_HEADER_SIZE);
    if (error != ESP_OK) {
        return false;
    }

    if (buf[0] != ESP_IMAGE_HEADER_MAGIC) {
        return false;
    }
    return true;
}

bool Espressif_Updater::write_start_data() const {
    if (m_ota_partition == nullptr) {
        return false;
    }

    const esp_err_t error = esp_partition_write(static_cast<const esp_partition_t*>(m_ota_partition), 0, reinterpret_cast<const uint32_t*>(m_start_data_buffer), SPI_FLASH_HEADER_SIZE);
    return error == ESP_OK;
}

bool Espressif_Updater::is_finished() const {
    return m_total_bytes_amount == m_written_bytes_amount;
}

bool Espressif_Updater::check_if_block_is_empty(const uint8_t* payload, const size_t& total_bytes) const {
    // check 32-bit aligned blocks only
    if (!total_bytes || total_bytes % sizeof(uint32_t)) {
        return true;
    }

    size_t dwl = total_bytes / sizeof(uint32_t);

    do {
        // for SPI NOR flash empty blocks are all one's, i.e. filled with 0xff byte
        if (*(uint32_t*)payload ^ 0xffffffff) {
            return true;
        }

        payload += sizeof(uint32_t);
    } while (--dwl);
    return false;
}

#endif // THINGSBOARD_USE_ESP_PARTITION

#endif // THINGSBOARD_ENABLE_OTA
