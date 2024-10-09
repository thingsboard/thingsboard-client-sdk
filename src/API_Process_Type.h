#ifndef API_Process_Type_h
#define API_Process_Type_h

// Library include.
#include <stdint.h>


/// @brief Possible processing types an API Implementation uses to handle responses from the server.
/// Only ever uses one at the time, because the response is either unserialized data which we need to process as such (OTA Firmware Update)
/// or actually JSON which needs to be serialized (everything else)
enum class API_Process_Type : uint8_t {
    RAW, ///< Passes the data into the process method as a copy but in its raw uint8_t array form
    JSON ///< Passes the data into the process method as a copy and in a serialized manner
};

#endif // API_Process_Type_h
