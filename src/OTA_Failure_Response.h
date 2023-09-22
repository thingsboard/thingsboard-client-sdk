#ifndef OTA_Failure_Response_h
#define OTA_Failure_Response_h

// Library include.
#include <stdint.h>


/// @brief Possible responses to error states the OTA update might fall into,
/// allows to react to certain issues in the most appropriate way, because some of them require us to restart the complete update,
/// whereas other issues can be solved if we simply attempt to refetch the current chunk
enum class OTA_Failure_Response : const uint8_t {
    RETRY_CHUNK, // Fetching the current chunk failed somehow, but we can still continue the update we just have to refetch the current chunk, mainly occurs from timeouts with requesting the chunks
    RETRY_UPDATE, // Internal process failed in the OTA that makes the complete already downloaded data not recoverable anymore, hashing or writing to flash memory failed, requires to restart the update from the first chunk and reinitalize the needed components
    RETRY_NOTHING // Initally passed arguments are invalid and would cause crashes or the update was forcefully stopped by the user, therefore we immediately stop the update and do not restart it
};

#endif // OTA_Failure_Response
