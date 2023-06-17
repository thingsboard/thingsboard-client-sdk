/*
  OTA_Failure_Response.h - Library API for sending data to the ThingsBoard
  Based on PubSub MQTT library.
  Created by Olender M. Oct 2018.
  Released into the public domain.
*/
#ifndef OTA_Failure_Response_h
#define OTA_Failure_Response_h

// Library include.
#include <stdint.h>

enum class OTA_Failure_Response : const uint8_t {
    RETRY_CHUNK,
    RETRY_UPDATE,
    RETRY_NOTHING
};

#endif // OTA_Failure_Response
