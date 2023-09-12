/*
  Arduino_ESP8266_Updater.h - Library API for sending data to the ThingsBoard
  Based on PubSub MQTT library.
  Created by Olender M. Oct 2018.
  Released into the public domain.
*/
#ifndef Arduino_ESP8266_Updater_h
#define Arduino_ESP8266_Updater_h

// Local include.
#include "Configuration.h"

#if THINGSBOARD_ENABLE_OTA

#if defined(ESP8266) && defined(ARDUINO)

// Local include.
#include "IUpdater.h"


class Arduino_ESP8266_Updater : public IUpdater {
  public:
    bool begin(const size_t& firmware_size) override;
  
    size_t write(uint8_t* payload, const size_t& total_bytes) override;

    void reset() override;
  
    bool end() override;
};

#endif // defined(ESP8266) && defined(ARDUINO)

#endif // THINGSBOARD_ENABLE_OTA

#endif // Arduino_ESP8266_Updater_h
