// Header include.
#include "Espressif_Updater.h"

#if THINGSBOARD_ENABLE_OTA

#if THINGSBOARD_USE_ESP_PARTITION

// Library include.
#include <algorithm>
#include <iterator>
#include <esp_ota_ops.h>
#include <esp_idf_version.h>
// ESP_IDF_VERSION_MAJOR Version 5 is a major breaking changes were the old esp_spi_flash.h header has been deprecated and spi_flash_mmap should be used instead.
#if ESP_IDF_VERSION_MAJOR < 5
#include <esp_spi_flash.h>
#else
#include <spi_flash_mmap.h>
#endif // ESP_IDF_VERSION_MAJOR < 5
#include <esp_app_format.h>


#include <esp_log.h>


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
    if (started_update() || firmware_size == 0U) {
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

    if (firmware_size == OTA_SIZE_UNKNOWN) {
        m_total_bytes_amount = ota_parition->size;
    }

    // Buffer to write memory is created with the needed alignment size,
    // writing less or not to 4KB aligned data to flash memory is not possible
    m_data_buffer = new uint8_t[SPI_FLASH_ALIGNMENT];
    
    if (m_data_buffer == nullptr){
        return false;
    }

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
        const size_t buffer_size = SPI_FLASH_ALIGNMENT - m_buffer_length;
        std::copy(payload + (total_bytes - bytes_to_write), payload + buffer_size, m_data_buffer);
        m_buffer_length += buffer_size;
        if (!write_buffer()) {
            return total_bytes - bytes_to_write;
        }
        bytes_to_write -= buffer_size;
    }

    std::copy(payload + (total_bytes - bytes_to_write), payload + bytes_to_write, m_data_buffer);
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
        std::copy(m_data_buffer, m_data_buffer + skip_bytes_amount, m_start_data_buffer);
    }
    
    const esp_partition_t *partition = static_cast<const esp_partition_t*>(m_ota_partition);

    const size_t write_address = partition->address + m_written_bytes_amount;
    // If it's the block boundary, than erase the whole block from here
    const bool block_erase = (m_total_bytes_amount - m_written_bytes_amount >= SPI_FLASH_BLOCK_SIZE) && (write_address % SPI_FLASH_BLOCK_SIZE == 0);
    // Sector belongs to unaligned partition heading block
    const bool part_head_sectors = partition->address % SPI_FLASH_BLOCK_SIZE && write_address < (partition->address / SPI_FLASH_BLOCK_SIZE + 1) * SPI_FLASH_BLOCK_SIZE;
    // Sector belongs to unaligned partition tailing block
    const bool part_tail_sectors = write_address >= (partition->address + m_total_bytes_amount) / (SPI_FLASH_BLOCK_SIZE * SPI_FLASH_BLOCK_SIZE);

    if (block_erase || part_head_sectors || part_tail_sectors) {
        const esp_err_t error = esp_partition_erase_range(partition, m_written_bytes_amount, block_erase ? SPI_FLASH_BLOCK_SIZE : SPI_FLASH_ALIGNMENT);
        if (error != ESP_OK) {
            reset();
            return false;
        }
    }

    // Try to skip empty blocks on unecrypted partitions
    const size_t skip_block_amount = skip_bytes_amount / sizeof(uint32_t);
    const bool block_is_empty = (partition->encrypted || check_if_block_is_not_empty(m_data_buffer + skip_block_amount, m_buffer_length - skip_bytes_amount));
    if (block_is_empty) {
        const esp_err_t error = esp_partition_write(partition, m_written_bytes_amount + skip_bytes_amount, reinterpret_cast<uint32_t*>(m_data_buffer) + skip_block_amount, m_buffer_length - skip_bytes_amount);
        if (error != ESP_OK) {
            reset();
            return false;
        }
    }

    m_written_bytes_amount += m_buffer_length;
    m_buffer_length = 0;
    return true;





    //first bytes of new firmware
    uint8_t skip = 0;
    if(!_progress && _command == U_FLASH){
        //check magic
        if(_buffer[0] != ESP_IMAGE_HEADER_MAGIC){
            _abort(UPDATE_ERROR_MAGIC_BYTE);
            return false;
        }

        //Stash the first 16 bytes of data and set the offset so they are
        //not written at this point so that partially written firmware
        //will not be bootable
        skip = ENCRYPTED_BLOCK_SIZE;
        _skipBuffer = (uint8_t*)malloc(skip);
        if(!_skipBuffer){
            log_e("malloc failed");
        return false;
        }
        memcpy(_skipBuffer, _buffer, skip);
    }
    if (!_progress && _progress_callback) {
        _progress_callback(0, _size);
    }
    size_t offset = _partition->address + _progress;
    bool block_erase = (_size - _progress >= SPI_FLASH_BLOCK_SIZE) && (offset % SPI_FLASH_BLOCK_SIZE == 0);             // if it's the block boundary, than erase the whole block from here
    bool part_head_sectors = _partition->address % SPI_FLASH_BLOCK_SIZE && offset < (_partition->address / SPI_FLASH_BLOCK_SIZE + 1) * SPI_FLASH_BLOCK_SIZE;    // sector belong to unaligned partition heading block
    bool part_tail_sectors = offset >= (_partition->address + _size) / (SPI_FLASH_BLOCK_SIZE * SPI_FLASH_BLOCK_SIZE);     // sector belong to unaligned partition tailing block
    if (block_erase || part_head_sectors || part_tail_sectors){
        if(!ESP.partitionEraseRange(_partition, _progress, block_erase ? SPI_FLASH_BLOCK_SIZE : SPI_FLASH_SEC_SIZE)){
            _abort(UPDATE_ERROR_ERASE);
            return false;
        }
    }

    // try to skip empty blocks on unecrypted partitions
    if ((_partition->encrypted || check_if_block_is_not_empty(_buffer + skip/sizeof(uint32_t), _bufferLen - skip)) && !ESP.partitionWrite(_partition, _progress + skip, (uint32_t*)_buffer + skip/sizeof(uint32_t), _bufferLen - skip)) {
        reset();
        return false;
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
    const esp_err_t error = esp_partition_read(static_cast<const esp_partition_t*>(m_ota_partition), 0U, (uint32_t*)buf, SPI_FLASH_HEADER_SIZE);
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

    const esp_err_t error = esp_partition_write(static_cast<const esp_partition_t*>(m_ota_partition), 0U, reinterpret_cast<const uint32_t*>(m_start_data_buffer), SPI_FLASH_HEADER_SIZE);
    return error == ESP_OK;
}

bool Espressif_Updater::is_finished() const {
    return m_total_bytes_amount == m_written_bytes_amount;
}

bool Espressif_Updater::check_if_block_is_not_empty(const uint8_t* payload, const size_t& total_bytes) const {
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
