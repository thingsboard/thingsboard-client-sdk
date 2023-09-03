/*
  ESP32_Updater.h - Library API for sending data to the ThingsBoard
  Based on PubSub MQTT library.
  Created by Olender M. Oct 2018.
  Released into the public domain.
*/
#ifndef ESP32_Updater_h
#define ESP32_Updater_h

// Local include.
#include "Configuration.h"

#if THINGSBOARD_ENABLE_OTA

#ifdef ESP32

// Library include.
#include <IUpdater.h>

class ESP32_Updater : public IUpdater {
  public:
    bool begin(const size_t& firmware_size) override;
  
    size_t write(uint8_t* payload, const size_t& total_bytes) override;

    bool reset() override;
  
    bool end() override;
};

#endif // ESP32

#endif // THINGSBOARD_ENABLE_OTA

#endif // ESP32_Updater_h
