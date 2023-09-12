/*
  Espressif_Updater.h - Library API for sending data to the ThingsBoard
  Based on PubSub MQTT library.
  Created by Olender M. Oct 2018.
  Released into the public domain.
*/
#ifndef Espressif_Updater_h
#define Espressif_Updater_h

// Local include.
#include "Configuration.h"

#if THINGSBOARD_ENABLE_OTA

#if THINGSBOARD_USE_ESP_PARTITION

// Local include.
#include "IUpdater.h"


constexpr size_t SPI_FLASH_HEADER_SIZE = 16U;


class Espressif_Updater : public IUpdater {
  public:
    Espressif_Updater();

    bool begin(const size_t& firmware_size) override;
  
    size_t write(uint8_t* payload, const size_t& total_bytes) override;

    void reset() override;
  
    bool end() override;
  
  private:
    uint8_t *m_data_buffer;
    // First 16 bytes are seperated so a partially written firmware is not bootable,
    // is written back at the correct place in flash memory at the successfull end of the update.
    uint8_t m_start_data_buffer[SPI_FLASH_HEADER_SIZE];
    size_t m_written_bytes_amount;
    size_t m_total_bytes_amount;
    size_t m_buffer_length;
    const void *m_ota_partition;

    bool write_buffer();

    bool verify_end();

    bool started_update() const;
    
    size_t remaining_bytes() const;

    bool is_partition_bootable() const;

    bool write_start_data() const;

    bool is_finished() const;

    bool check_if_block_is_empty(const uint8_t* payload, const size_t& total_bytes) const;

};

#endif // THINGSBOARD_USE_ESP_PARTITION

#endif // THINGSBOARD_ENABLE_OTA

#endif // Espressif_Updater_h
