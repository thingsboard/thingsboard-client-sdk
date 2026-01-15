#ifndef MQTT_Connection_Error_h
#define MQTT_Connection_Error_h

// Library include.
#include <stdint.h>


/// @brief Possible error states the MQTT broker connection can be in, if connecting the device to the MQTT broker failed
enum class MQTT_Connection_Error : uint8_t {
    NONE, ///< Connection accepted
    REFUSE_PROTOCOL, ///< Connection failed because of a non matching MQTT protocol version used by the device
    REFUSE_ID_REJECTED, ///< Connection failed because the client ID is in the correct format, but not allowed by the server because of other restrictions
    REFUSE_SERVER_UNAVAILABLE, ///< Connection failed because the MQTT server is unavailable
    REFUSE_BAD_USERNAME, ///< Connection failed because the data in the username or password is in the wrong format
    REFUSE_NOT_AUTHORIZED ///< Connection failed because the client is not authorized
};

#endif // MQTT_Connection_Error_h
